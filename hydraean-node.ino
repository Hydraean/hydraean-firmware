/*
Hydraean Node
Firmware for Hydraean Node Devices
----------------------------------
Author: Bryce Narciso C. Mercines
*/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Most performant ISM Band (Philippines) good RSSI
#define BAND 923E6

String LoRaData;

// Replace with your network credentials
const char *ssid = "Hydraean_Node";
const char *password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature()
{
  return String(random(100));
}

String readDHTHumidity()
{
  return String(random(100));
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Seantinel</title>
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }

    div{
      width: 100%;
      display: flex;
      flex-direction: column;
    }

     button{
        background: #000;
        padding: 10px;
       border: 0px;
       border-radius: 5px;
       font-size: 20px;
       color: #fff;
       margin-bottom: 20px;
      }
  </style>
</head>
<body>
  <h2>Seantinel</h2>
  <center>
   <div>
     <button onclick="sendAlert()">
       Illegal Fishing
     </button>
     <button onclick="sendHelp()">
       Call for Rescue
     </button>
     <div>
       </center>
</body>
<script>
  function sendAlert(){
     fetch("http://192.168.4.1/alert")
  }

   function sendHelp(){
     fetch("http://192.168.4.1/help")
  }
</script>
</html>
)rawliteral";

void sendData(String LORA_DATA)
{
  LoRa.beginPacket();
  LoRa.print(LORA_DATA);
  LoRa.endPacket();
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.on("/alert", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", "Sending Alert");
    sendData("Alert!");
  });

  server.on("/help", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", "Sending Help");
    sendData("Help");
  });

  // Start server
  server.begin();
}

void loop()
{
  //try to parse packet
  //  int packetSize = LoRa.parsePacket();
  //  if (packetSize)
  //  {
  //    //received a packet
  //
  //    //read packet
  //    while (LoRa.available())
  //    {
  //      LoRaData = LoRa.readString();
  //      sendData(LoRaData);
  //      Serial.println("Recieved Echo Data:---> " + LoRaData);
  //      delay(2000);
  //    }
  //}
}