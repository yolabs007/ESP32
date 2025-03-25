#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>

// Create an instance of the PCA9685 driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define servo pulse parameters (counts out of 4096)
#define SERVOMIN 100  // Pulse for approx. 0° (if used fully)
#define SERVOMAX 550  // Pulse for approx. 180°

// Servo channel assignments:
// Right Stick (Analog; range approx. -511 to +512)
#define SERVO_CH_RX 0  // Right stick horizontal (axisRX)
#define SERVO_CH_RY 1  // Right stick vertical (axisRY)
// Analog triggers (0–1023):
#define SERVO_CH_L2 2  // Brake trigger (L2)
#define SERVO_CH_R2 3  // Throttle trigger (R2)
// Digital buttons L1 and R1 with configurable positions:
#define SERVO_CH_L1 4  // L1 button control
#define SERVO_CH_R1 5  // R1 button control

// Calculate the center pulse (approx. 90°) for servos
int centerPulse = (SERVOMIN + SERVOMAX) / 2;

// Configure desired servo pulse values for L1 and R1
// Now the released state will be at 90° (centerPulse) and pressed state at 180° (SERVOMAX)
int L1_PRESSED_PULSE   = SERVOMAX;    // When L1 is pressed: servo moves to 180°
int L1_RELEASED_PULSE  = centerPulse; // When L1 is released: servo moves to 90°
int R1_PRESSED_PULSE   = SERVOMAX;    // When R1 is pressed: servo moves to 180°
int R1_RELEASED_PULSE  = centerPulse; // When R1 is released: servo moves to 90°

 
// Array to hold connected controllers.
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Callback: When a new controller connects.
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("Controller connected at index=%d\n", i);
      myControllers[i] = ctl;
      return;
    }
  }
  Serial.println("Controller connected, but no free slot available");
}

// Callback: When a controller disconnects.
void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      return;
    }
  }
  Serial.println("Controller disconnected, but was not found");
}

void processGamepad(ControllerPtr ctl) {
  // --- Right Stick Controls (Channels 0 & 1) ---
  int rawRx = ctl->axisRX(); // Expected range: -511 to +512
  int rawRy = ctl->axisRY();
  int pulseRx = map(rawRx, -511, 512, SERVOMIN, SERVOMAX);
  int pulseRy = map(rawRy, -511, 512, SERVOMIN, SERVOMAX);
  pulseRx = constrain(pulseRx, SERVOMIN, SERVOMAX);
  pulseRy = constrain(pulseRy, SERVOMIN, SERVOMAX);
  pwm.setPWM(SERVO_CH_RX, 0, pulseRx);
  pwm.setPWM(SERVO_CH_RY, 0, pulseRy);
  
  // --- Trigger Controls (Analog) ---
  // L2 (Brake) and R2 (Throttle) return analog values from 0 to 1023.
  int rawL2 = ctl->brake();
  int rawR2 = ctl->throttle();
  int pulseL2 = map(rawL2, 0, 1023, SERVOMIN, SERVOMAX);
  int pulseR2 = map(rawR2, 0, 1023, SERVOMIN, SERVOMAX);
  pulseL2 = constrain(pulseL2, SERVOMIN, SERVOMAX);
  pulseR2 = constrain(pulseR2, SERVOMIN, SERVOMAX);
  pwm.setPWM(SERVO_CH_L2, 0, pulseL2);
  pwm.setPWM(SERVO_CH_R2, 0, pulseR2);
  
  // --- L1 and R1 Button Controls ---
  // These are digital: when pressed the servo goes to the "pressed" value (180°)
  // when not pressed it goes to the "released" value (90°).
  int rawL1 = ctl->l1();  // Typically returns 0 or 1.
  int rawR1 = ctl->r1();
  int pulseL1 = rawL1 ? L1_PRESSED_PULSE : L1_RELEASED_PULSE;
  int pulseR1 = rawR1 ? R1_PRESSED_PULSE : R1_RELEASED_PULSE;
  pwm.setPWM(SERVO_CH_L1, 0, pulseL1);
  pwm.setPWM(SERVO_CH_R1, 0, pulseR1);
  
  // Debug output for calibration.
  Serial.printf("RX: %d -> S0: %d, RY: %d -> S1: %d\n", rawRx, pulseRx, rawRy, pulseRy);
  Serial.printf("Brake (L2): %d -> S2: %d, Throttle (R2): %d -> S3: %d\n", rawL2, pulseL2, rawR2, pulseR2);
  Serial.printf("L1: %d -> S4: %d, R1: %d -> S5: %d\n", rawL1, pulseL1, rawR1, pulseR1);
}

void processControllers() {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr ctl = myControllers[i];
    if (ctl && ctl->isConnected() && ctl->hasData() && ctl->isGamepad()) {
      processGamepad(ctl);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting 6-servo controller with configurable L1/R1 (90° to 180°)...");
  
  // Initialize the PCA9685 and set PWM frequency to 50 Hz (typical for servos)
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);
  
  // Setup Bluepad32 for PS4 controller handling.
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
  
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

void loop() {
  if (BP32.update()) {
    processControllers();
  }
  delay(20);
}
