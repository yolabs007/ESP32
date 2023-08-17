

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
