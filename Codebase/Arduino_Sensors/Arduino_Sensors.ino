// Author: Guy Hasewinkel
// System: Propulsion rig
// Program: Propulsion rig sensor script
 
// Imported libraries
#include "HX711.h"
#include "ACS712.h"
 
// Pin configuration used for the sensor arduino
const int HX711_data = 7;
const int HX711_clock = 6;
const int IRSensor = 3;
const int Current = A0;
 
// Instances created from the imported libraries
ACS712 ACS1(Current, 5.0, 1023, 40);
HX711 scale;
 
// Variables
float voltage;
float load;
unsigned long currentTime;
volatile unsigned int rotations = 0;
volatile float RPM = 0;
volatile float rpm = 0;
unsigned long debounceTime = 0;
volatile unsigned long lastInterruptTime = 0;
unsigned long debounce;
unsigned long old_debounce = 0;
float cur;
unsigned long timeDifference;
 
// Set-up
void setup() {
  // Open serial monitor to write measurands to
  Serial.begin(2000000); // Maximum baud rate
 
  // Initialise load cell and HX711
  scale.begin(HX711_data, HX711_clock);
  // Set scale
  scale.set_scale(430);
  // Tare the load cell and HX711 to zero
  scale.tare();
 
  // Initialise IR sensor interrupt
  pinMode(digitalPinToInterrupt(IRSensor), INPUT);
  // Interrupt service routine to calculate RPM is called rpm_calculator
  attachInterrupt(digitalPinToInterrupt(IRSensor), rpm_calculator, FALLING);
 
  // Initialise ACS sensor
  ACS1.autoMidPoint();
}
 
// Main loop function
void loop() {
  // Read current value
  cur = ACS1.mA_DC()/1000;
  load = scale.get_units(1);
  // Printing to the serial monitor - timestamp, load, load, current, current, RPM, RPM, voltage.
  Serial.print(millis()); // ms time value
  Serial.print(",");
  Serial.print(load); // Load cell value in g
  Serial.print(",");
  Serial.print(load); // Load cell value in g
  Serial.print(",");
  Serial.print(cur, 4); // Current value in A
  Serial.print(",");
  Serial.print(cur,4); // Current value in A
  Serial.print(",");
  Serial.print(RPM); // RPM
  Serial.print(",");
  Serial.print(RPM); // RPM value
  Serial.println();
}
 
// Interrupt service routine
void rpm_calculator(){
  debounce = 0.1*(60/RPM)*1000000.0/2;
  currentTime = micros();
  if (currentTime - lastInterruptTime > debounce) {
    timeDifference = currentTime - lastInterruptTime;
    lastInterruptTime = currentTime;
    RPM = (1/(timeDifference/60000000.0f))/2;
    rotations++;
  }
 
}