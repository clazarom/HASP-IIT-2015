#include "HackHD.h"

/* Pin for camera control */
int cameraPin = 51;

/* Camera on/off indicator */
int HackHD_on;

/* Indicates whether HackHD is in Standby mode or not */
bool HackHD_standBy;

/* Flag to start 10 minutes recording */
bool HackHD_recordTenMin = false;

/**
 * Hack HD setup function.
 */
void HackHD_setup(){
  
    /* Setup Camera as Off */
    pinMode(cameraPin, INPUT);
    HackHD_on = 0;
    HackHD_standBy = false;
}



/* 
 * Pushes the HackHD button for t milliseconds. 
 * 
 * @param t Amount of time in milliseconds that the button is held down.
 */
void HackHD_pushButton( int t ) {
    pinMode(cameraPin, OUTPUT);
    delay( t );
    pinMode(cameraPin, INPUT);
}



/**
 * Set/Exits camera in/from StandBy mode.
 */
void HackHD_setStandby(){

    /* Execute only when the camera is not in Automatic mode */
    if( !HackHD_standBy && HackHD_on ) return;
    HackHD_pushButton( 3000 );
    HackHD_standBy = ~HackHD_standBy;
    HackHD_on = ~HackHD_on;
}



/**
 * If HackHD is in StandBy mode: Takes a picture.
 * If HackHD is off: Starts automatic mode.
 * If HackHD is in Automatic mode: Turns off camera.
 */
void HackHD_startStop(){
    HackHD_pushButton( 500 );
    if( !HackHD_standBy ) HackHD_on = ~HackHD_on;
    if( !HackHD_on ) HackHD_recordTenMin = false;
}



/**
 * If HackHD is in StandBy mode: Takes a picture.
 * If HackHD is off: Starts automatic mode.
 * If HackHD is in Automatic mode: Turns off camera.
 */
void HackHD_startStopRecordTenMin(){

    /* If camera is Off, start recording */
    if( !HackHD_on ){

        /* Start taking pictures in Automatic mode */
        HackHD_startStop();
    
        /* Flag up */
        HackHD_recordTenMin = true;

    }

    /* Otherwise, if the HackHD is not in Standby mode, stop recording */
    else if( !HackHD_standBy ){
      
        /* Stop taking pictures */
        HackHD_startStop();
    
        /* Flag down */
        HackHD_recordTenMin = false;
    }
}



/**
 * Turn camera off.
 */
void HackHD_turnOff(){
    HackHD_pushButton( 6000 );
    HackHD_on = 0;
    HackHD_standBy = false;
    HackHD_recordTenMin = false;
}


/*
 * Getters.
 */
int HackHD_getHackHDOn(){ return HackHD_on; }
bool HackHD_getHackHDRecordTenMin(){ return HackHD_recordTenMin; }

