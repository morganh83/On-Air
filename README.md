# On-Air

This project is for those who work from home. It's an on-air sign that allows two modes: "Busy, but can be interrupted" and "In a meeting, be quiet and leave me alone"

The sign consists of two major components in an IoT pub/sub model.

Sign:
It is built from two Adafruit Huzzah boards with the ESP8266 modules (because they were lying around, that's why).
https://www.adafruit.com/product/2821

The sign also uses a NeoPixel strip:
https://www.adafruit.com/product/1507

Controller:
Using one of the Huzzah boards and whatever switches you have lying around. In my case, I had an old Saint's Row IV sound switch that I repurposed for this. Any two buttons or switches will do.

You will also need access to an MQTT broker (or start your own with EMQx, Mosquitto, etc).

Both the sign and controller publish and listen to two different topics (in my case, RandomTopic/cmd and RandomTopic/response)
the "cmd" topic is only for the controller to send the various commands. The "response" topic is used as an acknowledgement for an LED to give you the status of the sign.

_____________________________________
|            |  PUB     |  SUB      |
| controller |  cmd     |  response |
| sign       | response |  cmd      |
-------------------------------------
