
/*
Hydraean Gateway
Firmware for Hydraean Gateway Devices
Gateway Devices are designed to recieve data from nodes, and attempt to broadcast the data to the internet
and be able to return a response if the data alert, or report has been confirmed form the internet.
----------------------------------
Author: Bryce Narciso C. Mercines
*/

// API URL
String API_URL = "http://192.168.1.4:8000/";

#include <HTTPClient.h>
#include "WiFi.h"

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

// LoRa Data
String LoRaData;

HTTPClient http;

// Replace with your network credentials
const char *ssid = "PLDTHOMEFIBR9VY8P";
const char *password = "PLDTWIFIT2Z5R";

// gateway unique id
const char *uid = "HTX-00001";

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
  display.print("HYDRAEAN GATEWAY");
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

  WiFi.begin(ssid, password);
}

// Send LoRa packet

void sendData(String LORA_DATA)
{
  LoRa.beginPacket();
  LoRa.print(LORA_DATA);
  LoRa.endPacket();
}

// update OLED Screen

void setScreen(int yrssi, String message)
{
  // Dsiplay information
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("HYDRAEAN GATEWAY");
  display.println(WiFi.localIP());
  display.setCursor(0, 20);
  display.print("Activity");
  display.setCursor(0, 30);
  display.print(message);
  display.setCursor(0, 40);
  display.print("RSSI:");
  display.setCursor(30, 40);
  display.print(yrssi);
  display.display();
}

// attempt to send data to the internet

void gateWayConnect(String LORA_DATA, int xrssi)
{

  setScreen(xrssi, "Connection Attempt!");
  if (WiFi.status() == WL_CONNECTED)
  {
    String reqURL = API_URL + "?data=" + LORA_DATA;
    http.begin(reqURL);
    http.GET();
    Serial.println(reqURL);
    setScreen(xrssi, "Data sent!");
    http.end(); //Close connection
    delay(2000);
  }
  else
  {
    sendData(LORA_DATA);
    setScreen(xrssi, "Echoing..");
    display.setCursor(0, 10);
  }
}

void loop()
{
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    //received a packet

    //read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    gateWayConnect(LoRaData, rssi);
  }
}