#include "i2c.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <string.h>
#include <inttypes.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "bme280.h"

float getEnvironmentTemperature() {
	int temperature, pressure, humidity;
	
	usleep(1000000);
	bme280ReadValues(&temperature, &pressure, &humidity);
	
	return (float)temperature/100.0;
}
