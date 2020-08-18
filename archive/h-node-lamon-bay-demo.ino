/*
Hydraean Node
Firmware for Hydraean Node Devices
----------------------------------
Author: Bryce Narciso C. Mercines
*/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// for Parsing GPS Data
#include <TinyGPS++.h>

// The TinyGPS++ object
TinyGPSPlus gps;
String GPS_DATA;

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

// define hardware triggers
#define RESCUE_BTN 17
#define ILLEGAL_BTN 16

// LED Indicator
#define INDICATOR 23

// Most performant ISM Band (Philippines) good RSSI
#define BAND 923E6

// device unique ID

String UID = "HN-0001";

String LoRaData;

// Replace with your network credentials
const char *ssid = "Seantinel_Node";
const char *password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!doctype html><html><head><style>body {
  background: #07042C;
  color: #fff;
  font-family: Arial, Helvetica, sans-serif;
  width: 100%;
  padding-top: 20px;
  text-align: center;
}

h2 {
  font-size: 1.8rem;
  font-weight: 100;
  margin-top: 5px;
}

.content {
  width: 100%;
  height: 100vh;
  display: flex;
  flex-direction: column;
  max-width: 750px;
}

button {
  background: rgb(31, 38, 100);
  padding: 20px 30px;
  border: 0px;
  font-size: 20px;
  color: #fff;
  width: 100%
}

#sbt {
  background: rgb(18, 71, 156);
}

.sd {
  background: linear-gradient(to right, rgb(74, 207, 174), rgb(0, 204, 255)) !important;
  box-shadow: -1px 17px 31px -10px rgba(52, 224, 204, 0.876) !important;
}

.at {
  background: rgb(13, 133, 185) !important;
  box-shadow: -1px 2px 10px -1px rgba(50, 168, 219, 0.84) !important;
}

.am {
  background: rgb(185, 13, 90) !important;
  box-shadow: -1px 2px 10px -1px rgba(219, 50, 216, 0.84) !important;
}

#text {
  height: 120px;
  padding: 20px;
  background: rgb(10, 10, 37);
  color: #fff;
  font-size: 15px;
     border-radius: 5px;
  border: 2px solid rgb(72, 235, 186);
}

.df {
  display: flex;
  justify-content: center;
  width: 100%;
}

#fs {
  display: none;
  margin-top: 20px;
}

.screen-ut {
  display: none;
  margin-top: 20px;
}

.card {
  background: rgba(99, 98, 101, 0.5);
  border-radius: 3px;
  padding: 0px;
  text-align: left;
  width: 88%;
  margin-bottom: 20px;
  display: flex;
  flex-direction: column;
}

.card span {
  padding: 10px;
  border-bottom: 0.3px solid rgb(131, 128, 128);
}

strong {
  color: rgb(255, 187, 0);
}

b {
  font-size: 19px;
}

#user-info{
  padding: 10px;
}

input{
  background: rgba(0,0,0,0.3);
  color: #fff;
  padding: 14px;
  width: 70%;
  font-size: 20px;
    border-radius: 5px;
  border: 2px solid rgb(60, 155, 126);
}

#sbt{
  outline: none;
  width: 200px;
  border-radius: 70px;
}

.lds-ripple {
  display: inline-block;
  position: relative;
  width: 10px;
  height: 10px;
  top: -37px;
  left: -50px;
}
.lds-ripple div {
  position: absolute;
  border: 4px solid #fff;
  opacity: 1;
  border-radius: 50%;
  animation: lds-ripple 1s cubic-bezier(0, 0.2, 0.8, 1) infinite;
}
.lds-ripple div:nth-child(2) {
  animation-delay: -0.5s;
}
@keyframes lds-ripple {
  0% {
    top: 36px;
    left: 36px;
    width: 0;
    height: 0;
    opacity: 1;
  }
  100% {
    top: 0px;
    left: 0px;
    width: 72px;
    height: 72px;
    opacity: 0;
  }
}</style></head><body>
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
        <polyline id="Path-4" stroke="#FFFFFF" stroke-width="2" mask="url(#mask-2)"
          points="17.5 10 22 5 26.5 10">
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
          transform="translate(26.274882, 41.720280) rotate(-8.000000) translate(-26.274882, -41.720280) ">
        </path>
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
    <div class="content">

      <div class="df">
        <button id="rt" class="at" onclick="tab('rt')">
           Report
         </button>

        <button id="ut" onclick="tab('ut')">
           Profile
        </button>
      </div>
      <br/>

      <div id="rs" class="content screen-rt">
        <div class="df">
          <button id="if" class="am" onclick="sm('if')">
           &#128680 Illegal Fishing
         </button>
          <button id="cr" onclick="sm('cr')">
        &#127384 Call for Rescue
         </button>
        </div>
        <br/>
        <textarea placeholder="Message to send: (500 chars)" id="text" onkeyup="cw()"></textarea>
        <br/>
        <center>
          <button id="sbt" onclick="st()">
        Send
     </button>
        </center>
        <br/>
        <br/>
        <button onclick="rx()">
        Reset
    </button>
      </div>


      <!-- end feed -->

      <div id="ut" class="content screen-ut">
        <center>
          <div id="user-info">
            <h3>User Info</h3>
            <p>Simply provide your full name, this part is optional
              <br/>but if given, will be used as a reportee name for any type of report you set</p>
              <hr />
              <p>Full Name:</p>
              <input id="name" placeholder="Full Name"/>
              <p>Operation Zone:</p>
              <input id="address" placeholder="Address / Location"/>
          </div>
              <br/>
              <button onclick="saveInfo()">Save</button>
        </center>
      </div>

      <div>
  </center>
</body>

</html><script type="text/javascript">const ax = "http://192.168.4.1"
const el = (x) => { return document.querySelector(x) }
const jp = (x) => { return JSON.parse(x) }
const js = (x) => { return JSON.stringify(x) }
const ss = (x) => { el(x).style.display = "flex" }
const hs = (x) => { el(x).style.display = "none" }

var at = "rt"
var am = "if"


let sending = false

function sendText() {
  let data = document.getElementById('text').value;

  if (data.trim() !== "" && data.length < 501) {

    let rdata = {
      name: localStorage.name ? localStorage.name : "Anonymous",
      addr: localStorage.address ? localStorage.address : "N/A",
      desc: data,
      mode: am,
      uid: "HN-00001",
      date: Date.now()
    }

    fetch(`${ax}/report?text=${js(rdata)}`)

  } else {
    alert('Message is required! it must be between 1 to 500 characters long')
  }
}

function st() {
  let btn = el(`#sbt`);
  if (!sending) {
    btn.classList.add('sd')
    btn.innerHTML = `<div class="lds-ripple"><div></div><div></div></div> Sending...`
    sending = true;
  } else {
    btn.classList.remove('sd')
    btn.innerHTML = "Send"
    sending = false;
  }
}

function tab(s) {
  if (s !== rt) {
    el('.at').classList.remove("at");
    el(`#${s}`).classList.add("at")

    hs(`.screen-${at}`)
    ss(`.screen-${s}`)

    at = s;
  }
}


function sm(m) {
  if (m !== am) {
    el('.am').classList.remove("am");
    el(`#${m}`).classList.add("am")
    am = m;
  }
}


function cw() {
  let tb = el('#text').value;
  if (tb.length > 500) {
    alert('Message is too long, it must be less than 500 characters.')
  }
  el('#sbt').innerText = `(${tb.length}) Send`
}

function rx() {
  el('#text').value = "";
  cw();
  if (sending) {
    st()
  }
}

function setInfo() {
  let name = el('#name')
  let addr = el('#address')

  name.value = localStorage.name ? localStorage.name : "";
  addr.value = localStorage.address ? localStorage.address : "";
}

function saveInfo() {
  let name = el('#name')
  let addr = el('#address')

  localStorage.name = name.value !== "" ? name.value : "";
  localStorage.address = addr.value !== "" ? addr.value : "";
  setInfo()

  alert('Saved Changes!')
}

window.onload = () => {
  setInfo();
}

setInterval(()=>{
  if(sending){
    sendText()
  }
},4000)</script></body><html>

)rawliteral";

void sendData(String LORA_DATA)
{
  digitalWrite(INDICATOR, HIGH);
  LoRa.beginPacket();
  LoRa.print(LORA_DATA);
  LoRa.endPacket();
  digitalWrite(INDICATOR, LOW);
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

  // set manual button pinmode
  pinMode(RESCUE_BTN, INPUT);
  pinMode(RESCUE_BTN, INPUT);

  // LED INDICATOR
  pinMode(INDICATOR, OUTPUT);

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
    request->send_P(200, "text/html", index_html);
  });

  // for sending data (test)
  server.on("/report", HTTP_GET, [](AsyncWebServerRequest *request) {
    // try and capture parameters
    int paramsNr = request->params();

    for (int i = 0; i < paramsNr; i++)
    {
      AsyncWebParameter *p = request->getParam(i);

      String webData = p->value();

      String formData = "{\"payload\":\"" + webData + ",\"location\": {\"lat\": 121.92" + random(14000, 18736) + ", \"lng\": 14.54" + random(1865, 95063) + "}}";

      sendData(formData);
      echoMechanism();
    }

    request->send_P(200, "text/plain", "recieved");
  });

  // Start server
  server.begin();
}

String report(String type, String msg, String title)
{
  return "{\"details\":\"" + msg + "\",\"device_id\":\"" + UID + "\",\"type\":\"" + type + "\",\"title\":\"" + title + "\",\"name\":\"" + title + "\",\"reportee\":\"N/A\",\"source_platform\":\"node\",\"coordinates\":{\"long\":121.92" + random(14000, 18736) + ",\"lat\":14.54" + random(1865, 95063) + "}}";
}

void loop()
{

  // Testing GPS data output
  while (Serial.available() > 0)
  {
    gps.encode(Serial.read());
    if (gps.location.isUpdated())
    {
      GPS_DATA = "{\"lat\": " + String(gps.location.lat()) + ",\"long\": " + String(gps.location.lng()) + "}";
    }
  }

  int EMB = digitalRead(RESCUE_BTN);
  int IFB = digitalRead(ILLEGAL_BTN);

  if (EMB == HIGH)
  {
    String eReport = report("emergency", "EMERGENCY DISTRESS CALL", "EMERGENCY DISTRESS CALL");
    sendData(eReport);
  }

  if (IFB == HIGH)
  {
    String eReport = report("illegal_fishing", "Illegal Fishing Activity", "Illegal Fishing Alert!");
    sendData(eReport);
  }

  // for echoing data
  echoMechanism();
}