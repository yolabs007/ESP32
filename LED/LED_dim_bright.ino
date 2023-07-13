/* Brighten and Dim the LED
  dim  the Inbuild LED pin - 2 - dim- brighter- dim 
  No Connections Required - Inbuilt Pin(no-2) - change the pin numbers if you want to connect some external LEDs

  
  by Rahul Sharma for Yolabs 
  This example code is in the public domain.
  https://www.yolabs.in
*/

// the number of the LED pin
const int ledPin = 2;  // to brighten and dim inbuilt LED; 


// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

}
 
void loop(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);   
    delay(15);
  }
}
