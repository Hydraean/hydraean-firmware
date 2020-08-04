/*
Hydraean Node
Firmware for Hydraean Node Devices
*/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Most performant ISM Band (Philippines) good RSSI
#define BAND 923E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Initialize Screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// Replace with your network credentials
const char *ssid = "Hydraean";
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

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("HYDRAEAN NODE");
  display.display();

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
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);

  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  display.println("AP IP address: ");
  display.println(IP);
  display.display();

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
    LoRa.beginPacket();
    LoRa.print("Alert!");
    LoRa.endPacket();
  });

  server.on("/help", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", "Sending Help");
    LoRa.beginPacket();
    LoRa.print("Help!");
    LoRa.endPacket();
  });

  // Start server
  server.begin();
}

void loop()
{
}