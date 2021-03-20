#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include "crc.h"
#include "uart.h"
#include "i2c.h"
#include "bme280.h"

void gracefullyStop(int sig) {	
	endwin();
	
	exit(0);
}

int checkInput() {
	int input = getch();
	
	switch(input) {
		case 10:
			return 1;
		default:
			return 0;
	}
}

void displayInformation(Information *info) {
	clear();
	
	printw("Internal Temperature: %f\n", info->internalTemperature);
	printw("Environment Temperature: %f\n", info->environmentTemperature);
	printw("Reference Temperature: %f\n", info->referenceTemperature);
	printw("Press Enter to change reference temperature source.");
	
	refresh();
}

void mainLoop(int uartFilestream, int bm280, int mode, float referenceValue) {
	Information info;
	
	noecho();
	nodelay(stdscr, 1);
	while(1) {
		if(checkInput())
			break;
		
		//info.internalTemperature = getInternalTemperature(uartFilestream);
		//info.potentiometerTemperature = getPotentiometerTemperature(uartFilestream);
		info.internalTemperature = 37.2;
		info.potentiometerTemperature = 29.2;
		info.environmentTemperature = getEnvironmentTemperature();
		
		if(mode)
			info.referenceTemperature = referenceValue;
			
		else
			info.referenceTemperature = info.potentiometerTemperature;
		
		//controlTemperature(&info);
		
		//displayLCDInformation(&info);
		displayInformation(&info);
		sleep(1);
	}
}

int main() {
	float referenceValue;
	int choice, mode;
	
	int uartFilestream;
	if(UARTOpen(&uartFilestream)) {
		printf("UART initialization error.\n");
		return 0;
	}
	
	int bme280 = bme280Init(1, 0x76);
	if(bme280) {
		printf("BME280 device initialization error.\n");
		return 0;
	}
	
	initscr();
	
	signal(SIGINT, gracefullyStop);
	
	while(1) {
		clear();
		
		echo();
		nodelay(stdscr, 0);
		
		printw("Choose the reference temperature source:\n");
		printw("1. Potentiometer\n");
		printw("2. Terminal input\n");
	
		refresh();
		
		choice = getch();
		mode = -1;
		
		while(mode == -1) {
			switch(choice) {
				case 49:
					mode = 0;
					break;
					
				case 50:
					mode = 1;
					printw("Insert the reference value:\n");
					
					refresh();
					
					scanw("%f", &referenceValue);
					break;
					
				default:
					break;
			}
		}
		
		mainLoop(uartFilestream, bme280, mode, referenceValue);
	}
	
	return 0;
}