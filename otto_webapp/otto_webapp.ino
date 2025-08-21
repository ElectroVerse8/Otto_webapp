#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Otto.h>
Otto Otto;

#define LeftLeg 18 
#define RightLeg 14
#define LeftFoot 16 
#define RightFoot 13 
#define Buzzer  17 
#define Trigger 27
#define Echo 26 

// Wi-Fi AP creds
const char* ssid     = "OTTO_AP";
const char* password = "123456789";

// Web server on port 80
AsyncWebServer server(80);

// Command queue
String motionCommand = "";
volatile bool motionPending = false;

// ——— Your HTML UI ————————————————————————————————

const char* base64Logo =
  "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";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Otto Web Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <style>
    body { margin:0; background:#9aa2d6; font-family:sans-serif;
           display:flex; flex-direction:column; height:100vh; }
    #control-pad {
      position:relative; flex:1; display:grid;
      grid-template:1fr 1fr 1fr / 1fr 1fr 1fr; padding:5vw;
    }
    #control-pad::before {
      content:""; position:absolute; inset:5vw;
      border:8px solid #fff; border-radius:12px;
      pointer-events:none;
    }
    #control-pad button {
      background:transparent; border:none; cursor:pointer;
      display:flex; align-items:center; justify-content:center;
    }
    .arrow {
      width:0; height:0; border-style:solid;
      border-width:0 20px 35px 20px;
      border-color:transparent transparent #fff transparent;
    }
    #up{grid-area:1/2;}    #left{grid-area:2/1;transform:rotate(-90deg);}
    #down{grid-area:3/2;transform:rotate(180deg);} 
    #right{grid-area:2/3;transform:rotate(90deg);}
    #modes {
      display:flex; justify-content:space-around;
      padding:1rem 0.5rem; background:#7983c0;
    }
    #modes button {
      flex:1; margin:0 0.5rem; padding:0.75rem 0;
      font-size:1rem; background:#5f67a1; border:none;
      border-radius:8px; color:#fff; cursor:pointer;
    }
    #modes button:active { background:#4a4f80; }
  </style>
</head>
<body>

  <img src="data:image/png;base64,${base64Logo}" alt="Logo" style="display:block;margin:0 auto;max-width:100%;">

  <div id="control-pad">
    <button id="up"><div class="arrow"></div></button>
    <button id="left"><div class="arrow"></div></button>
    <button id="down"><div class="arrow"></div></button>
    <button id="right"><div class="arrow"></div></button>
  </div>
  <div id="modes">
    <button id="avoid">Avoid Mode</button>
    <button id="jump">Jump</button>
    <button id="flop">Flapping</button>
  </div>
  <script>
    function sendCmd(m) {
      fetch('/cmd?move=' + m).catch(e => console.error(e));
    }
    document.getElementById('up')   .addEventListener('click', ()=>sendCmd('forward'));
    document.getElementById('down') .addEventListener('click', ()=>sendCmd('backward'));
    document.getElementById('left') .addEventListener('click', ()=>sendCmd('left'));
    document.getElementById('right').addEventListener('click', ()=>sendCmd('right'));
    document.getElementById('avoid').addEventListener('click', ()=>sendCmd('avoid'));
    document.getElementById('jump') .addEventListener('click', ()=>sendCmd('jump'));
    document.getElementById('flop') .addEventListener('click', ()=>sendCmd('flapping'));
  </script>
</body>
</html>
)rawliteral";

// ——— HTTP handler just queues the command —————————————————
void handleCommand(AsyncWebServerRequest* request){
  motionCommand = request->arg("move");
  motionPending = true;
  request->send(200, "text/plain", "OK");
}

// === Motion routines (fill in your timings/angles) ===
void doForward() {
  Otto.walk(2,1000,1);
}
void doBackward() {
  Otto.walk(2,1000,-1);
}
void doLeft() {
  Otto.turn(2,1000,1);
}
void doRight() {
  Otto.turn(2,1000,-1);
}
void doAvoid() {
}
void doJump() {
  Otto.jump(1,500);
}
void doFlapping() {
  Otto.flapping(2, 1000, 20,1);
  Otto.flapping(2, 1000, 20,-1);
}

long ultrasound() {
   long duration, distance;
   digitalWrite(Trigger,LOW);
   delayMicroseconds(2);
   digitalWrite(Trigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(Trigger, LOW);
   duration = pulseIn(Echo, HIGH);
   distance = duration/58;
   return distance;
}

void setup(){
  Serial.begin(115200);

  // Start Wi-Fi AP + mDNS
  WiFi.softAP(ssid, password);
  MDNS.begin("otto");

  // Routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    String page = index_html;
    page.replace("${base64Logo}", base64Logo);
    req->send(200, "text/html", page);
  });
  server.on("/cmd", HTTP_GET, handleCommand);

  server.begin();
  Serial.println("ESP32 Otto Ready! Connect to OTTO_AP and open http://otto.local");

  //Otto setup
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  Otto.home();
}

void loop(){
  if(motionPending){
    Serial.println("Handeling...");
    motionPending = false;
    if      (motionCommand == "forward")  doForward();
    else if (motionCommand == "backward") doBackward();
    else if (motionCommand == "left")     doLeft();
    else if (motionCommand == "right")    doRight();
    else if (motionCommand == "avoid")    doAvoid();
    else if (motionCommand == "jump")     doJump();
    else if (motionCommand == "flapping") doFlapping();
    Serial.println("Handeled.");
    Otto.home();
  }
}
