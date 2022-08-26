#include <Arduino.h>
#include "motor_driver.hpp"


#include <L298N.h>
#include <L298NX2.h>
#define DEBUG 1 //show print statements?
const int echoPin = 2; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 3; //attach pin D3 Arduino to pin Trig of HC-SR04
const int LIGHT_PIN = A0; // Pin connected to voltage divider output
const int LIGHT_PIN2 = A1; //RIGHT
const int LIGHT_PIN3 = A2;
const int LED_PIN = 13; // Use built-in LED as dark indicator
// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 5.04; // Measured voltage of Ardunio 5V line
const float R_DIV = 4660.0; // Measured resistance of 3.3k resistor
// Set this to the minimum resistance require to turn an LED on:
const float LIGHT_THRESHOLD = 10000;
//#define EN_A
// const int IN1_A = 7;
// const int IN2_A = 6
// // #define EN_A
// const int IN1_B = 5;
// const int IN2_B = 4;
// const int TURN_SPEED =40;
// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
float lightV1; // voltage across photoresistor      CENTER
float lightR1; // resistance across photoresistor   CENTER
float lightV2; // voltage across photoresistor      RIGHT
float lightR2; // resistance across photoresistor   RIGHT
float lightV3; // voltage across photoresistor      LEFT
float lightR3; // resistance across photoresistor   LEFT
bool wentBackwards = false;
//Set up motors
// L298NX2 myMotors(IN1_A, IN2_A, IN1_B, IN2_B);

void StopMotor(){
  if (DEBUG)
    Serial.println("Motor stopped");
  myMotors.stop();
}


void GoBackwards(unsigned long wait){
  if (DEBUG)
    Serial.println("Going backwards");
  myMotors.backwardFor(wait);
  StopMotor();
}

void GoStraight(unsigned long wait){
  if (DEBUG)
    Serial.println("Going straight");
  myMotors.forwardFor(wait);
}
void GoLeft(unsigned long wait){
  if (DEBUG)
    Serial.println("Going left");
  //for x ms
  myMotors.stopB();
  myMotors.stopA();
  myMotors.setSpeedA(TURN_SPEED);
  myMotors.setSpeedB(TURN_SPEED);
  myMotors.backwardA();
  myMotors.forwardB();
  delay(wait);
  myMotors.stopB();
  myMotors.stopA();
}
void GoRight(unsigned long wait){
  if (DEBUG)
    Serial.println("Going right");
  //for x ms
  myMotors.stopB();
  myMotors.stopA();
  myMotors.setSpeedA(TURN_SPEED);
  myMotors.setSpeedB(TURN_SPEED);
  myMotors.forwardA();
  myMotors.backwardB();
  delay(wait);
  myMotors.stopB();
  myMotors.stopA();
}
float GetLight(int lightpin, float lightV, float lightR){
  // Read the ADC, and calculate voltage and resistance from it
  int lightADC = analogRead(lightpin);
  if (lightADC > 0)
  {
    // Use the ADC reading to calculate voltage and resistance
    lightV = lightADC * VCC / 1023.0;
    lightR = R_DIV * (VCC / lightV - 1.0);
    if (DEBUG){
      Serial.println("LightPin: " + String(lightpin));
      Serial.println("Voltage: " + String(lightV) + " V");
      Serial.println("Resistance: " + String(lightR) + " ohms");
    }
  }
  return lightV, lightR;
}
float GetDistance(int trigPin, int echoPin){
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
}


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(LIGHT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  if (DEBUG)
    Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  if (DEBUG)
    Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  if (DEBUG)
    Serial.println("with Arduino UNO R3");
}


void loop() {
  //Get distance
  distance = GetDistance(trigPin, echoPin);
  delay(1000);
  //Get light
  lightV1, lightR1 = GetLight(LIGHT_PIN, lightV1, lightR1);
  lightV2, lightR2 = GetLight(LIGHT_PIN2, lightV2, lightR2);
  lightV3, lightR3 = GetLight(LIGHT_PIN3, lightV3, lightR3);
  if (DEBUG)
    Serial.println(lightR2);
    //Control based off of light & distance
    if(distance <= 10){
      if (DEBUG)
        Serial.println("Too close");
      if (GoBackwards == false){
        GoBackwards(1000);
      }
      wentBackwards = true;
    }
    else{
      if ((lightR1 < lightR2) && (lightR1<lightR3)){
        wentBackwards = false;
        if (DEBUG)
          Serial.println("Go straight");
        GoStraight(100);
      }
      else if ((lightR2 <lightR1) and (lightR2 < lightR3)){
        wentBackwards = false;
        if (DEBUG)
          Serial.println("Go right");
        GoRight(100);
      }
      else if ((lightR3 < lightR1) and (lightR3 <lightR2)){
        wentBackwards = false;
        if (DEBUG)
          Serial.println("Go left");
        GoLeft(100);
      }
      else{
        wentBackwards = false;
        if (DEBUG)
          Serial.println("Stop");
        StopMotor();
      }
    }
    myMotors.stop();
}