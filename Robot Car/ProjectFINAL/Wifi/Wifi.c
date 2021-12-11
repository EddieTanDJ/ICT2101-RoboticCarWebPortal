/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * Description: A ESP8266 module that serves as a communication module .
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST          P3.2 |-> Wifi Tx
 *            |             P3.3 |<- Wifi Rx
 *            |     P1.3/UCA0TXD |-> PC
 *            |     P1.2/UCA0RXD |<- PC
 *            |                  |
 *            |                  |
 * Author: Joey
 *******************************************************************************/
#include "Wifi/wifi.h"

eUSCI_UART_ConfigV1 UART0Config = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK, 13, 0, 37,
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION };

eUSCI_UART_ConfigV1 UART2Config = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK, 13, 0, 37,
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION };

void Delay(uint32_t loop)
{
    volatile uint32_t i;
    for (i = 0; i < loop; i++);
}

void setup_connection()
{
    // Try to establish TCP connection to a HTTP server
    if (!ESP8266_EstablishConnection('0', TCP, HTTP_WEBPAGE, PORT))
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to connect to: %s\r\nERROR: %s\r\n",
        HTTP_WEBPAGE, ESP8266_Data);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        // retry the connection
        while (1)
        {
            if (ESP8266_EstablishConnection('0', TCP, HTTP_WEBPAGE, PORT))
            {
                break;
            }
            Delay(100000);
        };
    }
    // Indication that the robotic car is successfully connected to web portal
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
    MSPrintf(EUSCI_A0_BASE, "TCP connection Establish \n\r");
}

void get_api(char *api, uint32_t buffer_size)
{
    //  Query data to connected HTTP server, look for an API and request a key
    if (!ESP8266_SendData('0', api, buffer_size))
    {
        MSPrintf(EUSCI_A0_BASE,
                 "Failed to send: %s to %s \r\n\r\nError: %s\r\n\r\n", api,
                 HTTP_WEBPAGE,
                 ESP8266_Data);
        //  MSPrintf(EUSCI_A0_BASE, "Please retry establishing connection.");
        while (1)
        {
            if (ESP8266_SendData('0', api, buffer_size))
            {
                break;
            }
            Delay(1000);
        };
    }
    MSPrintf(EUSCI_A0_BASE,
             "Data sent: %s to %s \r\n\r\n ESP8266 Data Received: %s \r\n\r\n",
             api, HTTP_WEBPAGE, ESP8266_Data);
}

void initiate_wifi()
{
    // Ensure MSP432 is Running at 24 MHz
    CS_Init();

    // Initialize required hardware peripherals for the ESP8266
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(EUSCI_A0_BASE, &UART0Config);
    MAP_UART_enableModule(EUSCI_A0_BASE);
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(EUSCI_A2_BASE, &UART2Config);
    MAP_UART_enableModule(EUSCI_A2_BASE);
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);

    //  Reset GPIO of the ESP8266
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);
    MAP_Interrupt_enableMaster();

    //  Hard Reset ESP8266
    ESP8266_HardReset();
    __delay_cycles(48000000);

    //  Flush reset data, we do this because a lot of data received cannot be printed
    UART_Flush(EUSCI_A2_BASE);
    MSPrintf(EUSCI_A0_BASE, "Hard Reset\n\r");

    //  Pointer to ESP8266 global buffer
    ESP8266_Data = ESP8266_GetBuffer();

    //  Check UART connection to MSP432
    if (!ESP8266_CheckConnection())
    {
        MSPrintf(EUSCI_A0_BASE, "Failed MSP432 UART connection\r\n");
        //  Trap MSP432 if failed connection
        while (1);
    }
    MSPrintf(EUSCI_A0_BASE, "Nice! We are connected to the MSP432\r\n\r\n");
    Delay(1000);

    //  Connect to Access Point
    if (!ESP8266_ConnectToAP(SSID, PASSWORD))
    {
        MSPrintf(EUSCI_A0_BASE, "Waiting for connection\r\n\r\n");
        while (1)
        {
            if (ESP8266_ConnectToAP(SSID, PASSWORD))
            {
                break;
            }
            Delay(1000);
        };
    }
    MSPrintf(EUSCI_A0_BASE, "Connected to access point: %s \r\n\r\n", SSID);

    if (!ESP8266_EnableMultipleConnections(true))
    {
        MSPrintf(EUSCI_A0_BASE, "Failed to set multiple connections\r\n");
        // Red color means the WIFI is not connected
        // Green color means the WIFI is connected
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
        while (1)
        {
            if (ESP8266_EnableMultipleConnections(true))
            {
                break;
            }
            Delay(1000);
        };
    }
    MSPrintf(EUSCI_A0_BASE, "Multiple Connections set\n\r");
}

char *parseData()
{
    char *ESP8266_Data = ESP8266_GetBuffer();
    int i, indexOfColon, payloadLength;
    char *command;

    MSPrintf(EUSCI_A0_BASE, "The ESP8266 data is %s\r\n", ESP8266_Data);

    //  Get the paylaod
    for (i = 0; ESP8266_Data[i] != '\0'; i++)
    {
        if (ESP8266_Data[i] == ':')
        {
            indexOfColon = i;
            break;
        }
    }

    //  Get the payload length
    payloadLength = atoi(&ESP8266_Data[indexOfColon - 1]);
    MSPrintf(EUSCI_A0_BASE, "%i\r\n", payloadLength);

    //  Dynamic Allocate Memory for char Array: command
    command = malloc(sizeof(char) * (payloadLength + 1));

    //  Extract each of the character to char* command
    for (i = 0; i < payloadLength; i++)
    {
        command[i] = ESP8266_Data[indexOfColon + 1 + i];
    }

    //  "\0" to indicate the end of string
    command[payloadLength] = '\0';
    MSPrintf(EUSCI_A0_BASE, "The command is : %s\r\n", command);
    return command;
}
