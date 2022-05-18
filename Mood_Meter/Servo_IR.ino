
/* Move the Servo With the Help of IR sensor
  Rotate a servo  from Zero to 180 based on IR sensor

  This code is combination of IR sensor code and Servo Code

IR Circuit -  ESP32 has only one 3v3 pin. Hence we use pin-4 High for VCC 

 IR Connections 
  Out  -            Pin13 (D13)
  Gnd -             Gnd
  Vcc   -           pin4 (D4)


 
  SERVO Connections 
  Servo  -            ESP32 
  Brown -             Ground
  Red   -             3v3
  Orange/Yellow  -    Digital pin - D26 in this code 
  
  NOTE : If your servo need to take a good load or if you are connecting multiple servo,  
  you will  need external power supply with/without servo controller board               
  
  by Rahul Sharma for Yolabs - 18th may 2022
  This example code is in the public domain.
  https://www.yolabs.in
*/













#include <Servo.h>

Servo servo_2;   // give any name

void setup() {
  // put your setup code here, to run once:
  
  pinMode(2,OUTPUT); // inbuid LED pin- to confirm sensor output
  pinMode(4,OUTPUT); //  to power IR sensor - work as VCC
  pinMode(13,INPUT); // take IR Sensor INPUT

  servo_2.attach(26);
  digitalWrite(4,1);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(13)==1){
    servo_2.write(180);
    digitalWrite(2,1);
  }

  
  if (digitalRead(13)==0){
    servo_2.write(0);
    digitalWrite(2,0);
  }

}
