/** 
 * Note: For Bluepad based code you need to have a new board manager 
 * called ESP32+Bluepad. For more info, see: https://bluepad32.readthedocs.io/en/latest/
 * 
 * Quick Steps:
 * 1. Install the board manager: esp32_bluepad.
 * 2. From Tools, select the board manager: not "Arduino", not "ESP32", but "ESP32_Bluepad".
 * 3. Put this code into your ESP32.
 * 4. Power on your controller and press the Share and PlayStation buttons together.
 * 5. The controller will flash a quick white light and then display a steady blue light when connected.
 * 6. Now the left stick will control your vehicle while the right stick, triggers, and L1/R1 control the arm.
 * 7. Check the pins if your vehicle or arm is not working or behaving as expected.
 * 
 * This code is written by - Rahul Sharma for Yolabs - 24th Mar'25
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>

// -------------------- VEHICLE CONTROL --------------------

// Motor Control Pin Definitions (H-bridge inputs)
#define LEFT_MOTOR_FORWARD_PIN  13  // IN1
#define LEFT_MOTOR_REVERSE_PIN  12  // IN2
#define RIGHT_MOTOR_FORWARD_PIN 14  // IN3
#define RIGHT_MOTOR_REVERSE_PIN 27  // IN4

// -------------------- ARM CONTROL (PCA9685) --------------------

// Define servo pulse parameters (counts out of 4096)
#define SERVOMIN 100  // Pulse corresponding to approx. 0° (if used fully)
#define SERVOMAX 550  // Pulse corresponding to approx. 180°

// Arm servo channel assignments on PCA9685:
#define ARM_SERVO_CH_RX 0  // Right stick horizontal (axisRX)
#define ARM_SERVO_CH_RY 1  // Right stick vertical (axisRY)
#define ARM_SERVO_CH_L2 2  // L2 trigger (analog: brake)
#define ARM_SERVO_CH_R2 3  // R2 trigger (analog: throttle)
#define ARM_SERVO_CH_L1 4  // L1 button control (digital, configurable)
#define ARM_SERVO_CH_R1 5  // R1 button control (digital, configurable)

// For L1 and R1, we want the servo to move from ~90° (center) to 180°.
// Calculate a center pulse for approximately 90°.
int centerPulse = (SERVOMIN + SERVOMAX) / 2;  
// Configurable positions for L1 and R1:
int L1_PRESSED_PULSE   = SERVOMAX;    // When L1 pressed, move to 180°
int L1_RELEASED_PULSE  = centerPulse; // When L1 released, move to 90°
int R1_PRESSED_PULSE   = SERVOMAX;    // When R1 pressed, move to 180°
int R1_RELEASED_PULSE  = centerPulse; // When R1 released, move to 90°

  
// -------------------- GLOBAL OBJECTS --------------------

// Create an instance for the PCA9685 (arm control).
Adafruit_PWMServoDriver pwmServo = Adafruit_PWMServoDriver();

// Array to hold connected controllers.
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// -------------------- BLUEPAD32 CALLBACKS --------------------

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      myControllers[i] = ctl;
      return;
    }
  }
  Serial.println("CALLBACK: Controller connected, but no empty slot found");
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      return;
    }
  }
  Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
}

// -------------------- VEHICLE CONTROL FUNCTION --------------------

/* 
   For vehicle control we use the left stick.
   Here we invert axisX so that pushing the stick forward (to the left in this configuration) 
   gives a positive forward value.
*/
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

void processVehicle(ControllerPtr ctl) {
  int rawForward = -ctl->axisX();  // Invert axisX to get positive "forward" value
  int rawTurn    = ctl->axisY();    // Use axisY for turning
  
  float scale = 255.0 / 512.0;  // Scale controller range (-511 to +512) to PWM range (-255 to 255)
  int forwardSpeed = rawForward * scale;
  int turnSpeed = rawTurn * scale;
  
  int leftSpeed  = forwardSpeed + turnSpeed;
  int rightSpeed = forwardSpeed - turnSpeed;
  
  leftSpeed  = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  setMotorSpeed(LEFT_MOTOR_FORWARD_PIN, LEFT_MOTOR_REVERSE_PIN, leftSpeed);
  setMotorSpeed(RIGHT_MOTOR_FORWARD_PIN, RIGHT_MOTOR_REVERSE_PIN, rightSpeed);
  
  // Print debug information
  Serial.printf("Vehicle -> axisX: %d, axisY: %d -> Left: %d, Right: %d\n",
                ctl->axisX(), ctl->axisY(), leftSpeed, rightSpeed);
}

// -------------------- ARM CONTROL FUNCTION --------------------

void processArm(ControllerPtr ctl) {
  // Right stick: control two arm servos (channels 0 and 1)
  int rawRx = ctl->axisRX();  // Expected range: -511 to +512
  int rawRy = ctl->axisRY();
  int pulseRx = map(rawRx, -511, 512, SERVOMIN, SERVOMAX);
  int pulseRy = map(rawRy, -511, 512, SERVOMIN, SERVOMAX);
  pulseRx = constrain(pulseRx, SERVOMIN, SERVOMAX);
  pulseRy = constrain(pulseRy, SERVOMIN, SERVOMAX);
  pwmServo.setPWM(ARM_SERVO_CH_RX, 0, pulseRx);
  pwmServo.setPWM(ARM_SERVO_CH_RY, 0, pulseRy);
  
  // Analog triggers: L2 (brake) and R2 (throttle) control servos (channels 2 and 3)
  int rawL2 = ctl->brake();     // Analog value: 0 to 1023
  int rawR2 = ctl->throttle();  // Analog value: 0 to 1023
  int pulseL2 = map(rawL2, 0, 1023, SERVOMIN, SERVOMAX);
  int pulseR2 = map(rawR2, 0, 1023, SERVOMIN, SERVOMAX);
  pulseL2 = constrain(pulseL2, SERVOMIN, SERVOMAX);
  pulseR2 = constrain(pulseR2, SERVOMIN, SERVOMAX);
  pwmServo.setPWM(ARM_SERVO_CH_L2, 0, pulseL2);
  pwmServo.setPWM(ARM_SERVO_CH_R2, 0, pulseR2);
  
  // Digital buttons: L1 and R1 control servos (channels 4 and 5)
  // When pressed, the servo goes to 180° (SERVOMAX); when released, to 90° (centerPulse)
  int rawL1 = ctl->l1();  // Digital value: 0 or 1
  int rawR1 = ctl->r1();
  int pulseL1 = rawL1 ? L1_PRESSED_PULSE : L1_RELEASED_PULSE;
  int pulseR1 = rawR1 ? R1_PRESSED_PULSE : R1_RELEASED_PULSE;
  pwmServo.setPWM(ARM_SERVO_CH_L1, 0, pulseL1);
  pwmServo.setPWM(ARM_SERVO_CH_R1, 0, pulseR1);
  
  // Debug output for arm control.
  Serial.printf("Arm -> axisRX: %d -> S0: %d, axisRY: %d -> S1: %d\n", rawRx, pulseRx, rawRy, pulseRy);
  Serial.printf("Arm -> L2 (Brake): %d -> S2: %d, R2 (Throttle): %d -> S3: %d, L1: %d -> S4: %d, R1: %d -> S5: %d\n", 
                rawL2, pulseL2, rawR2, pulseR2, rawL1, pulseL1, rawR1, pulseR1);
}

// -------------------- PROCESS CONTROLLERS --------------------

void processControllers() {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr ctl = myControllers[i];
    if (ctl && ctl->isConnected() && ctl->hasData() && ctl->isGamepad()) {
      processVehicle(ctl);
      processArm(ctl);
    }
  }
}

// -------------------- SETUP & LOOP --------------------

void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup Bluepad32 with connection callbacks.
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);

  // Initialize vehicle motor control pins as outputs.
  pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_REVERSE_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_REVERSE_PIN, OUTPUT);
  
  // Initialize PCA9685 for arm control.
  pwmServo.begin();
  pwmServo.setPWMFreq(50);  // Typical servo frequency: 50 Hz.
  delay(10);
}

void loop() {
  if (BP32.update()) {
    processControllers();
  }
  delay(20);
}
