#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "crc.h"
#include "uart.h"
#include "i2c.h"
#include "bme280.h"

int main() {
	int uartFilestream;
	
	if(UARTOpen(&uartFilestream)) {
		printf("UART initialization error.\n");
		return 0;
	}

	printf("Internal Temperature: %f\n", getInternalTemperature(uartFilestream));

	sleep(1);

	printf("Potentiometer Temperature: %f\n", getPotentiometerTemperature(uartFilestream));

	close(uartFilestream);
	
	int i2cFilestream;
	struct bme280_dev *bme280 = initializeBME280("/dev/i2c-1", 0x76, &i2cFilestream);
	if(bme280 != NULL)
		printf("Reference Temperature: %f\n", getReferenceTemperature(bme280));
	
	free(bme280->intf_ptr);
	free(bme280);
	close(i2cFilestream);

	return 0;
}