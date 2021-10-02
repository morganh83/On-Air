/* Written by Morgan Habecker
  On-Air Sign by Adafruit
  https://www.thingiverse.com/thing:1506862
  Install on Adafruit Feather HUZZAH with ESP8266
  https://www.adafruit.com/product/2821
*/

// Libraries
#include <EspMQTTClient.h>
#include <SPI.h>

// Network & MQTT Settings **EDIT**---------------
EspMQTTClient client(
  "YourWiFiSSID",
  "YourWiFiPASSWORD",
  "mqtt.serverName.com",
  "AuthUsername",  // Omit this parameter to disable MQTT authentification
  "AuthPASSWORD",  // Omit this parameter to disable MQTT authentification
  "onair-ctrl"
);

// MORE MQTT SETTINGS
const char* mSub = "hm/onAir/response"; //Your SUBSCRIBE Topic
const char* mPub = "hm/onAir/cmd"; //Your PUBLISH Topic
//--------------------------------------

//Switch & Button States
int switchState;
int lastSwitchState = LOW;
int buttonState;
int lastButtonState = LOW;
int previous = LOW;

// Pins
const int switch1 = 14; //Switch pin
const int button1 = 12; //Button pin
int redPin = 13;
int greenPin = 15;
int bluePin = 16;

// Time settings
long btime = 0;
unsigned long debounceDelay = 400; // Change if flickering

void setLEDColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
void LEDOff() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}
void yellowColor() {
  setLEDColor(255, 128, 0);
}
void redColor() {
  setLEDColor(255, 0, 0);
}

void setup() {
  //Setup the button and switch pins
  pinMode(button1, INPUT);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  LEDOff();
  
  //Setup the serial and WiFi interfaces
  Serial.begin(115200);
  client.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  client.subscribe(mSub, [] (const String & payload) {
    if (payload == "onCallAck") {
      Serial.print("Received: Red");
      redColor();
    }
    else if (payload == "busyAck") {
      Serial.print("Received: Yellow\n");
      yellowColor();
    }
    else if (payload == "clearAck") {
      Serial.print("Received: Clear\n");
      LEDOff();
    }    
  });
}

void loop() {
  client.loop();
  switchState = digitalRead(switch1);
  int reading = digitalRead(button1);
  
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      Serial.print("Switch is ON\n");
      client.publish(mPub, "busy");
    }
    else {
      Serial.print("Switch is OFF\n");
      client.publish(mPub, "clear");
      //delay(200);
      Serial.print("clear Signal Sent\n");
    }
    lastSwitchState = switchState;
  }
  
  if (reading == HIGH && previous == LOW && millis() - btime > debounceDelay) {
    if (buttonState == HIGH) {
      buttonState = LOW;
      Serial.print("Button toggled: OFF\n");
      client.publish(mPub, "clear");
      Serial.print("clear Signal Sent\n");
    }
    else {
      buttonState = HIGH;
      Serial.print("Button toggled: ON\n");
      client.publish(mPub, "onCall");
      Serial.print("onCall Signal Sent\n");
    }  
    btime = millis();
  }
  previous = reading;
}
