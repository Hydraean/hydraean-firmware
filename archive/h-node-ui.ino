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

const char index_html[] PROGMEM = R"rawliteral(
<!doctype html><html><head><style>

html {
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }

body{
  background: #07042C;
  color: #fff;
  font-family: Arial, Helvetica, sans-serif;
  width: 100%;
  padding-top: 40px;
}

    h2 {
      font-size: 1.8rem;
      font-weight: 100;
      margin-top: 5px;
     }

        div {
      width: 100%;
      display: flex;
      flex-direction: column;
      max-width: 750px;
    }

     button{
        background: rgb(31, 38, 100);
        padding: 20px;
       border: 0px;
       border-radius: 5px;
       font-size: 20px;
       color: #fff;
       margin-bottom: 20px;
      }

       #sbt{
        background: rgb(18, 71, 156);
      }

      .sd{
        background: rgb(74, 207, 174) !important;
        box-shadow: -1px 17px 31px -10px rgba(52, 224, 204, 0.876) !important;
      }

     </style></head><body>
      <script nomodule src="https://unpkg.com/browser-es-module-loader/dist/babel-browser-build.js"></script>
      <script nomodule src="https://unpkg.com/browser-es-module-loader"></script>
    <!DOCTYPE HTML>
<html>
<head>
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0">
  <meta name="theme-color" content="#07042C">
  <title>Seantinel</title>
  </style>
</head>

<body>
<?xml version="1.0" encoding="UTF-8"?>
<svg width="48px" height="48px" viewBox="0 0 48 48" version="1.1" xmlns="http://www.w3.org/2000/svg"
  xmlns:xlink="http://www.w3.org/1999/xlink">
  <defs>
    <polygon id="path-1"
      points="6.46445498 6.46445498 37.535545 6.46445498 37.535545 37.535545 6.46445498 37.535545"></polygon>
  </defs>
  <g id="Web" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd">
    <g id="Oval-2" transform="translate(2.000000, 2.000000)">
      <mask id="mask-2" fill="white">
        <use xlink:href="#path-1"></use>
      </mask>
      <use id="Rectangle" stroke="#FFFFFF" stroke-width="2"
        transform="translate(22.000000, 22.000000) rotate(-315.000000) translate(-22.000000, -22.000000) "
        xlink:href="#path-1"></use>
      <polyline id="Path-4" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)" points="17.5 10 22 5 26.5 10">
      </polyline>
      <polyline id="Path-4" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
        transform="translate(35.500000, 21.500000) rotate(-270.000000) translate(-35.500000, -21.500000) "
        points="31 24 35.5 19 40 24"></polyline>
      <polyline id="Path-4" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
        transform="translate(7.500000, 22.000000) rotate(-90.000000) translate(-7.500000, -22.000000) "
        points="3 24.5 7.5 19.5 12 24.5"></polyline>
      <path
        d="M15.1503091,25.9999088 L22.0281203,32.7593626 L26.8720004,27.9999646 C28.5576209,28.5342426 30.1175778,28.9626562 31.8242306,28.9626562 C34.6602581,28.9626562 37.3855145,28.216336 40,26.7236955 L36.42819,36.0276813 L20.1303788,48 L6,32.7593626 L7.65707113,29.4781856 C9.41978426,27.412858 11.9175303,26.2534324 15.1503091,25.9999088 Z"
        id="Combined-Shape" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"></path>
      <path
        d="M18.885947,30.0028508 L21.979209,33.0045111 L24.2883312,30.7636175 C26.3568126,31.282893 27.9877342,31.902255 30.0194005,32.0839307 C32.2799198,32.2860709 34.2734529,32.096538 36,31.515332 L36,39.0373962 L19.946487,49 L9,36.2813923 L10.6801673,32.995338 C12.9580342,30.9368416 15.6932942,29.9393459 18.885947,30.0028508 Z"
        id="Combined-Shape" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"></path>
      <path
        d="M13.6767091,36.2630943 C15.9620376,34.0876981 18.0194473,33 19.848938,33 C22.5931742,33 23.2206654,33.5540883 26.9756992,34.225247 C29.4790551,34.6726861 31.4172133,34.7467369 32.7901737,34.4473994 L38,40.4096419 L20.4764292,44 L12,39.6202858 L13.6767091,36.2630943 Z"
        id="Path-3" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"></path>
      <path
        d="M16.8298616,39.5094364 C18.7361072,37.3593856 20.4522404,36.2843602 21.9782611,36.2843602 C24.2672922,36.2843602 24.7906972,36.8319929 27.9228581,37.4953319 C30.0109654,37.9375579 31.6276281,38.010746 32.7728463,37.714896 L37.1184834,43.6076729 L22.5016661,47.1561999 L15.4312796,42.8275135 L16.8298616,39.5094364 Z"
        id="Path-3" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
        transform="translate(26.274882, 41.720280) rotate(-8.000000) translate(-26.274882, -41.720280) "></path>
      <polygon id="Rectangle" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
        transform="translate(21.895735, 21.895735) rotate(-315.000000) translate(-21.895735, -21.895735) "
        points="14.3886256 14.3886256 29.4028436 14.3886256 29.4028436 29.4028436 14.3886256 29.4028436">
      </polygon>
      <polygon id="Rectangle" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
        transform="translate(22.000000, 22.000000) rotate(-315.000000) translate(-22.000000, -22.000000) "
        points="19.1848341 19.1848341 24.8151659 19.1848341 24.8151659 24.8151659 19.1848341 24.8151659">
      </polygon>
      <line x1="10.835" y1="21.875" x2="-0.055" y2="22.125" id="Line-4" stroke="#FFFFFF" stroke-width="2"
        stroke-linecap="square" mask="url(#mask-2)"></line>
      <line x1="43" y1="21.5" x2="32.1078431" y2="21.75" id="Line-4" stroke="#FFFFFF" stroke-width="2"
        stroke-linecap="square" mask="url(#mask-2)"></line>
      <line x1="21.875" y1="10.945" x2="22.125" y2="0.055" id="Line-4" stroke="#FFFFFF" stroke-width="2"
        stroke-linecap="square" mask="url(#mask-2)"></line>
    </g>
  </g>
</svg>
  <h2>Seantinel</h2>
  <center>
    <div>
      <button onclick="sendAlert()">
       Illegal Fishing
     </button>
      <button onclick="sendHelp()">
       Call for Rescue
     </button>
      <textarea placeholder="Message to send: (500 chars)" id="text" style="height: 200px; padding: 20px;"></textarea>
      <br/>
      <button id="sbt" class="sd"  onclick="sendText()">
        Send
     </button>

    <button onclick="st()">
         Stop Sending
    </button>

      <div>
  </center>
</body>
</html><script type="text/javascript">const ax = "http://192.168.4.1"
const el = (x) => {return document.querySelector(x)}
const jp = (x) => {return JSON.parse(x)}
const js = (x) => {return JSON.stringify(x)}
const s = (x) => {el(x).style.display = "block"}
const h = (x) => {el(x).style.display = "none"}

function sendAlert() {
  fetch(`${ax}/alert`)
}

function sendHelp() {
  fetch(`${ax}/help`)
}

function sendText() {
  let data = document.getElementById('text').value;
  fetch(`${ax}/report?text=${data}`)
}

function st(){

}</script></body><html>
)rawliteral";

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
      Serial.println("Recieved Echo Data:---> " + LoRaData);
      delay(2000);
    }
  }
  else
  {
    Serial.println("Idle..");
    delay(3000);
  }
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

  server.on("/alert", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", "Sending Alert");
    sendData("Alert!");
    Serial.println("sending data!!! Alert");
    echoMechanism();
  });

  // for sending help (test)
  server.on("/help", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", "Sending Help");
    sendData("Help");
  });

  // for sending data (test)
  server.on("/report", HTTP_GET, [](AsyncWebServerRequest *request) {
    // try and capture parameters
    int paramsNr = request->params();

    for (int i = 0; i < paramsNr; i++)
    {

      AsyncWebParameter *p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");

      sendData(p->value());
      echoMechanism();
    }

    request->send_P(200, "text/plain", "ok!");
  });

  // Start server
  server.begin();
}

void loop()
{
  echoMechanism();
}