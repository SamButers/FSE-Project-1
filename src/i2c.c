#include "i2c.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "bme280.h"
#include "lcd.h"

float getEnvironmentTemperature() {
	int temperature, pressure, humidity;
	
	usleep(100000);
	bme280ReadValues(&temperature, &pressure, &humidity);
	
	return (float)temperature/100.0;
}

int lcdSetup() {
	if(wiringPiSetup() == -1)
		return 1;
	
	fd = wiringPiI2CSetup(0x27);
	lcd_init();
	
	return 0;
}

void displayLCDInformation(Information *info, int mode) {
	char *infoOne = (char*) malloc(23); //TI = xx.xx TE = xx.xx
	char *infoTwo = (char*) malloc(12); //TE = xx.xx
	
	sprintf(infoOne, "TI:%.1f TE:%.1f", info->internalTemperature, info->environmentTemperature);
	sprintf(infoTwo, "TR:%.1f", info->referenceTemperature);
	
	ClrLcd();
	
	lcdLoc(LINE1);	
	typeln(infoOne);
	
	lcdLoc(LINE2);	
	typeln(infoTwo);
	
	free(infoOne);
	free(infoTwo);
}

void lcdClose() {
	close(fd);
}