/*
 * Ultrasonic.h
 *
 *  Created on: 23 Oct 2021
 *      Author: Eddie
 */

#ifndef ULTRASONIC_DRIVER_H_
#define ULTRASONIC_DRIVER_H_

#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define MIN_DISTANCE    15.0f
#define TICKPERIOD      1000
#define TIMERPERIOD


volatile static uint32_t tval1, tval2, nn=0;
volatile static uint32_t countval0 = 0, countval1 = 0, countval2 = 0, countval3 = 0, countstat;


volatile static int start_pulse = 1;
float detectObstacle(void);
#endif /* ULTRASONIC_DRIVER_H_*/
