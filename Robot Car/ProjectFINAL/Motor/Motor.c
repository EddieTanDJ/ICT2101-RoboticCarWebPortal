/*******************************************************************************
 * MSP432 Timer_A - Variable PWM
 *
 * Description: In this example, the Timer_A module is used to create a precision
 * PWM with an adjustable duty cycle. The PWM initial period is 80 ms and is
 * output on P2.4 and P2.5. The initial duty cycle of the PWM is 10%, however when the
 * button is pressed on P1.1 the duty cycle is sequentially increased by 10%.
 * Once the duty cycle reaches 90%, the duty cycle is reset to 10% on the
 * following button press.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.1  |<--Toggle Switch for output PWN ON P2.4
 *            |            P1.4  |<--Toggle Switch for output PWM ON P2.5
 *            |                  |
 *            |            P2.4  |--> Output PWM for left motor
 *            |            P2.5  |--> Output PWM for right motor
 *            |                  |
 *
 *******************************************************************************/
/* Standard Include */
#include <Motor/Motor.h>
#include <stdio.h>
extern uint32_t leftcount;
extern uint32_t rightcount;
extern uint32_t grid_pulse_left;
extern uint32_t grid_pulse_right;
int pulsetarget;
extern uint32_t rightSpeed;
extern bool isTurning;

// For the PID
extern uint32_t lastErrorLeft;
extern uint32_t lastErrorRight;
extern uint32_t totalErrorLeft;
extern uint32_t totalErrorRight;
extern float leftPIDOutput;
extern float rightPIDOutput;
extern uint32_t PWML;
extern uint32_t PWMR;
extern uint32_t speedL;
extern uint32_t speedR;

//#include <Sensor/encoder.h>
/* TODO: Since the enconder */
// Right Engine
Timer_A_PWMConfig pwmConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        30000, // 100% PWM
        // For Port2.4 as it is TA_0.1
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        15000};

//Configure for the left engine
Timer_A_PWMConfig pwmConfig2 =
    {
        // Need to increase the PWM for right motor as it is moving slower
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_24,
        30000,
        // For Port 2.5 as it is TA_0.2
        TIMER_A_CAPTURECOMPARE_REGISTER_2,
        TIMER_A_OUTPUTMODE_RESET_SET,
        15000};

void generatePWM(int leftDutyCycle, int rightDutyCycle)
{
    // Set the duty cycle
    pwmConfig.dutyCycle = rightDutyCycle;
    pwmConfig2.dutyCycle = leftDutyCycle;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2); // left engine
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);  // right engine
}

void Initalise_Motor(void)
{
    /* Timer_A PWM Configuration Parameter */
    // Configure for the left motor
    /* Configuring P4.4 and P4.5 as Output. P2.4 as peripheral output for PWM and P1.1 for button interrupt */

    // This output allow motor to turn (clockwise direction in1->4.5 and in2->P4.4)
    // Set the P4.4 and 4.5 as out for the left wheel
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN5);

    // Set P4.4 as low and 4.5 as high for the left wheel to turn clockwise. High on In1 and Low on In2
    // To change the direction just put Low on In1 and High on In2 (Anticlockwise)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);  // In1
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); // In2

    //Set P4.0 and 4.2 as out for right wheel
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0); // In2
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN2); // In1

    //Set P4.0 and 4.2 as out for right wheel to move it clockwise High on In3 and Low on In4
    // (clockwise direction in3->4.2 and in4->P4.0)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);  // In4
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); // In3

    // Set the PSEL0 & PSEL1 multiplex and alternative pin function as timer function
    // Set peripheral module function in the output direction for PWM P2.4 for left wheel & 2.5 for right wheel as primary module function modes.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // Turn on the motor
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION); // Turn on the motor

    // Set the P1.1 & 1.4 as pull up resistor for button interrupt
    // P1.1 control PWM for left motor
    // P1.4 control PWM for right motor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);

    // Clear the interrupt flag and enable interrupt for P1.1
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Clear the interrupt flag and enable interrupt for P1.4
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    /* Configuring Timer_A to have a period of approximately 80ms and an initial duty cycle of 10% of that (1000 ticks)  */
    // // For left motor
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    // For right motor
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);

    /* Enabling global interrupts and starting the watchdog timer */
    Interrupt_enableInterrupt(INT_PORT1);
}

/* Move  forward by the grid
 * This method will move the car forward
 */
void moveForward(int tiles)
{
    // startTimer();
    pulsetarget = 40 * tiles;

    moveLeftFoward();
    moveRightFoward();
    //    while (grid_pulse_left <= target && grid_pulse_right <= target); // Delay based on the encounter tiles
//    volatile uint32_t i;
//    for (i = 0; i < pulsetarget; i++)
//    {
//        __delay_cycles(12000000);
//    }
//    stop();
}
/*Move backward
 * This method will move the car backward
 */
void moveBackward(int tiles)
{
    // startTimer();
    isTurning = 1;
    pulsetarget = 40 * tiles;

    moveLeftBackward();
    moveRightBackward();
    // __delay_cycles(3000000);
//    while (leftcount <= 20); // Delay based on the encoder count
    volatile uint32_t i;
    leftcount = 0;
    while (leftcount < 20);
    stop();
    isTurning = 0;
}

/* Move left forward
 * This method will move the left wheel of robotic car
 */
void moveLeftFoward()
{
    //Set P4.0 and 4.2 as out for right wheel to move it clockwise High on In3 and Low on In4
    // (clockwise direction in3->4.2 and in4->P4.0)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);  // In4
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); // In3
    pwmConfig2.dutyCycle = PWML;
    // printf("PWML: %d\n" , PWML);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
}
/* Move right forward
 * This method will move the left wheel of robotic car
 */
void moveRightFoward()
{
    // Set P4.4 as low and 4.5 as high for the left wheel to turn clockwise. High on In1 (4.5) and Low on In2 (4.4)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);  // In2
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); // In1
    pwmConfig.dutyCycle = PWMR;
    // printf("PWMR: %d\n" , PWMR);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

/* Move left Backward
 * This method will move the left wheel of robotic car backward
 */
void moveLeftBackward()
{
    //Set P4.0 and 4.2 as out for left wheel to move it anticlockwise Low on In3 and High on In4
    // (clockwise direction in1->4.0 and in2->P4.2)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);  // In3
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2); // In4
//    while (grid_pulse_left != 20)
//        ; // Delay based on the encoder count
    //The differences between the left motor and right motor PWM is about 1000
    // Left Motor
    pwmConfig2.dutyCycle = PWML;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
}

/* Move right Backward
 * This method will move the right wheel of robotic car backward
 */
void moveRightBackward()
{
    // Set P4.4 as low and 4.5 as high for the right wheel to turn anticlockwise.
    // To change the direction just put Low on In3 and High on In4 (Anticlockwise)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5); // In1
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4); // In2
    // Right Motor
    pwmConfig.dutyCycle = PWMR;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

/* Turn Right
 * This method will turn the robotic car to left
 */
void turnright()
{
    isTurning = 1;
//    startTimer();
    printf("leftcount: %d", leftcount);
    // // To turn left: You have to stop the right motor
     // Left Motor
     pwmConfig2.dutyCycle = PWML;
     Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);

    // // Right Motor
    // pwmConfig.dutyCycle = 0;
    // Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    // Set the left motor to move clockwise
    // Set P4.0 and 4.2 as out for left wheel to move it clockwise High on In3 and Low on In4
    // (clockwise direction in3->4.2 and in4->P4.0)
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);  // In4
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); // In3
    // Set P4.4 and P4.5
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);
    // Ensure it turn 90 degree to left by counting the encounter holes.
    // For some reason the leftcount dont work
    leftcount = 0;
    while (leftcount < 20);

//    __delay_cycles(6000000);
    stop();
    isTurning = 0;
    
}

/* Turn Left
 * This method will turn the robotic to the right
 */
void turnleft()
{
    isTurning = 1;
//    startTimer();
    // To turn right: You have to stop the left motor
    //    pwmConfig2.dutyCycle = 0;
    //    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
    //
//    pwmConfig.dutyCycle = PWMR;
//    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);  // In4
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);  // In3
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5); // In2
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);  // In1
    rightcount = 0;
    // Ensure it turn 90 degree to right by counting the encounter holes.
    while (rightcount < 20);
    // __delay_cycles(6000000);
    stop();
    isTurning = 0;
}

/* Stop
 * This method will stop the robotic car.
 */
void stop()
{
    stopTimer();
    // Stop the left motor
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
    // Stop the right motor
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
    leftcount = 0;
    rightcount = 0;

}


void rotate() {
    isTurning = 1;
    int a;
    for (a=0; a<=3; ++a) {
        turnright();
    }
}


/* Check Command
 * This function is to check command when data is been recieved from ESP8266
 */
void checkCommand(char *command)
{
    if (strcmp(command, "W") == 0)
    {
        MSPrintf(EUSCI_A0_BASE, "FORWARD!\r\n\r\n");
        moveForward(1);
    }
    else if (strcmp(command, "A") == 0)
    {
        MSPrintf(EUSCI_A0_BASE, "LEFT!\r\n\r\n");
        turnleft();
    }
    else if (strcmp(command, "S") == 0)
    {
        MSPrintf(EUSCI_A0_BASE, "BACKWARD!\r\n\r\n");
        moveBackward(1);
    }
    else if (strcmp(command, "D") == 0)
    {
        MSPrintf(EUSCI_A0_BASE, "RIGHT!\r\n\r\n");
        turnright();
    }
    else if (strcmp(command, "R") == 0)
    {
        MSPrintf(EUSCI_A0_BASE, "ROTATE!\r\n\r\n");
        rotate();
    }
}





/* Port1 ISR - This ISR will progressively step up the duty cycle of the PWM on a button press */
void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    if (status & GPIO_PIN1)
    {
        //Left Motor
        //    if(pwmConfig2.dutyCycle == 9000) {
        //        pwmConfig2.dutyCycle = 0;
        //        stop();
        //    }

        //    else {
        //        pwmConfig2.dutyCycle += 1000;
        //        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig2);
        //        startTimer();
        //    }
        stop();
    }

    if (status & GPIO_PIN4)
    {
        // Right Motor
        //        if(pwmConfig.dutyCycle == 9000) {
        //            pwmConfig.dutyCycle = 0;
        //            stop();
        //        }
        //        else {
        //            pwmConfig.dutyCycle += 1000;
        //            Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
        //        }
        moveForward(5);
    }
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

}

// Set the PID of the car to the desired value
void CarPID(int target)
{

    int lastErrorLeft = 0;
    int lastErrorRight = 0;
    //    int totalErrorLeft =0;
    //    int totalErrorRight =0;
    // Set the target count
    int targetCount = target;
    // Calculate the Porportional Control
    float Kp = 0.03; // Proportional gain
    // Get the error for left and right enconder
    //    printf("LeftCount %d\n" , leftcount);
    //    printf("RightCount %d\n" , rightcount);
    int errorLeft = targetCount - leftcount;
    int errorRight = targetCount - rightcount;
    //    printf("errorLeft1 %d\n" , errorLeft);
    //    printf("errorRight1 %d\n" , errorRight);
    leftcount = 0;
    rightcount = 0;
    if ((errorLeft < 0 || errorLeft > 3) || (errorRight < 0 || errorRight > 3))
    {
        float PoutLeft = Kp * errorLeft;
        float PoutRight = Kp * errorRight;
        // Ensure the KP is within the range of 0 to 1 for POUTLeft and PoutRight
        if (PoutLeft > 1)
        {
            PoutLeft = 1;
        }
        else if (PoutLeft < 0)
        {
            PoutLeft = 0;
        }
        if (PoutRight > 1)
        {
            PoutRight = 1;
        }
        else if (PoutRight < 0)
        {
            PoutRight = 0;
        }
        // Debugging Purpose
        //            printf("errorLeft2 %d\n" , errorLeft);
        //            printf("errorRight2 %d\n" , errorRight);
        //    printf("PoutLeft %.2f\n" , PoutLeft);
        //    printf("PoutRight %.2f\n" , PoutRight);

        // Calculate the Derivative Control
        float Kd = 0.15; // Derivative gain
        float DoutLeft = Kd * (errorLeft - lastErrorLeft);
        float DoutRight = Kd * (errorRight - lastErrorRight);
        //Debugging Purpose
        //printf("DoutLeft %.2f\n" , DoutLeft);
        //printf("DoutRight %.2f\n" , DoutRight);

        // Update the last error
        lastErrorLeft = errorLeft;
        lastErrorRight = errorRight;

        // Calculate the Integral Control
        float Ki = 0.005; // Integral gain
        float IoutLeft = Ki * totalErrorLeft;
        float IoutRight = Ki * totalErrorRight;
        //Debugging Purpose
        //printf("IoutLeft %.2f\n" , PoutLeft);
        //printf("IoutRight %.2f\n" , PoutRight);
        // Add all the error together
        totalErrorLeft += errorLeft;
        totalErrorRight += errorRight;

        // Calculate the PID output
        leftPIDOutput = PoutLeft + DoutLeft + IoutLeft;
        rightPIDOutput = PoutRight + DoutRight + IoutRight;

        // Debugging Purpose
        // printf("leftPIDOutput %.2f\n" , leftPIDOutput);
        // printf("rightPIDOutput %.2f\n" , rightPIDOutput);

        //Ensure the PID output is within the range of 0 to 1
        if (leftPIDOutput > 1)
        {
            leftPIDOutput = 1;
        }
        else if (leftPIDOutput < -1)
        {
            leftPIDOutput = 0;
        }
        if (rightPIDOutput > 1)
        {
            rightPIDOutput = 1;
        }
        else if (rightPIDOutput < -1)
        {
            rightPIDOutput = 0;
        }

        PWML = (1 + leftPIDOutput) * speedL;
        PWMR = (1 + rightPIDOutput) * speedR;

        // Set the PWM for the left and right motor
        // Ensure that dutycycle does not go too high or too low for the motor
        // If both motor is slower than the target count, then the motor will increase PWM
        if (pwmConfig.dutyCycle <= 10000 && pwmConfig2.dutyCycle <= 10000)
        {
            pwmConfig.dutyCycle += PWML;
            pwmConfig2.dutyCycle += PWMR;
        }
        else if (pwmConfig.dutyCycle > 10000 && pwmConfig2.dutyCycle < 10000)
        {
            pwmConfig.dutyCycle = speedL;
            pwmConfig2.dutyCycle += (PWMR - PWML);
        }
        else if (pwmConfig.dutyCycle < 10000 && pwmConfig2.dutyCycle > 10000)
        {
            pwmConfig.dutyCycle += (PWML - PWMR);
            pwmConfig2.dutyCycle = speedR;
        }
        else if (pwmConfig.dutyCycle > 10000 && pwmConfig2.dutyCycle > 10000)
        {
            pwmConfig.dutyCycle -= PWML;
            pwmConfig2.dutyCycle -= PWMR;
        }

        if (pwmConfig2.dutyCycle + leftPIDOutput > 9000)
        {
            pwmConfig2.dutyCycle = 9000;
        }
        else if (pwmConfig2.dutyCycle + leftPIDOutput < 0)
        {
            pwmConfig2.dutyCycle = 0;
        }
        else
        {
            pwmConfig2.dutyCycle += leftPIDOutput;
        }

        // Debugging Purpose
        //            printf("PWMLeftInput %d\n" , PWML);
        //            printf("PWMRightInput %d\n" , PWMR);
        // Set the PWM for the left and right motor
        generatePWM(PWML, PWMR);
    }
    else
    {
        generatePWM(PWML, PWMR);
    }
}
