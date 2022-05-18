/* Move the Servo
  Rotate a servo  from Zero to 180. Connect to any digital(PWM) pin and servo will rotate. 
  Refer the diagram  for connection to servo  and ESP32
  Connections 
  Servo  -            ESP32 
  Brown -             Ground
  Red   -             3v3
  Orange/Yellow  -    Digital pin - D4 in this code 
  
  NOTE : If your servo need to take a good load or if you are connecting multiple servo,  
  you will  need external power supply with/without servo controller board               
  
  by Rahul Sharma for Yolabs 
  This example code is in the public domain.
  https://www.yolabs.in
*/






#include <Servo.h>

Servo servo_2;

void setup()
{
  servo_2.attach(4);

}


void loop()
{

    servo_2.write(180);
    delay(3000);
    servo_2.write(0);
    delay(3000);

}
