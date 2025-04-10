#include "BluetoothSerial.h"

// Create a BluetoothSerial object
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  // Initialize Bluetooth with a device name
  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth SPP device started. Now you can pair it with your computer or mobile device.");
}

void loop() {
  // Read from Serial Monitor and send to Bluetooth, if available.
  if (Serial.available()) {
    char c = Serial.read();
    SerialBT.write(c);
  }
  
  // Read from Bluetooth and print to Serial Monitor, if available.
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c);
  }
  
  delay(20);
}
