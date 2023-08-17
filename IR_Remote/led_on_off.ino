/*

This code is written by Rahul Sharma for Yolabs 
This code will  help you to know the  button and its code and will help you make inbuilt LED on or off for a sec as well as test blocks to check your  HEXcode from remote

To control a motor in both directions using an ESP32 and IR remote control, you will need to modify the code or take the code - Remote_Motor Control.ino  
Hardware:
ESP32 Development Board -dev kit v1 
IR Receiver Module & remote control - provided as a set 
Motor Driver Module: e.g., L298N or L293D
DC Motor
Wiring: Jumper Wire 

REciver to ESP32 Connection 

VCC or + -> 3.3V of ESP32
GND or - -> GND of ESP32
OUT or S -> Any GPIO (for this code it is  D15)

*/


#include <IRremoteESP8266.h>  // if not installed please install this library it works for ESP32 also
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECEIVE_PIN 15
#define LED_PIN 2

// Sample IR codes . Please replace with the one you captured. To know your code use serial monitor.

#define BUTTON1_PRESS_CODE1 0xE318261B
#define BUTTON1_PRESS_CODE2 0xFFA25D
#define BUTTON_PRESS_UP 0x3D9AE3F7
#define BUTTON_PRESS_STOP1 0x488F3CBB
#define BUTTON_PRESS_STOP2 0xFF38C7
#define BUTTON_PRESS_DOWN 0x1BCO152B





IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    irrecv.enableIRIn();
    Serial.println("Ready to receive IR commands...");
}

void loop() {
    if (irrecv.decode(&results)) {
        Serial.print("Received IR Code: 0x");
        Serial.println(results.value, HEX); // This will print the received IR code in HEX format
        
        if (results.value == BUTTON1_PRESS_CODE1 || results.value == BUTTON1_PRESS_CODE2) {
            Serial.println("Matched! Toggling LED.");
            digitalWrite(LED_PIN, HIGH);
            delay(1000);
            digitalWrite(LED_PIN, LOW);
        }
        
        else if (results.value == BUTTON_PRESS_UP) {

          liftup();
        }

        else if (results.value == BUTTON_PRESS_STOP1 || results.value == BUTTON_PRESS_STOP2) {

          liftstop();
        }


        
        
        else {
            Serial.println("Received code did not match expected code.");
        }
        irrecv.resume();
    }
    delay(100); // Small delay to avoid flooding the Serial monitor
}

//Create your own functions or modify them as per your project requirements

void liftup(){
  Serial.println("LIFT IS GOING UP.");
}

void liftstop(){
  Serial.println("LIFT IS STOPED.");
}

void liftdown(){
  Serial.println("LIFT IS GOING DOWN.");
}

