#ifndef HACKHD_H
#define HACKHD_H


/*********************************/
/************ INCLUDES ***********/
/*********************************/

#include "HASP_Code.h"



/*******************************/
/********** FUNCTIONS **********/
/*******************************/

void HackHD_setup();
void HackHD_setStandby();
void HackHD_startStop();
void HackHD_startStopRecordTenMin();
void HackHD_turnOff();
int HackHD_getHackHDOn();
bool HackHD_getHackHDRecordTenMin();

#endif
