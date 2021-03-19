#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "crc.h"
#include "uart.h"

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

	return 0;
}