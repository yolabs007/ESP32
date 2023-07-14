/* Control Multiple Servos using  a Mobile App- Android app 
  Control Servos from Zero to 180. This code will require the following hardware 
  1. PCA - 9685 Card 
  2. ESP32 - DEvkit V1
  3. Servos 
  4. Power Supply (5-6 V to PCA Card)
  5. DC-DC buck Convertor -  if power supply is with higer voltage 

  NOTE: you must reset your ESP-32 (EN button) aqfter giving connection and powering all. 
  PCA card will loose comunication if power it off 
  and will requires ESP32 to reset (use En button)
 PCA to ESP32 Coonection 
  Connections 

  PCA 9685         ESP32  
  GND -            GND 
  SCL -            D22
  SDA   -          D21   
  
v+ and VCC short them 

Servo's need to be connected to channel no 1-4 
              
  by Rahul Sharma for Yolabs 
  This example code is in the public domain.
  https://www.yolabs.in
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  525// this is the 'maximum' pulse length count (out of 4096)

// To change the movement of the servo change the angle and not servomax or minimum

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32Test123"); 
  
  board1.begin();
  board1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
}


void loop() {

  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');
    int servoNum = received.substring(0, received.indexOf(':')).toInt();
    int pos = received.substring(received.indexOf(':') + 1).toInt();
    
    if(servoNum == 1) {
      board1.setPWM(0, 0, angleToPulse(pos) );
    } else if(servoNum == 2) {
       board1.setPWM(1, 0, angleToPulse(pos) );
    }else if(servoNum == 3) {
       board1.setPWM(2, 0, angleToPulse(pos) );
    }else if(servoNum == 4) {
       board1.setPWM(3, 0, angleToPulse(pos) );
    }
  
  delay(10); // for smoother operations 
 
}
}

// angle to pluse mapping 

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
