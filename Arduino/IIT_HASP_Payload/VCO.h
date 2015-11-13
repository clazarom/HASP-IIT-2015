#ifndef VCO_H
#define VCO_H


/*********************************/
/************ INCLUDES ***********/
/*********************************/

#include "HASP_Code.h"




/*********************************/
/************ DEFINES ************/
/*********************************/

#define VCO_ANALOG_OUTPUT_450MHz 212    /* Analog output constant to obtain 450 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_449MHz 206    /* Analog output constant to obtain 449 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_448MHz 200    /* Analog output constant to obtain 448 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_447MHz 194    /* Analog output constant to obtain 447 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_446MHz 188    /* Analog output constant to obtain 446 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_445MHz 182    /* Analog output constant to obtain 445 MHz on VCO output */
#define VCO_ANALOG_OUTPUT_443MHz 170    /* Analog output constant to obtain 443 MHz on VCO output */

#define VCO_SIGNALPERIOD_MS 50000       /* Analog output signal change period: every 5 min */
#define VCO_OUPTUT_VALUE_SHIFT  2       /* Value to add to the analog output in each period */




/***********************************/
/************ FUNCTIONS ************/
/***********************************/

void VCO_setup();
void VCO_setAnalogOutput( int out );
void VCO_setBoundaries( uint8_t maxVCOValue, uint8_t minVCOValue );
void VCO_nextState();
void VCO_switch();
int VCO_getAnalogOutput();
int VCO_getMaxAnalogOutput();
int VCO_getMinAnalogOutput();
bool VCO_getAutotune();
int VCO_getOnOff();
void VCO_setAutotune( bool autotune );


#endif
