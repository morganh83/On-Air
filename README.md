# On-Air

This project is for those who work from home. It's an on-air sign that allows two modes: "Busy, but can be interrupted" and "In a meeting, be quiet and leave me alone"

The sign consists of two major components in a client/server model.

Server (Sign):
It is built from two Adafruit Huzzah boards with the ESP8266 modules (because they were lying around, that's why).
https://www.adafruit.com/product/2821

The sign also uses a NeoPixel strip:
https://www.adafruit.com/product/1507

Client (control):
Using one of the Huzzah boards and whatever switches you have lying around. In my case, I had an old Saint's Row IV sound switch that I repurposed for this. Any two buttons or switches will do.
