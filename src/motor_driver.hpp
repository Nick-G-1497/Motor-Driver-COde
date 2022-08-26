#include <Arduino.h>

// Single Motor Instance
#include <L298N.h>

// For two motors instance at once
#include <L298NX2.h>

// #define EN_A 
#define IN1_B 6
#define IN2_B 9
// #define EN_A 
#define IN1_A 10
#define IN2_A 11

#define TURN_SPEED 40



/** Without Enable pin and jumper in place

                  motor A      motor B
              |------------||------------|
*/
L298NX2 myMotors(IN1_A, IN2_A, IN1_B, IN2_B);

// // With Enable pin to control speed

//                      motor A            motor B
//               |------------------||------------------|
// L298NX2 myMotors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

void turn_left_for_x_ms(unsigned long wait)
{
  myMotors.stopB();
  myMotors.stopA();
  myMotors.setSpeedA(TURN_SPEED);
  myMotors.setSpeedB(TURN_SPEED);
  myMotors.backwardA();
  myMotors.forwardB();
  delay(delay);
  myMotors.stopB();
  myMotors.stopA();
}
void turn_right_for_x_ms(unsigned long wait)
{
  myMotors.stopB();
  myMotors.stopA();
  myMotors.setSpeedA(TURN_SPEED);
  myMotors.setSpeedB(TURN_SPEED);
  myMotors.forwardA();
  myMotors.backwardB();
  myMotors.stopB();
  myMotors.stopA();
}

void forward_for_x_ms (unsigned long delay)
{
  myMotors.forwardFor(delay);
}

void backwards_for_x_ms(unsigned long delay)
{
  myMotors.backwardFor(delay);
}

void test() {
  // put your setup code here, to run once:
  forward_for_x_ms(1000);

  delay(1000);

  backwards_for_x_ms(1000);

  delay(1000);

  turn_left_for_x_ms(1000);

  delay(1000);

  turn_right_for_x_ms(1000);

  delay(1000);
  

}
