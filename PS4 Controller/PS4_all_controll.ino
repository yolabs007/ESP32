/* This code is written by Rahul Sharma for Yolabs - 19th July 2024

This code allow for a quick connection b/w ps4 controller give aceesss to all the keys so you can use them as you wish

This code requires MAC Address of your Gamepad so you must be knowing the same for PS4 you can get it as follows 

1. Start the PS4 controller and put it in pairing mode - 10-15 sec  PS4 and share button pressed together ( a quick double flash light like police cars)

2. Go ot mac or phone and connect the device ( MAC preffered)

3. Run this command in your terminal - system_profiler SPBluetoothDataType and note down the mac address for Connected:

     DUALSHOCK 4 Wireless Controller:  for example - Address: A4:AE:12:C3:9F:57


4. Note: this is not must use only if ESP32 espress if is not working -  Now Install  a new board manager called ESP32 Bluepad 32-  follow this link for ref - https://www.youtube.com/watch?v=-JTZOHBaWOo&t=4s

5. Select the board and USB port - Please note someties DOIT ES32 devkit V1 and sometimes DOIT ES32 devkit V1 _esp32 bluepad works so try both if one is not working

6. Left joystick up & down will allow you to control the  Build in LED. 

  */

#include <PS4Controller.h> 
#include <BluetoothSerial.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>


// Variables to store the state of the controller
bool up, down, left, right;
bool square, cross, circle, triangle;
bool l1, r1, l2, r2;
bool l3, r3;
bool share, options, psButton, touchpad;
int lStickX, lStickY, rStickX, rStickY;
int l2Value, r2Value;
int gyroX, gyroY, gyroZ;
int accelX, accelY, accelZ;

// for Debuging only ********************

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const char* ps4ControllerMAC = "A4:AE:12:C3:9F:57"; // ensure mac address of your ps4 controller 
const int ledPin = 2;

void setup() {
  Serial.begin(115200);

  // Initialize Bluetooth
  if (!btStart()) {
    Serial.println("Failed to initialize controller");
    return;
  }
  
  if (esp_bluedroid_init() != ESP_OK) {
    Serial.println("Failed to initialize bluedroid");
    return;
  }

  if (esp_bluedroid_enable() != ESP_OK) {
    Serial.println("Failed to enable bluedroid");
    return;
  }

  // Print the ESP32 device MAC address
  Serial.print("ESP32 MAC Address: ");
  printDeviceAddress();
  Serial.println("");

  
  // Initialize PS4 controller connection with PS4 controller's MAC address
  Serial.print("Connecting to PS4 controller at MAC address: ");
  Serial.println(ps4ControllerMAC);
  if (PS4.begin(ps4ControllerMAC)) {
    Serial.println("PS4 controller started. Waiting for connection...");
  } else {
    Serial.println("Failed to start PS4 controller.");
  }

  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisconnect);

  // Debuging code ends here 

  // Initialize the LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is off initially
}




void loop() {
  if (PS4.isConnected()) {
    Serial.println("PS4 controller is connected.");
    readControllerState();

    int val = PS4.LStickY();
    
     

    Serial.printf("LStickY: %d", val);
    Serial.printf("Up: %d, Down: %d, Left: %d, Right: %d, Square: %d, Cross: %d, Circle: %d, Triangle: %d, "
                      "L1: %d, R1: %d, L2: %d, R2: %d, L3: %d, R3: %d, Share: %d, Options: %d, PSButton: %d, Touchpad: %d, "
                      "LStickX: %d, LStickY: %d, RStickX: %d, RStickY: %d, L2Value: %d, R2Value: %d, "
                      "GyroX: %d, GyroY: %d, GyroZ: %d, AccelX: %d, AccelY: %d, AccelZ: %d\n",
                      up, down, left, right, square, cross, circle, triangle,
                      l1, r1, l2, r2, l3, r3, share, options, psButton, touchpad,
                      lStickX, lStickY, rStickX, rStickY, l2Value, r2Value,
                      gyroX, gyroY, gyroZ, accelX, accelY, accelZ);

    if (val >= 100) {
      Serial.println("Turning LED on.");
      digitalWrite(ledPin, HIGH);
    } else if (val <= -100) {
      Serial.println("Turning LED off.");
      digitalWrite(ledPin, LOW);
    }

    delay(100); // Delay for readability
  } 
  else {
    Serial.println("PS4 controller is not connected.");
    delay(1000); // Delay for readability
  }
}

// function to print MAC address of ESP32 not required again and again if we do not want to print 

void printDeviceAddress() {
  const uint8_t* mac = esp_bt_dev_get_address();
  if (mac != nullptr) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println(macStr);
  } else {
    Serial.println("Failed to get MAC address");
  }
}

void onConnect() {
  Serial.println("PS4 Controller Connected!");
}

void onDisconnect() {
  Serial.println("PS4 Controller Disconnected!");
}



// Function to read and save the state of the controller
void readControllerState() {
    up = PS4.Up();
    down = PS4.Down();
    left = PS4.Left();
    right = PS4.Right();
    square = PS4.Square();
    cross = PS4.Cross();
    circle = PS4.Circle();
    triangle = PS4.Triangle();
    l1 = PS4.L1();
    r1 = PS4.R1();
    l2 = PS4.L2();
    r2 = PS4.R2();
    l3 = PS4.L3();
    r3 = PS4.R3();
    share = PS4.Share();
    options = PS4.Options();
    psButton = PS4.PSButton();
    touchpad = PS4.Touchpad();
    lStickX = PS4.LStickX();
    lStickY = PS4.LStickY();
    rStickX = PS4.RStickX();
    rStickY = PS4.RStickY();
    l2Value = PS4.L2Value();
    r2Value = PS4.R2Value();
    gyroX = PS4.GyrX();
    gyroY = PS4.GyrY();
    gyroZ = PS4.GyrZ();
    accelX = PS4.AccX();
    accelY = PS4.AccY();
    accelZ = PS4.AccZ();
}

