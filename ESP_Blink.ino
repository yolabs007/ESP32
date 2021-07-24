/*
  Blink  -  by Rahul Sharma for Yolabs 

  Turns an LED on for one second, then off for one second, repeatedly.

  ESP 32 is having an inbuilt LED @ pin 2 
  Note - Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6 and can be accessed by typing LED_BUILTIN directly .
  
  However for ESP 32 it need to be set to pin-2, this code makes it 
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your board you need to go through the document

  This example code is in the public domain and you are free to use or modify it.

  https://www.yolabs.in
*/


 int LED_BUILTIN = 2; // declare the pin, please ensure pin numbedr matches to your inbuilt LED pin number 
 
// the setup function runs once when you press reset or power the board

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
