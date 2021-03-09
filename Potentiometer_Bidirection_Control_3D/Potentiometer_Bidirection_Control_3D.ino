/* This code is written for Winter 2021 MAE 156B Team 5 to control current output using an Arduino Mega 2560 
 *  and a Cytron 10A motor driver shield. This code will be used to vary current provided to Helmholtz Coils 
 *  for a 3D Magnetic Control System.
 *  
 *  Version 1 written by Tanner Hanson, January 2021
 *  Version 2 written By Joseph Martin, February 2021
 *  Last Edited by: Joseph Martin, March 8, 2021
 */
#include <LiquidCrystal.h>

//LiquidCrystal lcd = LiquidCrystal(34, 35, 36, 37, 38, 39);
LiquidCrystal lcd = LiquidCrystal(44, 42, 38, 36, 34, 32);

// Variables for motor driver and PWM value for motor
// These are manually set on the stacked motor drivers, 
// if replaced make sure the new driver matches the desired pin
int pwm_pin3 = 3;
int direction_pin3 = 2;
int pwm_pin2 = 5;
int direction_pin2 = 4;
int pwm_pin1 = 6;
int direction_pin1 = 7;

// pins set for potentiometers. Make 

int pot_pin1 = 3; //blue
int pot_pin2 = 4; //green
int pot_pin3 = 5; //yellow

int pot_pin4 = 0; //grey, white. This one is to set overall power output. 

// pins for ammeters

int am_pin1 = 8; //blue, bottom
int am_pin3 = 9; //green, top
int am_pin2 = 10; //yellow, middle

// for more complex bidirectional control
int motor_value = 0; // 0 - 255, magnitude of pwm signal
int motor_direction1 = 0; 
int motor_direction2 = 0;
int motor_direction3 = 0;

// Variables for counting
int long counter = 0;

// Variables for current 
float current1;
float current2; 
float current3;

float tempCurrent1; //these are for filtering purposes
float tempCurrent2; 
float tempCurrent3;

int loopFilter = 150; // For numerical filtering of current sensing.  

float zeroCurrent1 = 512; //outputCurrent1 is a float to display a more accurate output current on the lcd screen
float zeroCurrent2 = 512;
float zeroCurrent3 = 512;

float maxCurrent = 9.8; // cutoff for maximum Current allowed in each channel, CHANGE TO ~95% MOTOR DRIVER CAPACITY

// For determining max pwm to reach maxCurrent on each channel. Reset Arduino to reset calibration 
float maxPwmVal1 = 255;
float maxPwmVal2 = 255;
float maxPwmVal3 = 255;

// For tracking if output will produce overCurrents
boolean isOver1 = false; 
boolean isOver2 = false;
boolean isOver3 = false;

// var for pot
float potVal1;
float potVal2;
float potVal3;
float scalePot = 3.6; // get output into pwm range. CALIBRATE FOR YOUR POWER SUPPLY

// safety feature, fourth pot cuts off power to all
float potVal4;
float cutOff = 0; //scaling factor based on P
float half = 128; //

// values for output pwm 0-255
float pwmVal1;
float pwmVal2;
float pwmVal3;

//display vals
int displayVal1;
int displayVal2;
int displayVal3;

void setup() {
  Serial.begin(9600);              //serial output 
  
  lcd.begin(16, 2); //defines the size of the code

//  pinMode(direction_pin, OUTPUT);           // Set the direction pin output
  pinMode(pwm_pin1, OUTPUT);                 // Set the pwm pin output
    pinMode(pwm_pin2, OUTPUT); 
      pinMode(pwm_pin3, OUTPUT); 

  pinMode(pot_pin1, INPUT);
  pinMode(pot_pin2, INPUT);
  pinMode(pot_pin3, INPUT);
  pinMode(pot_pin4, INPUT);

  pinMode(am_pin1, INPUT);
  pinMode(am_pin2, INPUT);
  pinMode(am_pin3, INPUT);

  pinMode(direction_pin1, OUTPUT);
  pinMode(direction_pin2, OUTPUT);
  pinMode(direction_pin3, OUTPUT);

  digitalWrite(direction_pin1, 0);   // Set the motor in motion
  digitalWrite(direction_pin2, 0); 
  digitalWrite(direction_pin3, 0); 
  
  analogWrite(pwm_pin1, 0);              // Set the motor in motion
  analogWrite(pwm_pin2, 0);              // Set the motor in motion
  analogWrite(pwm_pin3, 0);              // Set the motor in motion

  current1 = analogRead(am_pin1) - zeroCurrent1;
  tempCurrent1 = current1;

  current2 = analogRead(am_pin2) - zeroCurrent2;
  tempCurrent2 = current2;

  current3 = analogRead(am_pin3) - zeroCurrent3;
  tempCurrent3 = current3;
}
 
void loop() {

  //read pots

  potVal1 = analogRead(pot_pin1);
  potVal2 = analogRead(pot_pin2);
  potVal3 = analogRead(pot_pin3);
  potVal4 = analogRead(pot_pin4);
  cutOff = potVal4/1010;
  if (cutOff > 1){
    cutOff = 1;
  }
  //delay(20);

  // current
  if (counter % 20 == 0)
  {
    lcd.clear();
  }
  if (counter % loopFilter == 0)
  {
    // update current
    current1 = tempCurrent1/loopFilter/20;
    current2 = tempCurrent2/loopFilter/20;
    current3 = tempCurrent3/loopFilter/20;
    
    tempCurrent1 = analogRead(am_pin1) - zeroCurrent1;
    tempCurrent2  = analogRead(am_pin2) - zeroCurrent2;
    tempCurrent3 = analogRead(am_pin3) - zeroCurrent3;
  }

  // set output pwm
  pwmVal1 = potVal1/scalePot;
  pwmVal2 = potVal2/scalePot;
  pwmVal3 = potVal3/scalePot;

  //increment temp current
  tempCurrent1 = tempCurrent1 + analogRead(am_pin1) - zeroCurrent1;
  tempCurrent2  = tempCurrent2 + analogRead(am_pin2) - zeroCurrent2;
  tempCurrent3 = tempCurrent3 + analogRead(am_pin3) - zeroCurrent3;


  // OUTPUT 

  //Bidirectional measure from halfway point
  //Serial.println(motor_direction2);
  if (pwmVal1 < half){
    motor_direction1 = 0;
  }
  else {
    motor_direction1 = 1;
  }
  if (pwmVal2 < half){
    motor_direction2 = 0;
  }
  else {
    motor_direction2 = 1;
    
  }
  if (pwmVal3 < half){
    motor_direction3 = 0;
  }
  else {
    motor_direction3 = 1;
  }
//  Serial.println(pwmVal1);
  pwmVal1 = abs(pwmVal1 - half)*2*cutOff;
  pwmVal2 = abs(pwmVal2 - half)*2*cutOff;
  pwmVal3 = abs(pwmVal3 - half)*2*cutOff;
  if (pwmVal1 > 255){
    pwmVal1 = 255;
  }
  if (pwmVal2 > 255){
    pwmVal2 = 255;
  }
  if (pwmVal3 > 255){
    pwmVal3 = 255;
  }
  
  digitalWrite(direction_pin1, motor_direction1);   // Set the motor in motion
  digitalWrite(direction_pin2, motor_direction2); 
  digitalWrite(direction_pin3, motor_direction3); 

// output to coils if measured current below threshold
  if (current1 < maxCurrent && pwmVal1 <= maxPwmVal1) {
      isOver1 = false;
      analogWrite(pwm_pin1, pwmVal1); 
  }
  else {
    if (!isOver1) {
      maxPwmVal1 = pwmVal1;
    }
    isOver1 = true;
    analogWrite(pwm_pin1, 0); 
  }
  
  if (current2 < maxCurrent && pwmVal2 <= maxPwmVal2) {
      isOver2 = false;
      analogWrite(pwm_pin2, pwmVal2); 
  }
  else {
    if (!isOver2) {
      maxPwmVal2 = pwmVal2;
    }
    isOver2 = true;
    analogWrite(pwm_pin2, 0); 
  }
  
  if (current3 < maxCurrent && pwmVal3 <= maxPwmVal3) {
      isOver3 = false;
      analogWrite(pwm_pin3, pwmVal3); 
  }
  else {
    if (!isOver3) {
      maxPwmVal3 = pwmVal3;
    }
    isOver3 = true;
    analogWrite(pwm_pin3, 0); 
  }
  
  lcd.setCursor(0, 0);
  lcd.print("1:");
  
  if (isOver1){
    lcd.print("maxd"); 
  }
  else {
    lcd.print(current1);    
  }
  lcd.print(",2:");
    if (isOver2){
    lcd.print("maxd"); 
  }
  else {
    lcd.print(current2);    
  }
  //lcd.print(" Amps");
  lcd.setCursor(0, 1);
  lcd.print("3:");
  
  if (isOver3){
    lcd.print("maxd"); 
  }
  else {
    lcd.print(current3);    
  }
  lcd.print(",P:");
  lcd.print((int)(cutOff*100));
  lcd.print("%");
  
  
//  Serial.print(tempCurrent1);
//  Serial.print(' ');
//   Serial.print(tempCurrent2);
//   Serial.print(' ');
    
  //Serial.println(maxPwmVal1);
  Serial.println(maxPwmVal3);
  counter ++;

}
