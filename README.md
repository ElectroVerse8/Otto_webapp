# Otto Web App

This project provides a simple web application for controlling an ESP8266 based
robot named **Otto**.  The ESP8266 acts as a Wi‑Fi access point and serves a
web page with buttons to send motion commands such as forward, backward, left,
right and a couple of preset moves.  The interface is now built directly into
the sketch so the project can be compiled and uploaded without any extra data
files.

The sketch has been kept intentionally small so that it can be used as a
starting point for your own robot projects.

## Required Arduino Libraries

- **ESP8266WiFi**
- **ESP8266mDNS**
- **ESPAsyncWebServer**
- **ESPAsyncTCP**
- **Servo** (built in)

## Installing with the Arduino IDE

1. Install the ESP8266 board package through the *Boards Manager*.
2. Install the libraries listed above using the *Library Manager* or by copying
   them into your Arduino `libraries` folder.  Any custom libraries included in
   this repository can be found in the `libraries/` directory.
3. Copy the entire project folder (`Otto_webapp`) into your Arduino
   sketchbook.
4. Open `src/otto_webapp.ino` in the Arduino IDE.
5. Select your ESP8266 board and click **Upload** to flash the sketch.
6. After boot the board creates a Wi‑Fi network named `OTTO_AP` with password
   `password`.
7. Connect to this network with your phone and open
   [`http://otto.local/`](http://otto.local/) in a browser. The mDNS responder
   announces the device as `otto.local` so no IP address is required.

Actual servo sequences for each command are left as TODO items in the sketch and
should be adapted to match your hardware.
