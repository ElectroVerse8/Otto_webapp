#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <Servo.h>

const char* ssid = "OTTO_AP";
const char* password = "password";

AsyncWebServer server(80);

Servo servo1;
Servo servo2;

const int servo1Pin = D1;
const int servo2Pin = D2;


void handleCommand(AsyncWebServerRequest* request){
  String move = request->arg("move");
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

  LittleFS.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  if (MDNS.begin("otto")) {
    Serial.println("mDNS responder started: http://otto.local");
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.on("/cmd", HTTP_GET, handleCommand);
  server.begin();
}

void loop(){
  MDNS.update();
}
