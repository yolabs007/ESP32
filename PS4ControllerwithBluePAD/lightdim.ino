/** Note: For Bluepad based code you need to have New board manager 
called ESP32+Bluepad   for more info look here - https://bluepad32.readthedocs.io/en/latest/ 
Quick Steps 
1. Install the board manager -  esp32_bluepad
2. select the right board maanger from tools --> its not arduino , not esp32, its esp32_bluepad 
3. Now put this code in ESP32. 
4. power on your controller and press share and playstation button(P) together. 
5. controller will give quick flash white lite and after connected a blue light (no blinking)
6. now left stick will control your vehicle.
7. Note : this is written for  esp32 where pin-2 is connected to inbuilt led

This code is written by - Rahul Sharma for Yolabs - 24th Mar'25 

**/


#include <Bluepad32.h>

#define LED_PIN 2  // Output pin for the LED

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but no empty slot found");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }
  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

// Process gamepad input and set LED brightness on LED_PIN
void processGamepad(ControllerPtr ctl) {
  // Use the left stick's X axis for this test.
  // The axis value typically ranges from -511 to 512.
  int rawValue = ctl->axisX();
  // Map the raw value: negative values -> lower brightness, positive values -> higher brightness.
  int brightness = map(rawValue, -511, 512, 0, 255);
  brightness = constrain(brightness, 0, 255);
  
  analogWrite(LED_PIN, brightness);
  Serial.printf("axisX: %d -> brightness: %d\n", rawValue, brightness);
}

void processControllers() {
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected() && ctl->hasData()) {
      if (ctl->isGamepad()) {
        processGamepad(ctl);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);

  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();
  
  delay(20);
}
