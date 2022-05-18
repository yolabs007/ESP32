/* Use IR sensor to Control the LED
 
  Contrl ESP-32 in built LED (pin-2) Using a simple IR sensor
  Connections 
  Out  -            Pin13 
  Gnd -             Gnd
  Vcc   -           3v3
  O
  
  NOTE : IR sensors comes in two types - Default High and Default Low

  Default High - If done Nothing(no Hand) out pin will be high
  Default Low - if dome Nothing (no Hand) out pin will b e low
  
  Created by  Rahul Sharma for Yolabs 
  This example code is in the public domain.
  https://www.yolabs.in
*/
 



void setup() {
  // put your setup code here, to run once:

  pinMode(2,OUTPUT);  // inbuild LED on pin-2
  pinMode(13,INPUT);   // pin connected to IR sensor

}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(13)==0){
    digitalWrite(2,1);
   
  }
if (digitalRead(13)==1){
    digitalWrite(2,0);
   
  }
}
