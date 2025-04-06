// Author: Guy Hasewinkel
// System: Propulsio rig
// Program: Propulsion rig motor script
 
// Imported libraries
#include <Servo.h>
 
// Pin configuration
byte Motor_1 = 8;
byte Motor_2 = 9;
byte Motor_3 = 10;
byte Motor_4 = 11;
 
// Instances from libraries
Servo motor_1;
Servo motor_2;
Servo motor_3;
Servo motor_4;
 
// Variables for PWM
int new_PWM = 1000;
int old_PWM = 1000;
int commanded_PWM = 0;
 
// Set-up
void setup() {
  // Open serial monitor to write PWM values to motors
  Serial.begin(2000000); // Maximum baud rate
 
  // Attatch instance of motors
  motor_1.attach(Motor_1);
  motor_2.attach(Motor_2);
  motor_3.attach(Motor_3);
  motor_4.attach(Motor_4);
}
 
// Main loop function
void loop() {
  // Check if there is something on the serial monitor
  if(Serial.available()>0){
    // Retrieve value on the serial monitor
    commanded_PWM = Serial.parseInt();
 
    // Check if the value is not the default sent value
    if (commanded_PWM != 0){
      // Map the 0 to 100 duty cycle command to the ESC configured PWM range
      new_PWM = map(commanded_PWM, 0, 100, 1039, 1961);
    }
  }
 
  // Only change the written PWM value if it has changed
  if(new_PWM != old_PWM){
    old_PWM = new_PWM;
  }
 
  // Write the PWM value to the motors
  motor_1.write(old_PWM);
  motor_2.write(old_PWM);
  motor_3.write(old_PWM);
  motor_4.write(old_PWM);
}