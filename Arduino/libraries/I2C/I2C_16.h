#ifndef I2C_16
#define I2C_16

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Wire.h"

// Read 16 bit int from I2C address addr and register reg
uint16_t read16(uint8_t addr, uint8_t reg);

// Write data to I2C address addr, register reg
void write16(uint8_t addr, uint8_t reg, uint16_t data);

#endif
