/*******************************************************************************
 * MSP432 Timer Interrupt
 *
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P5.0  |<-- Left Encoder
 *            |                  |
 *            |            P5.2  |<--- Right Encoder
 *            |                  |
 *            |                  |
 *
 *
 * MUST PUT THIS DEFINITION IN PROJECT
 ******************************************************************************/


#include <Sensor/encoder.h>
#include <Motor/motor.h>

extern uint32_t PWML;
extern uint32_t PWMR;
extern uint32_t leftcount;
extern uint32_t rightcount;
extern uint32_t grid_pulse_right;
extern uint32_t grid_pulse_left;
extern uint32_t pulseCountL;
extern uint32_t pulseCountR;
extern bool isTurning;

/* Timer_A UpMode Configuration Parameter to count the time*/
const Timer_A_UpModeConfig samplePeriodforSpeed =
{
        TIMER_A_CLOCKSOURCE_ACLK,           // S Clock Source 32.768kHz
        TIMER_A_CLOCKSOURCE_DIVIDER_10,      // Divider 10 to count every ten second
        TIMER_PERIOD,                       // When reach 0x8000, will trigger interrupt. Count to 0x8000 to count 10 secs
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
};

void Initalise_Enconder(void)
{
    // Check Clock speed for Aclk clock
//    int b = CS_getACLK();
//    printf("%d\n",b);
    // Left Enconder for P5.0
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN0);
    // Select the edge to trigger the interrupt from high to low transition
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    // Clear InterruptFlag on P5.0
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN0);
    // Enable Port P3.5 as Interrupt
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN0);

    // Right Enconder for P5.2
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN2);
    // Select the edge to trigger the interrupt from high to low transition
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    // Clear InterruptFlag on P5.2
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN2);
    // Enable Port P5.2 as Interrupt
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN2);

    // Enable interrupts globally for PORT5
    MAP_Interrupt_enableInterrupt(INT_PORT5);

    /* Configuring Timer_A3 for Up Mode */
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &samplePeriodforSpeed);
    /* Enabling interrupts and starting the timer */
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
    printf("\n\rTimer A3 configured for Up Mode\n\r");
}

void PORT5_IRQHandler(void)
{
    uint32_t status1;
    uint32_t status2;
    status1 = MAP_GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN0);
    status2 = MAP_GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN2);

    // If P5.0 is low for left encoder 
    if (status1 &= GPIO_PIN0)
    {
        leftcount++;
        pulseCountL++;
        grid_pulse_left++;
        leftcountrotation++;
    }
     // Every one revualatiion, adjust the PID
    if ( pulseCountL % 20 == 0 && pulseCountL > 19 && isTurning == 0) {
        CarPID(pulseCountL);
        pulseCountR = pulseCountL;
    }

    // if ( pulseCountL == pulseCountTrigger ) {

    //     pulseCountL = 0;
    //     pulseCountR = 0;
    // }


    if (leftcountrotation == 20)
    {
        leftdistance += 22;
       // printf("Left Distance: %d\n", leftdistance);
        leftcountrotation = 0;
    }

    // If P5.2 is low
    if (status2 &= GPIO_PIN2)
    {
        rightcount++;
        grid_pulse_right++;
        rightcountrotation++;
        pulseCountR++;
    }
    // Every one revualatiion, adjust the PIDD
    if ( pulseCountR % 20 == 0 && pulseCountR > 19 && isTurning == 0) {
        CarPID(pulseCountR);
        pulseCountL = pulseCountR;
    }
    if (rightcountrotation == 20)
    {
        rightdistance += 22;
        //printf("Right Distance: %d\n", rightdistance);
        rightcountrotation = 0;
    }

    // Clear the flag by resetting the local variable
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status2);
}

void startTimer ()
{
//    printf("Timer Started!");
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    MAP_Timer_A_enableCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    leftcount = 0;
    rightcount = 0;

}

void stopTimer()
{
    volatile static double decimal = 0;
    currentCount = Timer_A_getCounterValue(TIMER_A3_BASE);
    decimal = (double)currentCount / (double)TIMER_PERIOD; // Current Counter Value / Period to get the decimal for second
    decimal = decimal * 10; // Multiply by 10 to get the decimal for tenth of second
    sum = time + decimal;

    // printf("Timer stop at %.2f secs\n", sum);
    Timer_A_disableCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    // remaingleftdistance = (double) leftdistance + (double)((leftcountrotation / 20)*22);
    // remaingrightdistance = (double) rightdistance + (double)((rightcountrotation / 20)*22);
    // printf("Left Distance transversed: %.2f cm\n", remaingleftdistance);
    // printf("Right Distance transversed: %.2f cm\n", remaingrightdistance);
    time = 0;
    leftdistance = 0;
    rightdistance = 0;
    leftcount = 0;
    rightcount = 0;
    Timer_A_stopTimer(TIMER_A3_BASE);
    Timer_A_clearTimer(TIMER_A3_BASE);
}

void TA3_0_IRQHandler(void)
{
    time+=10;
    leftspeed = (double)leftdistance / (double)time;
    rightspeed = (double)rightdistance / (double)time;
    // printf("Left Distance: %d\n", leftcountrotation);
    // printf("Right Distance: %d\n", rightcountrotation);
    // printf("Time: %d\n", time);
    // printf("Left Speed: %.2f\n", leftspeed);
    // printf("Right Speed: %.2f\n", rightspeed);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
