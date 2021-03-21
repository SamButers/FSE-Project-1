#ifndef I2C
#define I2C
#include <stdint.h>
#include "bme280.h"

typedef struct Information {
	float internalTemperature, potentiometerTemperature, environmentTemperature, referenceTemperature;
} Information;

float getEnvironmentTemperature();
void displayLCDInformation(Information *info, int mode);
int lcdSetup();

#endif
