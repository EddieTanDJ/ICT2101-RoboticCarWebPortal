/*
 * Encoder.h
 *
 *  Created on: 23 Oct 2021
 *      Author: Eddie
 */


#ifndef ENCODER_ENCODER_H
#define ENCODER_ENCODER_H

#include <stdio.h>
#include <stdlib.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
volatile static int leftcountrotation = 0;
volatile static int leftdistance = 0;
volatile static double leftspeed = 0;
volatile static int rightcountrotation = 0;
volatile static int rightdistance = 0;
volatile static double rightspeed = 0;
volatile static int time = 0;
volatile static int currentCount = 0;
volatile static double sum = 0;
volatile static double remaingleftdistance = 0;
volatile static double remaingrightdistance = 0;
// volatile static int kp = 0.02;
// volatile static float Kd = 0.01;
// volatile static float Ki = 0.05;




#define TIMER_PERIOD 0x8000

// Function in Enconder
void Initalise_Enconder(void);
void PORT5_IRQHandler(void);
void startTimer (void);
void stopTimer (void);
void TA3_0_IRQHandler(void);
void CarPID();

#endif /* ENCODER_ENCODER_H*/
