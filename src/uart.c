#include "uart.h"
#include "crc.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

int UARTOpen(int *filestream) {
	*filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	if(*filestream == -1)
		return 1;
		
	struct termios options;
	tcgetattr(*filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(*filestream, TCIFLUSH);
	tcsetattr(*filestream, TCSANOW, &options);
	
	return 0;
}

int UARTRead(int filestream, void *buffer, int length) {
	int rx_length;

	rx_length = read(filestream, buffer, length);
	
	if(rx_length == 0) {
		printf("No data available.\n");
		return 0;
	}
	
	else if(rx_length < 0) {
		printf("UART RX error.\n");
		return -1;
	}

	printf("Bytes lidos: %d\n", rx_length);
	
	return rx_length;
}

int verifyMessage(unsigned char *message, int size) {
	short messageCrc;
	memcpy((void*) &messageCrc, (void*) (message + size - 2), 2);

	if(messageCrc != calcula_CRC(message, size - 2))
		return 1;

	return 0;
}

float getTemperature(int filestream, unsigned char subCode) {
	unsigned char msg_buffer[9] = {0x01, 0x23, subCode, 4, 0, 4, 0, 0, 0};
	
	short crc = calcula_CRC(msg_buffer, 7);
	memcpy((void*) (msg_buffer + 7), (void*) &crc, 2);
	
	int count = write(filestream, &msg_buffer, 9);
	if(count < 0) {
		printf("UART TX error.\n");
		return 1;
	}
	
	sleep(1);
	
	unsigned char *readBuffer = malloc(9);
	int bytesRead = UARTRead(filestream, (void*) readBuffer, 9);

	if(bytesRead == -1) {
		printf("Read error.\n");
		return -1;
	}

	if(verifyMessage(readBuffer, bytesRead)) {
		printf("Invalid CRC.\n");
		return -1;
	}
	
	float value;
    memcpy((void*) &value, (void*) (readBuffer + 3), 4);
	free(readBuffer);
	
	return value;
}

float getInternalTemperature(int filestream) {
	return getTemperature(filestream, 0xC1);
}

float getPotentiometerTemperature(int filestream) {
	return getTemperature(filestream, 0xC2);
}
