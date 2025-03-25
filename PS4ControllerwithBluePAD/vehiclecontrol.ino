/** Note: For Bluepad based code you need to have New board manager 
called ESP32+Bluepad   for more info look here - https://bluepad32.readthedocs.io/en/latest/ 
Quick Steps 
1. Install the board manager -  esp32_bluepad
2. select the right board maanger from tools --> its not arduino , not esp32, its esp32_bluepad 
3. Now put this code in ESP32. 
4. power on your controller and press share and playstation button(P) together. 
5. controller will give quick flash white lite and after connected a blue light (no blinking)
6. now left stick will control your vehicle.
7. please check the pins if your vehicle is not working or not behaving well   

This code is written by - Rahul Sharma for Yolabs - 24th Mar'25 

**/
#include <Bluepad32.h>

// Motor Control Pin Definitions (H-bridge inputs)
#define LEFT_MOTOR_FORWARD_PIN  13  // IN1
#define LEFT_MOTOR_REVERSE_PIN  12  // IN2
#define RIGHT_MOTOR_FORWARD_PIN 14  // IN3
#define RIGHT_MOTOR_REVERSE_PIN 27  // IN4

// Array to hold connected controllers
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Callback when a new controller is connected
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

// Callback when a controller is disconnected
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

// Set motor speed using analogWrite()
// 'speed' ranges from -255 (full reverse) to 255 (full forward)
void setMotorSpeed(uint8_t forwardPin, uint8_t reversePin, int speed) {
  if (speed > 0) {
    analogWrite(forwardPin, speed);
    analogWrite(reversePin, 0);
  } else if (speed < 0) {
    analogWrite(forwardPin, 0);
    analogWrite(reversePin, -speed);
  } else {
    analogWrite(forwardPin, 0);
    analogWrite(reversePin, 0);
  }
}

// Process gamepad input and drive the vehicle
void processGamepad(ControllerPtr ctl) {
  // Read left stick values:
  // axisY: typically -511 to 512 (pushing forward gives a negative value),
  // so we invert it to get a positive "forward" value.
  int rawForward = -ctl->axisX();
  int rawTurn    =  ctl->axisY();
  
  // Scale the raw values from [-511, 512] to [-255, 255]
  float scale = 255.0 / 512.0;
  int forwardSpeed = rawForward * scale;
  int turnSpeed    = rawTurn * scale;
  
  // Differential drive calculations:
  // Left motor: forwardSpeed + turnSpeed
  // Right motor: forwardSpeed - turnSpeed
  int leftSpeed  = forwardSpeed + turnSpeed;
  int rightSpeed = forwardSpeed - turnSpeed;
  
  // Constrain speeds to [-255, 255]
  leftSpeed  = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  // Apply computed speeds to the motors using analogWrite
  setMotorSpeed(LEFT_MOTOR_FORWARD_PIN, LEFT_MOTOR_REVERSE_PIN, leftSpeed);
  setMotorSpeed(RIGHT_MOTOR_FORWARD_PIN, RIGHT_MOTOR_REVERSE_PIN, rightSpeed);
  
  // Optional: Print debug information to the Serial Monitor
  Serial.printf("axisY: %d, axisX: %d -> Left Speed: %d, Right Speed: %d\n",
                -ctl->axisY(), ctl->axisX(), leftSpeed, rightSpeed);
}

// Process all connected controllers
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

  // Setup Bluepad32 with connection callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);

  // Initialize motor control pins as outputs
  pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_REVERSE_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_REVERSE_PIN, OUTPUT);
}

void loop() {
  // Update Bluepad32 to process any controller events
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();
  
  // Short delay to keep the loop responsive
  delay(20);
}

