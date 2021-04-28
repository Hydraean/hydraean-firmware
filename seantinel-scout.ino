/*
Seantinel Scout
----------------------------------
This is the firmware for the seantinel scout device,
which is used to list for LoRa data transmissions and display the data on a HTTP endpoint
----------------------------------
Author: Bryce Narciso C. Mercines (c) 2021
version: 0.0.2
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

// device unique ID (can be changed)

String UID = "HSC-00001";

String LoRaData;

// current scout data
String current_data = "";

// Replace with your network credentials
const char *ssid = "Seantinel_Scout_1";
const char *password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String device_data = "{\"type\": \"Seantinel Scout\", \"version\": 0.0.1, \"firmware\": \"Scout\",\"device_id\": \"HN-XXXX\"}";


void sendData(String LORA_DATA)
{
  LoRa.beginPacket();
  LoRa.print(LORA_DATA);
  LoRa.endPacket();
}

void echoMechanism()
{
  // testing echo
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    //received a packet

    //read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
      sendData(LoRaData);
      delay(2000);
    }
  }
  else
  {
    delay(3000);
  }
}

void setup()
{
  // Serial Port for communicating to GPS Module
  Serial.begin(9600);

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
  display.println("SEANTINEL SCOUT");
  display.println("SCOUT ID: " + UID);
  display.display();

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
  {
    // Starting LoRa Fails
    while (1)
      ;
  }

  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    device_data.replace("HN-XXXX",UID);
    request->send(200, "text/plain", device_data);
  });

  // for sending data (test)
  server.on("/scout/incidents", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", LoRaData);
  });

  // Start server
  server.begin();
}

void loop()
{

  // for echoing data
  // testing echo .
  int packetSize = LoRa.parsePacket();
  Serial.println("pz: " + packetSize);
  if (packetSize)
  {
    //received a packet

    //read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
      current_data = LoRa.readString();
    }

    sendData("e1:" + LoRaData);
    Serial.println("e1:" + LoRaData);
  }


}