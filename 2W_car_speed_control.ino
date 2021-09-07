/*
  Vehicle Drive  -  by Rahul Sharma for Yolabs
  https://www.yolabs.in 

  This code is to move the vehicle in front, back, left and right

   This code also helps in controling the speed 

  This code deploys four pins D2,D4,D5 and D12
   

  

  https://www.yolabs.in
*/


 int pinLf = 2; //  For left motor to turn forward 
 int pinLb = 4; //  for left motor to turn backward 
 int pinRf = 5; // for rigtt motor to turn forward 
 int pinRb = 12; // for right motor to turn backward

 int Speed = 150;
 
// the setup function runs once when you press reset or power the board

void setup() {
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
// the loop function runs over and over again forever
void loop() {
  forward() ;      // Move forward 
  delay(10000);   // wait for given seconds
  backward();    // move backward
  delay(10000);  // wait for given seconds
  left();    // move backward
  delay(10000);  // wait for given seconds
  right();    // move backward
  delay(10000);  // wait for given seconds

}
void forward() {
  ledcWrite(0, Speed);
  ledcWrite(1, 0);
  ledcWrite(pinRf, Speed);
  ledcWrite(pinRb, 0);
  
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
