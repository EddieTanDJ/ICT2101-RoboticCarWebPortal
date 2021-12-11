/******************************************************************************
* MSP432 Line Tracker
*
* Description: A line tracker module that detect black tiles using GPIO.
*
*                MSP432P401
*             ------------------
*         /|\|                  |
*          | |                  |
*          --|RST               |
*            |             P6.4 | -> Left Line Tracker
*            |             P6.5 | -> Right Line Tracker
*            |                  |
*            |                  |
*            |                  |
*            |                  |
* Author: Eddie
*******************************************************************************/
#include "Linetracker.h"
#include <Motor/Motor.h>
// Global Variables
extern blackTiles;
extern bool isTurning;
extern uint32_t leftcount;
/*
 * Function: initLineTracker
 * Description: Initialize the line tracker module.
 * Inputs: None
 * Outputs: None
 */
void init_line_tracker(void)
 {
   // Set P2.0 / 2.1 as output pin
   MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
   // Set P2.5 as pull up resistor
   MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN4);
   MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN5);
   // Because by defualt is high to low transition
   // When it detect black tiles, it will be low to high transition
   MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
   MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P6, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
   // Clear InterruptFlag on P6.4 AND P6.5
   MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
   MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5);
   // Enable Port P6.4 AND P6.5 as Interrupt
   MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
   MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN5);
   // Set P2.0 to low
   MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
   MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
   // Enable interrupts globally for PORT6.0
   MAP_Interrupt_enableInterrupt(INT_PORT6);
}
/*
 * Function: PORT6 Interrupt Handler
 * Description: Interrupt Service Routine for line tracker.
 * Inputs: None
 * Outputs: None
 */
void PORT6_IRQHandler(void)
{

  uint32_t status, status2;
  status = MAP_GPIO_getInterruptStatus(GPIO_PORT_P6, GPIO_PIN4);
  status2 = MAP_GPIO_getInterruptStatus(GPIO_PORT_P6, GPIO_PIN5);
  // If P6.4 & P6.5 is high black tiles detected
  if (status |= GPIO_PIN4) {
      if (status2 |= GPIO_PIN5) {
          // Toggle the LED
          GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
          __delay_cycles(24000000);
          stop();
          blackTiles = true;
      }
  }
  // Clear the flag by resetting the local variable
  MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status);
  MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, status2);
}

