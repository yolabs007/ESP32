/* Brighten and Dim the LED using a mobile slider 
  dim  the Inbuild LED pin - 2 - dim- brighter- dim 
  No Connections Required - Inbuilt Pin(no-2) - change the pin numbers if you want to connect some external LEDs

You will need a phone and basic knowledge of MIT app inventor to make this wotrk 
  
  by Rahul Sharma for Yolabs 
  This example code is in the public domain.
  https://www.yolabs.in
*/
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
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

  // configure bluetooth and serial 
  Serial.begin(115200);
  SerialBT.begin("ESP32Test"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

}
 
void loop(){
  // increase the LED brightness
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n'); // this ensure that you process one number at a time
    int dutyCycle = received.toInt();
    ledcWrite(ledChannel, dutyCycle);
    delay(15); }
 
}

