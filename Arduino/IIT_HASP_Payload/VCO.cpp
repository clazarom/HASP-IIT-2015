#include "VCO.h"

/*********************************************/
/************ VCO (Communications) ***********/
/*********************************************/
/**                                         **/
/**               660 OHM                   **/
/**  ARDUINO o-----/\/\/----.-----o VCO IN  **/
/**                         |               **/
/**                       _____             **/
/**                       _____ 100 uF      **/
/**                         |               **/
/**                         |               **/
/**                        GND              **/
/**                                         **/
/*********************************************/


/* Current analog output on VCO pin */
int VCO_current_Analog_Output = VCO_ANALOG_OUTPUT_446MHz;      

/* Boundaries of the VCO input */
int VCO_max_Analog_Output = VCO_ANALOG_OUTPUT_450MHz;              
int VCO_min_Analog_Output = VCO_ANALOG_OUTPUT_446MHz;

/* Time control variables */
unsigned long VCO_time = 0;                                        
unsigned long VCO_lastTime = 0;

/* VCO frequency */
int VCO_freq;

/* VCO frequency state (for Autotune mode) */
int VCO_state;

/* VCO state (ON/OFF) */
int VCO_onOff;

/* VCO autotune state (ON/OFF) */
bool VCO_autotune;


/**
 * Setup function for VCO.
 */
void VCO_setup(){
  
    /* Set Autotune false */
    VCO_autotune = false;
    
    /* Setup VCO */
    VCO_state = 0;
    VCO_freq = 446;
    VCO_current_Analog_Output = VCO_ANALOG_OUTPUT_446MHz;
    analogWrite(VCO_ANALOG_PIN,VCO_current_Analog_Output);
    
    /* Power VCO up */
    VCO_switch();
}


/**
 * Set the analog output for the VCO
 * 
 * @param out The value (analog output) to set.
 */
void VCO_setAnalogOutput( int out ){

    if( out >= VCO_max_Analog_Output )                              /* Check if the value is within boundaries */
        out = VCO_max_Analog_Output;
    if( out <= VCO_min_Analog_Output )
        out = VCO_min_Analog_Output;

    VCO_current_Analog_Output = out;
    analogWrite(VCO_ANALOG_PIN,VCO_current_Analog_Output);          /* Refresh the analog output */
}


/**
 * Modify the boundaries of the VCO input signal
 *
 * @param maxVCOValue The high boundary to be set. No change if this is 0.
 * @param minVCOValue The low boundary to be set. No change if this is 0.
 */
void VCO_setBoundaries( uint8_t maxVCOValue, uint8_t minVCOValue ){
    
    uint8_t mx = maxVCOValue;
    uint8_t mn = minVCOValue;
  
    /* Set high boundary. Leave unchanged if parameter is 0 */
    if( mx =! 0 ){
        VCO_max_Analog_Output = mx;
    }
    
    /* Set low boundary. Leave unchanged if parameter is 0 */
    if( mn =! 0 ){
        VCO_min_Analog_Output = mn;
    }
}



/**
 * Change VCO to next state (i.e. next frequency)
 */
void VCO_nextState(){

    if( VCO_state <5 ) VCO_state ++;
    else VCO_state = 0;

    switch( VCO_state ){
        case 0:
            VCO_freq = 446;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_446MHz);
            break;
        case 1:
            VCO_freq = 446;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_446MHz);
            break;
        case 2:
            VCO_freq = 447;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_447MHz);
            break;
        case 3:
            VCO_freq = 448;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_448MHz);
            break;
        case 4:
            VCO_freq = 449;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_449MHz);
            break;
        case 5:
            VCO_freq = 450;
            VCO_setAnalogOutput(VCO_ANALOG_OUTPUT_450MHz);
            break;
    }
}


/**
 * Switch VCO On/Off.
 */
void VCO_switch(){
    
    if( VCO_onOff ){
        VCO_autotune = false;
        VCO_onOff = 0;
        digitalWrite(CURRENTSENSOR2_PIN, LOW);     /* Switch off the current supply */
    } else {
        VCO_onOff = 1;
        digitalWrite(CURRENTSENSOR2_PIN, HIGH);    /* Switch on the current supply */
    }
}




/*
 * Getters
 */
int VCO_getAnalogOutput(){ return VCO_current_Analog_Output; }
int VCO_getMaxAnalogOutput(){ return VCO_max_Analog_Output; }
int VCO_getMinAnalogOutput(){ return VCO_min_Analog_Output; }
bool VCO_getAutotune(){ return VCO_autotune; }
int VCO_getOnOff(){ return VCO_onOff; }

/*
 * Setters
 */
void VCO_setAutotune( bool autotune ){ VCO_autotune = autotune; }
