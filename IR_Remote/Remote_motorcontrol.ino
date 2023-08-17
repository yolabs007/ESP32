/*

This code is written by Rahul Sharma for Yolabs 
This code will  help you to use up down and stop button to control your lift via dc motor and motor driver. 
Hardware:
ESP32 Development Board -dev kit v1 
IR Receiver Module & remote control - provided as a set 
Motor Driver Module: e.g., L298N or L293D
DC Motor
Wiring: Jumper Wire 

IR Reciver to ESP32 Connection 

VCC or + -> 3.3V of ESP32
GND or - -> GND of ESP32
OUT or S -> Any GPIO (for this code it is  D15)


Motor Driver to ESP32 Connection

Ground  --> GND of ESP32
Input 1  --> D12
Input 2  --> D13  

if you want the effect to be opposite please swap D12 and D13 pins

*/


#include <IRremoteESP8266.h>  // if not installed please install this library it works for ESP32 also
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECEIVE_PIN 15
#define LED_PIN 2
#define M1_clockwise 12
#define M1_anticlockwise 13



// Sample IR codes . Please replace with the one you captured. To know your code use serial monitor.

#define BUTTON1_PRESS_CODE1 0xE318261B
#define BUTTON1_PRESS_CODE2 0xFFA25D

// Buttons for up/down and stop lift 
#define BUTTON_PRESS_UP 0x3D9AE3F7
#define BUTTON_PRESS_STOP1 0x488F3CBB
#define BUTTON_PRESS_STOP2 0xFF38C7
#define BUTTON_PRESS_DOWN1 0x1BC0157B
#define BUTTON_PRESS_DOWN2 0xFF4AB5



IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(M1_clockwise, OUTPUT);
    pinMode(M1_anticlockwise, OUTPUT);
    
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

        else if (results.value == BUTTON_PRESS_DOWN1 || results.value == BUTTON_PRESS_DOWN2) {

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
   digitalWrite(M1_clockwise, HIGH);
   digitalWrite(M1_anticlockwise,LOW);
   
}

void liftstop(){
    digitalWrite(M1_clockwise, LOW);
    digitalWrite(M1_anticlockwise,LOW);
}

void liftdown(){
   digitalWrite(M1_clockwise, LOW);
   digitalWrite(M1_anticlockwise,HIGH);
}

