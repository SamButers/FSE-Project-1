#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "crc.h"
#include "uart.h"

int main() {
	int filestream;
	
	if(UARTOpen(&filestream)) {
		printf("UART initialization error.\n");
		return 0;
	}

	printf("Internal Temperature: %f\n", getInternalTemperature(filestream));

	sleep(1);

	printf("Internal Temperature: %f\n", getPotentiometerTemperature(filestream));

	return 0;
}