#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

const char* ssid = "OTTO_AP";
const char* password = "password";

AsyncWebServer server(80);

Servo servo1;
Servo servo2;

const int servo1Pin = D1;
const int servo2Pin = D2;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang=\"en\">
<head>
  <meta charset=\"UTF-8\">
  <title>Otto Web Control</title>
  <style>
    body {
      background-color: #2196F3;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      font-family: Arial, sans-serif;
    }
    #controls {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      grid-template-rows: 1fr 1fr 1fr;
      gap: 10px;
      width: 240px;
    }
    button {
      width: 80px;
      height: 80px;
      font-size: 40px;
    }
    #forward { grid-column: 2; grid-row: 1; }
    #left    { grid-column: 1; grid-row: 2; }
    #right   { grid-column: 3; grid-row: 2; }
    #backward{ grid-column: 2; grid-row: 3; }
  </style>
  <script>
    function sendCmd(cmd){
      fetch('/cmd?move=' + cmd);
    }
  </script>
</head>
<body>
  <div id=\"controls\">
    <button id=\"forward\" onclick=\"sendCmd('forward')\">&#9650;</button>
    <button id=\"left\" onclick=\"sendCmd('left')\">&#9664;</button>
    <button id=\"right\" onclick=\"sendCmd('right')\">&#9654;</button>
    <button id=\"backward\" onclick=\"sendCmd('backward')\">&#9660;</button>
  </div>
</body>
</html>
)rawliteral";


void handleCommand(AsyncWebServerRequest* request){
  String move = request->arg("move");
  Serial.println(move);  // log incoming command for debugging
  if (move == "forward") {
    // TODO: implement forward motion sequence
  } else if (move == "backward") {
    // TODO: implement backward motion sequence
  } else if (move == "left") {
    // TODO: implement left turn sequence
  } else if (move == "right") {
    // TODO: implement right turn sequence
  } else if (move == "wave") {
    // TODO: implement wave preset
  } else if (move == "dance") {
    // TODO: implement dance preset
  }
  request->send(200, "text/plain", "OK");
}

void setup(){
  Serial.begin(115200);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);


  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  if (MDNS.begin("otto")) {
    Serial.println("mDNS responder started: http://otto.local");
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/cmd", HTTP_GET, handleCommand);
  server.begin();
}

void loop(){
  MDNS.update();
}
