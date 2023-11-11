/*
  Blink with Bluetooth   -  by Rahul Sharma for Yolabs 

  Turns inbuilt or external  LED on  and off using an android device 
  
  ESP 32 is having an inbuilt LED @ pin 2  as well as bluetooth classis (old stype of bluetooth, BLE(bluetooth low energy is new )

  Please go to the simple  bluetooth setup wizard to check your bluetooth is work 

  file-example-example for esp32 dev module - Serial to Serial BT - upload this code in your ESP32

 standard android app - Serial Bluetooth Terminal -  install in your phone 

 check in serial monitor that  you can comunicate two way 
 
  https://www.yolabs.in
*/

#include "BluetoothSerial.h"   // add the library to access bluetooth 

BluetoothSerial SerialBT;  //give a short name to bluetooth serial 
int LED_BUILTIN = 2;       // the pin you woud loke to control 


char receivedChar;      // store the char you received in this variable 

const char turnON ='a';   // decide the char for on 
const char turnOFF ='b';   // decide charcter for off 


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32yolabs"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED_BUILTIN1,OUTPUT);    // decalre the pin as output pin
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());

  }
  if (SerialBT.available()) {
    receivedChar = (char)SerialBT.read();
    Serial.println(receivedChar);
    //Serial.write(SerialBT.read());
    
    if (receivedChar==turnON){
      Serial.println("i got one switch on the light");
      digitalWrite(LED_BUILTIN1,HIGH);
    }
    if (receivedChar==turnOFF){
      Serial.println("i got zero switch off the light");
      digitalWrite(LED_BUILTIN1,LOW);
    }
    
  }
  delay(20);
}
