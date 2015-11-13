

#ifndef HUMIDITY_HIH6130
#define HUMIDITY_HIH6130
#define uint8_t byte
#define REGISTER_INPUT (0)
#include <stdint.h>
#include <math.h>
#include <Wire.h>
#include "Arduino.h"

byte fetch_humidity_temperature(unsigned int *p_H_dat, unsigned int *p_T_dat);

void print_float(float f, int num_digits);

#endif
