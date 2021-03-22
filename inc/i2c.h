#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#include "bme280.h"
#include "utils.h"

float getEnvironmentTemperature();
void displayLCDInformation(Information *info, int mode);
int lcdSetup();
void lcdClose();

#endif
