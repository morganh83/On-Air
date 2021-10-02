/* Written by Morgan Habecker
  On-Air Sign by Adafruit
  https://www.thingiverse.com/thing:1506862
  Install on Adafruit Feather HUZZAH with ESP8266
  https://www.adafruit.com/product/2821
*/
#include <EspMQTTClient.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#define PIN 12
#define NUMPIXELS 50

// Network & MQTT Settings **EDIT**---------------

EspMQTTClient client(
  "YourWiFiSSID",
  "YourWifiPASSWORD",
  "mqtt.server.com",
  "AuthUsername",  // Omit this parameter to disable MQTT authentification
  "AuthPassword",  // Omit this parameter to disable MQTT authentification
  "onair-light"
);

// MORE MQTT SETTINGS
const char* mSub = "hm/onAir/cmd"; //Your SUBSCRIBE Topic
const char* mPub = "hm/onAir/response"; //Your PUBLISH Topic
//------------------------------------------------

//DO NOT CHANGE BELOW THIS LINE---------------------------------------

Adafruit_NeoPixel strip(50, PIN, NEO_GRB + NEO_KHZ800);
//Functions
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup()
{
  Serial.begin(115200); //Start logging for the serial monitor
  strip.begin(); //Start the neopixel strip
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
}

void onConnectionEstablished() 
{
  client.subscribe(mSub, [] (const String & payload) {
    if (payload == "onCall") {
      Serial.print("Received: onCall");
      colorWipe(strip.Color(255, 0, 0), 50);
      strip.show();
      client.publish(mPub, "onCallAck");
    }
    else if (payload == "busy") {
      Serial.print("Received: Yellow\n");
      colorWipe(strip.Color(255, 128, 0), 50);
      strip.show();
      client.publish(mPub, "busyAck");
    }
    else if (payload == "clear") {
      Serial.print("Received: Clear\n");
      colorWipe(strip.Color(0, 0, 0), 50);
      strip.show();
      client.publish(mPub, "clearAck");
    }    
  });
}

void loop() {
  client.loop();
}
