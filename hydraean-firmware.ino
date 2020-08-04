// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

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
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Seantinel</h2>
  <p>
    <span class="dht-labels">Temperature</span>
    <span id="temperature"> -- </span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity"> -- </span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
 fetch('http://192.168.4.1/temperature')
  .then(response => response.json())
  .then(data =>{
    document.getElementById("temperature").innerHTML = data
    })
}, 2000 ) ;

setInterval(function ( ) {
 fetch('http://192.168.4.1/humidity')
  .then(response => response.json())
  .then(data =>{
    document.getElementById("humidity").innerHTML = data
    })
}, 2000 ) ;
</script>
</html>)rawliteral";

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


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

  // Start server
  server.begin();
}

void loop()
{
}