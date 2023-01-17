/* Sweep the Servo
  Sweep a servo  from Zero to 180. Connect to any digital(PWM) pin and servo will rotate. 
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

Servo servo_2;  //create a servo object

int angle = 0;   // servo position in degrees 
 

void setup()
{
  servo_2.attach(4);   //attach a pin to servo 

}


void loop() 
{ 
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++)  
  {                                  
    servo_2.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    servo_2.write(angle);           
    delay(15);       
  } 
} 
