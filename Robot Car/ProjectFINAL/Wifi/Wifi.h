///*
// * wifi.h
// *
// *  Created on: 27 Nov 2021
// *      Author: Eddie
// */
//
#ifndef WIFI_WIFI_H_
#define WIFI_WIFI_H_

/* DriverLib Includes */
#include <Hardware/CS_Driver.h>
#include <Devices/ESP8266.h>
#include <stdio.h>
#include <stdlib.h>


// Configure your WIFI ssid, password and http webpage, port here
#define SSID            "SINGTEL-81A6"
#define PASSWORD        "0016482726"
#define HTTP_WEBPAGE  "192.168.1.75"
#define PORT          "5000"

#define BUFFER_SIZE 100

/*Pointer to ESP8266 global buffer*/
char *ESP8266_Data;


void Delay(uint32_t loop);
void get_apis();
void get_api(char *api, uint32_t buffer_size);
void setup_connection();
void initiate_wifi();
char* parseData();

#endif /* WIFI_WIFI_H_ */

