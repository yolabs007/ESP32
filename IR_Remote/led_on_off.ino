/*
This code is written by Yolabs 
This code will  help you to know the  button and its code and will help you make inbuilt LED on or off for a sec

To control a motor in both directions using an ESP32 and IR remote control, you need to modify the code  
Hardware:
ESP32 Development Board -dev kit v1 
IR Receiver Module & remote control - provided as a set 
Motor Driver Module: e.g., L298N or L293D
DC Motor
Wiring: Jumper Wire 

REciver to ESP32 Connection 

VCC or + -> 3.3V of ESP32
GND or - -> GND of ESP32
OUT or S -> Any GPIO (for this code it is  22)

*/


#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECEIVE_PIN 22
#define LED_PIN 2

// Sample IR code. Please replace with the one you captured.
#define BUTTON_PRESS_CODE1 0xE318261B
#define BUTTON_PRESS_CODE2 0xFFA25D



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
        
        if (results.value == BUTTON_PRESS_CODE1 || results.value == BUTTON_PRESS_CODE2) {
            Serial.println("Matched! Toggling LED.");
            digitalWrite(LED_PIN, HIGH);
            delay(1000);
            digitalWrite(LED_PIN, LOW);
        } else {
            Serial.println("Received code did not match expected code.");
        }
        irrecv.resume();
    }
    delay(100); // Small delay to avoid flooding the Serial monitor
}
