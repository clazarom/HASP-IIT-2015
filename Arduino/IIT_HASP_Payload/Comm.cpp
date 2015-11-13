#include "Comm.h"

/*********************************************/
/**          Communications Code            **/
/**                                         **/
/**  NASA COMM: Serial2                     **/
/**  PAYLOAD COMM: Serial3                  **/
/**                                         **/
/*********************************************/


/* Array to store the incoming commands */
uint8_t inString[7]="      ";

/**
 * Setup function.
 */
void COMM_setup(){
  
    /* Setup Communicaton with NASA */
    Serial2.begin(SERIAL2_BAUDRATE);
    pinMode(PIN_TX_SERIAL2, OUTPUT);
    digitalWrite(PIN_TX_SERIAL2, HIGH);
}


/**
 * Send sensors information through NASA COMM link
 *
 * The format of the package sent is:
 * <Head><Temp1><Temp2><Humidity><Pressure><Light><Camera><A1><A2><fcom><Modcom><Time><Checksum><Footer>
 * Initial flag: !
 * Checksum: XOR function
 * End with: carry return
 *
 * @param sensorArray Array where the sensors info is stored.
 */
void COMM_sendSensors(unsigned long* sensorArray, unsigned long time) {
  
    int i;
    uint8_t checksum = 0;
  
    /* Send Initial flag: '!' */
    Serial2.print("!");
    checksum = checksum ^ int8_t('!');

    /* Write other sensors values */
    for( i=0 ; i<NUMBER_OF_SENSORS-2 ; i++ ){

        Serial2.write(lowByte(sensorArray[i]));
        checksum = checksum ^ lowByte(sensorArray[i]);
        
        Serial2.write(lowByte(sensorArray[i]>>8));
        checksum = checksum ^ lowByte(sensorArray[i]>>8);
    }

    
    /* Send Camera status (ON/OFF) */
    uint8_t camStatus = HackHD_getHackHDOn();
    Serial2.write(camStatus);
    checksum = checksum ^ uint8_t(camStatus);

    /* Send VCO minimum Analog Output */
    int VCOminOut = VCO_getMinAnalogOutput();
    Serial2.write(VCOminOut);
    checksum = checksum ^ uint8_t(VCOminOut);
    
    /* Send Current Sensors values */
    for( i = NUMBER_OF_SENSORS-2 ; i<NUMBER_OF_SENSORS ; i++ ){
      
        Serial2.write(lowByte(sensorArray[i]));
        checksum = checksum ^ lowByte(sensorArray[i]);
        
        Serial2.write(lowByte(sensorArray[i]>>8));
        checksum = checksum ^ lowByte(sensorArray[i]>>8);
    }
    
    
    /* Send VCO analog output */
    int VCOout = VCO_getAnalogOutput();
    Serial2.write(VCOout);
    checksum = checksum ^ (VCOout);

    /* Send VCO state (ON/OFF) */
    int VCOon = VCO_getOnOff();
    Serial2.write(int8_t(VCOon));
    checksum = checksum ^ (int8_t(VCOon));
    
    /* Send time information */
    Serial2.write(lowByte(time));
    checksum = checksum ^ lowByte(time);
    
    Serial2.write(lowByte(time>>8));
    checksum = checksum ^ lowByte(time>>8);
    
    Serial2.write(lowByte(time>>16));
    checksum = checksum ^ lowByte(time>>16);
    
    Serial2.write(lowByte(time>>24));
    checksum = checksum ^ lowByte(time>>24);
    
    /* Send checksum */
    Serial2.write(checksum);
  
    /* Prints the return carriage */
    Serial2.println();
  
    /* Reset the port */
    Serial2.flush();
}




/**
 * Read from serial port
 * SERIAL - Serial2
 *
 * List of commands (hex value):
 *   00 00 - Reset Arduino
 *   AA FF - Start Recording
 *   AA 00 - Stop Recording
 *   AA 11 - Record for 10 min
 *   11 11 - Next VCO state
 *   11 22 - Switch VCO autotune
 *   11 00 - VCO OFF
 *   11 FF - VCO ON
 *   20 XX - Set VCO frequency upper boundary
 *   21 XX - Set VCO frequency lower boundary
 *   
 */
void COMM_readSerial(){
  
    while (Serial2.available()>0) {
      
        /* Collect all 7 bytes in inString */
        for(int i=0;i<6;i++) { inString[i] = inString[i+1]; }
        inString[6]=Serial2.read();
    
        /* Then, we analyze the bytes:  */
        /* Byte 0: 0x1                  */
        /* Byte 1: 0x2                  */
        /* Byte 2: command byte 1       */
        /* Byte 3: command byte 2       */
        /* Byte 4: 0x3                  */
        /* Byte 5: 0xD                  */
        /* Byte 6: 0xA                  */

        /* First we check that Bytes 0, 1, 4, 5 and 6 are correct */
        if( inString[0] == 0x01 && 
            inString[1] == 0x02 && 
            inString[4] == 0x03 && 
            inString[5] == 0x0D && 
            inString[6] == 0x0A ){

            /* Extract the command bytes */
            uint16_t command = (inString[2] << 8) + inString[3];

            /* Identify them */
            switch( command ){

                /* 0x0000: Reset Arduino */
                case 0x0000:
                    HackHD_turnOff();                   /* Turn off camera */
                    digitalWrite(CURRENTSENSOR2_PIN, LOW);     /* Turn off VCO */
                    break;

                /* 0xAAFF: Start recording */
                case 0xAAFF:
                    if( !HackHD_getHackHDOn() )
                        HackHD_startStop();
                    break;

                /* 0xAA00: Stop recording */
                case 0xAA00:
                    if( HackHD_getHackHDOn() )
                        HackHD_startStop();
                    break;

                /* 0xAA11: Record for 10 minutes */
                case 0xAA11:
                    if( !HackHD_getHackHDRecordTenMin() )
                        HackHD_startStopRecordTenMin();
                    break;

                /* 0x1111: Next VCO state */
                case 0x1111:
                    VCO_setAutotune(false);
                    VCO_nextState();
                    break;

                /* 0x1122: Switch VCO autotune */
                case 0x1122:
                    if( !VCO_getAutotune() )
                        VCO_setAutotune(true);
                    else VCO_setAutotune(false);
                    break;

                /* 0x1100: Turn VCO off */
                case 0x1100:
                    VCO_switch();
                    break;

                /* 0x1100: Turn VCO on */
                case 0x11FF:
                    VCO_switch();
                    break;
            }  

            /* 0x20XX: Set VCO frequency high boundary */
            if( inString[2] == 0x20 ){
                uint8_t bound = uint8_t(inString[3]);
                VCO_setBoundaries( bound, 0 );
                VCO_setAnalogOutput(VCO_getAnalogOutput());
            }

            /* 0x21XX: Set VCO frequency high boundary */
            if( inString[2] == 0x21 ){
                uint8_t bound = uint8_t(inString[3]);
                VCO_setBoundaries( 0, bound );
                VCO_setAnalogOutput(VCO_getAnalogOutput());
            }
        }
    }
    Serial2.flush();
}
