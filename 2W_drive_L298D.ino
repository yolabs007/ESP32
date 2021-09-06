/*
  Vehicle Drive  -  by Rahul Sharma for Yolabs
  https://www.yolabs.in 

  This code is to move the vehicle in front, back, left and right 

  This code deploys four pins D2,D4,D5 and D12. 

  

  https://www.yolabs.in
*/


 int pinLf = 2; //  For left motor to turn forward 
 int pinLb = 4; //  for left motor to turn backward 
 int pinRf = 5; // for rigtt motor to turn forward 
 int pinRb = 12; // for right motor to turn backward
 
// the setup function runs once when you press reset or power the board

void setup() {
  pinMode(pinLf, OUTPUT);
  pinMode(pinLb, OUTPUT);
  pinMode(pinRf, OUTPUT);
  pinMode(pinRb, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  forward() ;      // Move forward 
  delay(10000);   // wait for given seconds
  backward();    // move backward
  delay(10000);  // wait for given seconds

}
void forward() {
  digitalWrite(pinLf, HIGH);
  digitalWrite(pinLb, LOW);
  digitalWrite(pinRf, HIGH);
  digitalWrite(pinRb, LOW);
  
}

void backward() {
  digitalWrite(pinLf, LOW);
  digitalWrite(pinLb, HIGH);
  digitalWrite(pinRf, LOW);
  digitalWrite(pinRb, HIGH);
  
}
