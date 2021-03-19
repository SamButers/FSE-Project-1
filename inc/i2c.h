#ifndef I2C
#define I2C
#include <stdint.h>
#include "bme280.h"

int setupBME280(struct bme280_dev *device);
struct bme280_dev* initializeBME280(char *bus, uint8_t address, int *filestream);
float getReferenceTemperature(struct bme280_dev *device);

typedef struct Identifier {
	uint8_t address, filestream;
} Identifier;

#endif
