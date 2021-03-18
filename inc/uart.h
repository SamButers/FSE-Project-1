#ifndef UART
#define UART

int UARTOpen(int *filestream);
int UARTRead(int filestream, void *buffer, int length);
int verifyMessage(unsigned char *message, int size);
float getTemperature(int filestream, unsigned char subCode);
float getInternalTemperature(int filestream);
float getPotentiometerTemperature(int filestream);

#endif