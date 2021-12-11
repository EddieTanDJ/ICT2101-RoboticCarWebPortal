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
 *            |            P3.7  |--> Ultrasonic Echo
 *            |            P6.6  |--> Ultrasonic Trigger
 *
 *  PORT3_IRQHandler - Ultrasonic
 *
 * Author: Eddie
 *******************************************************************************/

#include <Sensor/init_Ultrasonic.h>

const Timer_A_ContinuousModeConfig contmConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_24,          // SMCLK/24 = 3MHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_CompareModeConfig CCR3Config =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_3,       //CCR3 ( TIMER_A2 + CCR3 => [P6.6] )
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_SET_RESET,
        0x4000//0xFFFE for very slow
};



void Initalise_HCSR04_New(void) {

    /* Configure P6.6 to output timer TA2.3 (secondary module function, output)*/
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configure P3.0 as input */
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN0);

    /* Configuring Timer_A2 CCR3 then set for continuous Mode */
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &contmConfig); // Set it as continous Mode which will wrap to 0 when it reach 2^16
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE); // Start the counter
    Timer_A_initCompare(TIMER_A2_BASE, &CCR3Config);   //CCR3 (turns OUT3 on)
    Timer_A_disableInterrupt(TIMER_A2_BASE); // Disable interrupt since we not using it

    /* Configuring P3.0 (ECHO) as an input and enable PORT3 interrupts */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION);
    Interrupt_enableInterrupt(INT_PORT3);
    printf("Ultrasonic initalised\n");
}
