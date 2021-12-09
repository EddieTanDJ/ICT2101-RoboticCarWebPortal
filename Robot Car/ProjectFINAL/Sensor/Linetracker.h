/*
 * Ultrasonic.h
 *
 *  Created on: 09 11 2021
 *      Author: Eddie
 */

#ifndef LINETRACKER_LINETRACKER_H_
#define LINETRACKER_LINETRACKER_H_

#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Function prototypes
void init_line_tracker(void);
//void PORT3_IRQHandler(void);

volatile static int index = 0;
volatile static int count = 0;
#endif /* LINETRACKER_LINETRACKER_H_ */
