#ifndef COMM_H
#define COMM_H


/*********************************/
/************ INCLUDES ***********/
/*********************************/

#include "HASP_Code.h"
#include "VCO.h"
#include "HackHD.h"
#include <SoftwareSerial.h>

/************************************/
/************* DEFINES **************/
/************************************/

#define PIN_TX_SERIAL2       31
#define SERIAL_BAUDRATE    9600
#define SERIAL2_BAUDRATE   1200




/*******************************/
/********** FUNCTIONS **********/
/*******************************/

void COMM_setup();
void COMM_sendSensors(unsigned long* sensorArray, unsigned long time);
void COMM_readSerial();

#endif
