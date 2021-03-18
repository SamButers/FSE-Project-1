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

int UARTRequestInt(int filestream) {
	unsigned char msg_buffer[5] = {0x01, 0x23, 0xA1, 0, 0};
	
	short crc = calcula_CRC(msg_buffer, 3);
	memcpy((void*) (msg_buffer + 3), (void*) &crc, 2);
	
	int count = write(filestream, &msg_buffer, 5);
	if(count < 0) {
		printf("UART TX error.\n");
		return 1;
	}
	
	sleep(1);
	
	unsigned char *readBuffer = malloc(64);
	UARTRead(filestream, (void*) readBuffer, 64);
	
	int intValue;
    memcpy((void*) &intValue, (void*) (readBuffer + 3), 4);
	
	printf("Int lido: %d\n", intValue);
	free(readBuffer);
	
	return 0;
}

int UARTRequestFloat(int filestream) {
	unsigned char msg_buffer[5] = {0x01, 0x23, 0xA2, 0, 0};
	
	short crc = calcula_CRC(msg_buffer, 3);
	memcpy((void*) (msg_buffer + 3), (void*) &crc, 2);
	
	int count = write(filestream, &msg_buffer, 5);
	if(count < 0) {
		printf("UART TX error.\n");
		return 1;
	}
	
	sleep(1);
	
	unsigned char *readBuffer = malloc(64);
	UARTRead(filestream, (void*) readBuffer, 64);
    
    //printf("%hhx\n%hhx\n%hhx\n", *(readBuffer), *(readBuffer + 1), *(readBuffer + 2));
    //printf("%hhx\n%hhx\n%hhx\n", *(readBuffer + 3), *(readBuffer + 4), *(readBuffer + 5));
    //printf("%hhx\n%hhx\n%hhx\n", *(readBuffer + 6), *(readBuffer + 7), *(readBuffer + 8));
    
    float floatValue;
    memcpy((void*) &floatValue, (void*) (readBuffer + 3), 4);
	
	printf("Float lido: %f\n", floatValue);
	free(readBuffer);
	
	return 0;
}

int UARTRequestString(int filestream) {
	unsigned char msg_buffer[5] = {0x01, 0x23, 0xA3, 0, 0};
	
	short crc = calcula_CRC(msg_buffer, 3);
	memcpy((void*) (msg_buffer + 3), (void*) &crc, 2);
	
	int count = write(filestream, &msg_buffer, 5);
	if(count < 0) {
		printf("UART TX error.\n");
		return 1;
	}
	
	sleep(1);
	
	unsigned char *readBuffer = malloc(4);
	UARTRead(filestream, (void*) readBuffer, 4);
    
    int stringSize = (int) readBuffer[3];
    free(readBuffer);
    
    readBuffer = malloc(stringSize + 1);
    UARTRead(filestream, (void*) readBuffer, stringSize);
    
    printf("String lida: %s\n", readBuffer);
	free(readBuffer);
	
	return 0;
}