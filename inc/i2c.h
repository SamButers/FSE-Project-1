#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>
#include "bme280.h"

typedef struct Information {
	float internalTemperature, potentiometerTemperature, environmentTemperature, referenceTemperature;
} Information;

float getEnvironmentTemperature();
void displayLCDInformation(Information *info, int mode);
int lcdSetup();
void lcdClose();

#endif
