
/******************************************************************************
 * ICT 2104 Robotic Car
 *
 * Description: Robot car control from web portal.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|            P5.0  | -> Line encoder
 *          | |            P5.2  | -> Right encoder
 *          --|RST         P6.4  | -> Left Line Tracker
 *            |            P6.5  | -> Right Line Tracker
 *            |            P3.6  | -> Trigger Ultrasonic Sensor
 *            |            P3.7  | <- Echo Ultrasonic Sensor
 *            |            P2.4  | >- Right Motor
 *
 *            |            P2.5  | >- Left Motor
 *            |            P2.6  | >- Right Motor
 * 
 * PORT1_IRQHandler() - Button interrupt handler
 * PORT3_IRQHandler() - Ultrasonic interrupt handler
 * PORT4_IRQHandler() - Line tracker interrupt handler
 * PORT5_IRQHandler() - Encoder interrupt handler
 * 
 * SPLITTER
 * BLUE - GROUND
 * 1. Left Line Tracker
 * 2. Ultrasonic Sensor
 * 3. Right Line Tracker
 * 
 * YELLOW - 3V3
 * 1. WIFI
 * 2. Ultrasonic Sensor
 * 3. WIFI
 *
 * RED - 3V3
 * 1. Left Encoder
 * 2. Right Encoder
 * 3. Right Line Tracker
 * 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>


// Global variables for the encoders
uint32_t leftcount = 0;
uint32_t rightcount = 0;
uint32_t PWML = 15000;
uint32_t PWMR = 15000;
uint32_t grid_pulse_left = 0;
uint32_t grid_pulse_right = 0;
//float distance = 0;
uint32_t countTrigger = 0;
uint32_t pulseTrigger = 0;
uint32_t speedL = 5000;
uint32_t speedR = 5000;
float speed = 0;
float totalDistance = 0;

float leftPIDOutput;
float rightPIDOutput;
uint32_t pulseCountL = 0;
uint32_t pulseCountR = 0;
bool isTurning;
uint32_t totalErrorLeft = 0;
uint32_t totalErrorRight = 0;

// For WIFI Pooling Status
bool pooling = false;
bool blackTiles = false;
bool obstacle = false;

// For HTTP REQUEST
//  Data that will be sent to the HTTP server.
//  This will allow us to query car commands.
char http_req_getfirstcommands[] = "GET http://172.18.16.175/api/commands/getFirstCommands\r\n\r\n";
char http_req_dequeue[] = "GET http://172.18.16.175/api/commands/dequeueCar\r\n\r\n";
char http_req_feedback_black[] = "GET http://172.18.16.175/api/data/feedback?feedback=black\r\n\r\n";
char http_req_feedback_obstacle[] = "GET http://172.18.16.175/api/data/feedback?feedback=obstacle\r\n\r\n";
char http_req_feedback_black_obstacle[] = "GET http://172.18.16.175/api/data/feedback?feedback=blackobstacle\r\n\r\n";
char http_req_getallcommands[] = "GET http://172.18.16.175/api/commands/getAllCommands\r\n\r\n";

/*Subtract one to take account for the null character*/
uint32_t http_req_size_getfirstcommand = sizeof(http_req_getfirstcommands) - 1;
uint32_t http_req_size_dequeue = sizeof(http_req_dequeue) - 1;
uint32_t http_req_size_feedback_black = sizeof(http_req_feedback_black) - 1;
uint32_t http_req_size_feedback_obstacle = sizeof(http_req_feedback_obstacle) - 1;
uint32_t http_req_size_feedback_black_obstacle = sizeof(http_req_feedback_black_obstacle) - 1;
uint32_t http_req_size_getallcommands = sizeof(http_req_getallcommands) - 1;


// For commands
char* commands_buffer;

// For PID Stuff


//float dist = 0;
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <Sensor/encoder.h>
#include <Sensor/Ultrasonic.h>
#include <Sensor/init_Ultrasonic.h>
#include <Sensor/Linetracker.h>
#include <Motor/Motor.h>
#include <Wifi/Wifi.h>


char* request() {
    char http_req_sendtest[] = "GET http://172.18.16.175/api/data/feedback?speed=%.2f\r\n\r\n";
    // append to append to first argument
    sprintf(http_req_sendtest, http_req_sendtest, speed);
    return http_req_sendtest;
}


uint32_t main(void)
 {
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();
    // For PIN Toggling
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
    // Now can work when connect to PC, but cannot work when only used battery
    init_line_tracker();
    // Initalise 24MHZ
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
    Initalise_HCSR04_New();
    Initalise_Enconder();
    startTimer();
    Initalise_Motor();
    char http_req_speeddistance[100] = request();
    printf("%s", http_req_speeddistance);


    Interrupt_enableMaster();
    // Inititate ESP8266
    initiate_wifi();

    while(1)
    {
        __delay_cycles(48000000);
        __delay_cycles(48000000);
        // Receive data from the web server
        if (blackTiles == true && obstacle == true) {
            // Estblish TCP connection to the web server
            // Clear InterruptFlag on P6.4 AND P6.5
            MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
            MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5);
            // Enable Port P6.4 AND P6.5 as Interrupt
            MAP_GPIO_disableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
            MAP_GPIO_disableInterrupt(GPIO_PORT_P6, GPIO_PIN5);
            setup_connection();
            Delay(100000);
            get_api(http_req_feedback_black_obstacle, http_req_size_feedback_black_obstacle);
            Delay(100000);
            char *command = parseData();
            Delay(100000);
            checkCommand(command);
            __delay_cycles(24000000);
            obstacle = false;
            blackTiles = false;
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            // Clear InterruptFlag on P6.4 AND P6.5
             MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
             MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5);
             // Enable Port P6.4 AND P6.5 as Interrupt
             MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
             MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN5);

        }
        else if (obstacle == true) {
            // Estblish TCP connection to the web server
            setup_connection();
            Delay(100000);
            get_api(http_req_feedback_obstacle, http_req_size_feedback_obstacle);
            Delay(100000);
            obstacle = false;
        }
        else if(blackTiles == true) {
            // Clear InterruptFlag on P6.4 AND P6.5
            MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
            MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5);
            // Enable Port P6.4 AND P6.5 as Interrupt
            MAP_GPIO_disableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
            MAP_GPIO_disableInterrupt(GPIO_PORT_P6, GPIO_PIN5);
            setup_connection();
            Delay(100000);
            get_api(http_req_feedback_black, http_req_size_feedback_black);
            Delay(100000);
            char *command = parseData();
            Delay(100000);
            checkCommand(command);
            __delay_cycles(24000000);
            blackTiles = false;
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            // Clear InterruptFlag on P6.4 AND P6.5
             MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN4);
             MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN5);
             // Enable Port P6.4 AND P6.5 as Interrupt
             MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN4);
             MAP_GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN5);
        }
    }
}







