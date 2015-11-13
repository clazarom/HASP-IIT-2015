#ifndef TASKS_H
#define TASKS_H


/*********************************/
/************ INCLUDES ***********/
/*********************************/

#include "Comm.h"



/************************************/
/************* DEFINES **************/
/************************************/

#define NUMBER_OF_TASKS               9

#define TASK_TEMP1                    0
#define TASK_HUMIDITY_TEMP2           1
#define TASK_PRESSURE                 2
#define TASK_LIGHT                    3
#define TASK_CURRENT1                 4
#define TASK_CURRENT2                 5
#define TASK_SENDSENSORS              6
#define TASK_RECORDTENMIN             7
#define TASK_VCOAUTOTUNE              8

#define TASKPERIOD_TEMP1              1000
#define TASKPERIOD_HUMIDITY_TEMP2     1000
#define TASKPERIOD_PRESSURE           1000
#define TASKPERIOD_LIGHT              1000
#define TASKPERIOD_CURRENT1           2000
#define TASKPERIOD_CURRENT2           2000
#define TASKPERIOD_SENDSENSORS        1000
#define TASKPERIOD_RECORDTENMIN     600000
#define TASKPERIOD_VCOAUTOTUNE        1000



/***************************************/
/********** VARIABLES/OBJECTS **********/
/***************************************/




/*******************************/
/********** FUNCTIONS **********/
/*******************************/

void checkTasks();

#endif
