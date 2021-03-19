#include "i2c.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <string.h>
#include "bme280.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
	Identifier id;

	id = *((Identifier *)intf_ptr);

	write(id.filestream, &reg_addr, 1);
	read(id.filestream, data, len);

	return 0;
}

void user_delay_us(uint32_t period, void *intf_ptr) {
	usleep(period);
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
	uint8_t *buf;
	Identifier id;

	id = *((Identifier *)intf_ptr);

	buf = malloc(len + 1);
	buf[0] = reg_addr;
	memcpy(buf + 1, data, len);
	if (write(id.filestream, buf, len + 1) < (uint16_t)len)
	{
		return BME280_E_COMM_FAIL;
	}

	free(buf);

	return BME280_OK;
}

int setupBME280(struct bme280_dev *device) {
	int8_t result;
	uint8_t settings_sel;
	
	device->settings.osr_h = BME280_OVERSAMPLING_1X;
	device->settings.osr_p = BME280_OVERSAMPLING_16X;
	device->settings.osr_t = BME280_OVERSAMPLING_2X;
	device->settings.filter = BME280_FILTER_COEFF_16;
	
	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
	
	result = bme280_set_sensor_settings(settings_sel, device);
	if(result != BME280_OK) {
		printf("Device setup error.\n");
		return 1;
	}
	
	return 0;
}

struct bme280_dev* initializeBME280(char *bus, uint8_t address, int *filestream) {
	Identifier id;
	struct bme280_dev *device = malloc(sizeof(struct bme280_dev));
	int8_t result = BME280_OK;
	
	if((id.filestream = open(bus, O_RDWR)) == -1) {
		printf("I2C bus error.\n");
		return NULL;
	}
	
	*filestream = id.filestream;
	
	id.address = address;
	device->intf = BME280_I2C_INTF;
	device->read = user_i2c_read;
	device->write = user_i2c_write;
	device->delay_us = user_delay_us;
	device->intf_ptr = &id;
	
	result = bme280_init(device);
	if(result != BME280_OK) {
		printf("Device initialization error.\n");
		return NULL;
	}
	
	if(setupBME280(device))
		return NULL;
	
	return device;
}

float getReferenceTemperature(struct bme280_dev *device) {
	struct bme280_data data;
	int8_t result;
	
	result = bme280_set_sensor_mode(BME280_FORCED_MODE, device);
	if(result != BME280_OK) {
		printf("Sensor mode setup error.\n");
		return -1;
	}
	
	device->delay_us(bme280_cal_meas_delay(&device->settings), device->intf_ptr);
	
	result = bme280_get_sensor_data(BME280_ALL, &data, device);
	
	return 0.01f * data.temperature;
}
