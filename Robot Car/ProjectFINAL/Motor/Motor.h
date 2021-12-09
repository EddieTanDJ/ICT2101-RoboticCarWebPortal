/*
 * Motor.h
 *
 *  Created on: 23 Oct 2021
 *      Author: Eddie
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdbool.h>


// All the methods
void Initalise_Motor(void);
void PORT1_IRQHandler(void);
void moveForward(int tiles);
void moveBackward(int tiles);
void moveLeftFoward();
void moveRightFoward();
void moveLeftBackward();
void moveRightBackward();
void stop();
void turnleft();
void turnright();
void generatePWM();
void CarPID(int target);
void checkCommand();
void rotate();

#endif /* MOTOR_MOTOR_H_ */
