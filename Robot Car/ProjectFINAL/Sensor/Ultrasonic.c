 /*******************************************************************************
 * ICT2104 Robotic Car
 *
 * Description: Initalise ultrasonic
 * .
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          | |                  |
 *          | |                  |
 *          --|RST               |
 *            |            P3.0  |--> Ultrasonic Echo
 *            |            P6.6  |--> Ultrasonic Trigger
 *
 *  PORT3_IRQHandler - Ultrasonic
 *
 * Author: Eddie
 / ******************************************************************************/
 // -------------------------------------------------------------------------------------------------------------------
#include <Sensor/Ultrasonic.h>
float dist, distance, divide;
extern float distance;
extern bool obstacle;
/*
*   Function: PORT3_IRQHandler
*   Input: void
*   Output: void
*   Description: This function detects if there is an obstacle in front of the robotic car
*/
 float detectObstacle(void)
 {
     if (start_pulse == 1) {

           tval1 = Timer_A_getCounterValue(TIMER_A2_BASE);

           //busy wait for the for input value to drop
           while((GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0) & 0x1) == 1);

           tval2 = Timer_A_getCounterValue(TIMER_A2_BASE);

           start_pulse = 0;

           if(tval1 > tval2)   //it means the 16-bit counter hit 0xffff and wrapped back to 0x0000
               tval2 = tval2 + 0xffff;

           /* take an average of 4 readings because single readings are a bit inaccurate */
           countval3 = countval2;
           countval2 = countval1;
           countval1 = countval0;
           countval0 = (tval2-tval1);
           /* to be smarter, try to ignore outliers (i.e. if the first or last readings are very different */
           countstat = (countval3 + countval2 + countval1 + countval0) >> 2;
          nn = nn+1;
          if(nn > 3)
          {
           dist = 0.5 * 100.0 * 340.0 * 0.000001 * (float)countstat;
//          printf("%.2f cm\n", dist);
           nn = 0;
          }
     }
     return dist;
 }

 // PORT 3 ISR for the echo pin when it goes high then calculate the distance
/*
*  Function: PORT3_IRQHandler
*  Input: void
*  Output: void
*  Description: This function is the interrupt handler for the echo pin and calculates the distance
* 
*/
 void PORT3_IRQHandler(void)
 {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
//    printf("In the ISR");
    if(status & GPIO_PIN0)
    {
        start_pulse = 1;
        distance = detectObstacle();
        if (distance > 1 && distance  < 15) {
//            printf("Obstacle Detected!\n");
            stop();
            obstacle = true;
        }
    }
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
 }
