/*
  Car Control  with Bluetooth   -  by Rahul Sharma for Yolabs 
Control your car direction and speed by using an android app 
  
This code helps to control your ESP32 powered car using Bluetooth classic.

You can connect to any bluetooth app - and send f -forward b-backward l- left and r- right and number 0-255 for speed. 

You can use the Serial Bluetooth app from Playstore for quick check or can use Yolabs super cool app for more fun and control


 
  https://www.yolabs.in
*/

#include "BluetoothSerial.h"   // add the library to access bluetooth 

BluetoothSerial SerialBT;  //give a short name to bluetooth serial 

String inString = "";    // string to hold speed input

String outString = ""; //string to hold other button inputs 

int Speed = 0;

String Direction = "";

// pin setup  for car control 

int pinLf = 2; //  For left motor to turn forward 
int pinLb = 4; //  for left motor to turn backward 
int pinRf = 5; // for rigtt motor to turn forward 
int pinRb = 12; // for right motor to turn backward


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32yolabs"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  //  Attach pins 

 ledcAttachPin(pinLf,0);
 ledcAttachPin(pinLb,1);
 ledcAttachPin(pinRf,2);
 ledcAttachPin(pinRb,3);

 // setup all the channlel

 ledcSetup(0,5000,8); // 0 channel, 5000 freq, 8 byte resolution
 ledcSetup(1,5000,8);
 ledcSetup(2,5000,8);
 ledcSetup(3,5000,8);
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());

  }
  if (SerialBT.available()) {
    int inChar = SerialBT.read();
     if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }

    if (isAlpha(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      outString += (char)inChar;
    }

    
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Value:");
      Serial.println(inString.toInt());
      Serial.print("String: ");
      Serial.println(inString);
      Serial.print("outString: ");
      Serial.println(outString);
      
      if (inString.length() > 0) {
        Speed = inString.toInt(); 
        Serial.print("Speed: ");
        Serial.println(Speed); 
      }
      if (outString.length() > 0) {
        Direction = outString;
         Serial.print("Direction: ");
        Serial.println(Direction);
        
      }
      // clear the string for new input:
      inString = "";
      outString = "";
      
    }
  }
  if (Direction == "f"){
          forward();  
        }
  if (Direction == "b"){
          backward();  
        }
  if (Direction == "l"){
          left();  
        }
  if (Direction == "r"){
          right();  
        }
  //Serial.println(Speed); 
  //Serial.println(Direction);      
    
}

void forward() {
  ledcWrite(0, Speed);
  ledcWrite(1, 0);
  ledcWrite(2, Speed);
  ledcWrite(3, 0);
  
}

void backward() {
  ledcWrite(0, 0);
  ledcWrite(1, Speed);
  ledcWrite(2, 0);
  ledcWrite(3, Speed);
  
}

void left() {
  ledcWrite(0, 0);
  ledcWrite(1, Speed);
  ledcWrite(2, Speed);
  ledcWrite(3, 0);
  
}
void right() {
  ledcWrite(0, Speed);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, Speed);
  
}
