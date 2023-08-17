/*

This code is written by Rahul Sharma for Yolabs 
This code will  help you to use up, down and stop button to control your lift via dc motor and motor driver. 
Hardware:
ESP32 Development Board -dev kit v1 
Mobile Phone - Android preffered 
Motor Driver Module: e.g., L298N or L293D
DC Motor
Wiring: Jumper Wire 


Motor Driver to ESP32 Connection

Ground  --> GND of ESP32
Input 1  --> D12
Input 2  --> D13  

if you want the effect to be opposite please swap D12 and D13 pins

*/

#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
#define M1_clockwise 12
#define M1_anticlockwise 13


void setup() {
  pinMode(2,OUTPUT); 
  pinMode(M1_clockwise,OUTPUT); 
  pinMode( M1_anticlockwise,OUTPUT); 
  SerialBT.begin("ESP32Test"); // Bluetooth device name
}

void loop() {
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');  // very imp otherwise you will find the servo is legggy and received string arbitery 
    if (received == "up"){
      liftup();
    }
    else if (received == "down"){ 

      liftdown();

    }

    else if (received == "stop"){ 

      liftstop();
    }
   
  }
}

void liftup(){
   digitalWrite(2,HIGH);
   digitalWrite(M1_clockwise, HIGH);
   digitalWrite(M1_anticlockwise,LOW);
   
}

void liftstop(){
    digitalWrite(2,LOW);
    digitalWrite(M1_clockwise, LOW);
    digitalWrite(M1_anticlockwise,LOW);
}

void liftdown(){
   digitalWrite(2,HIGH);
   digitalWrite(M1_clockwise, LOW);
   digitalWrite(M1_anticlockwise,HIGH);
}
