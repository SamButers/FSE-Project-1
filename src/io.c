#include <wiringPi.h>
#include <stdio.h>    
#include <unistd.h>
#include <softPwm.h>
#include "io.h"

float totalError = 0;
float previousError = 0;
Status status = {0, 0};

float calculatePID(float referenceValue, float value) {
	float error = referenceValue - value;
	totalError += error;
	
	if(totalError >= MAX_ERROR)
		totalError = MAX_ERROR;
		
	else if(totalError <= MIN_ERROR)
		totalError = MIN_ERROR;
		
	float delta = error - previousError;
	float pid = KP * error + (KI * T) * totalError + (KD/T) * delta;
	
	if(pid >= MAX_ERROR) {
		pid = MAX_ERROR;
	}
		
	else if(pid <= MIN_ERROR) {
		pid = MIN_ERROR;
	}
	
	previousError = error;
	
	return pid;
}

void applyPWM(int pin, float pid) {
	softPwmWrite(pin, pid);
	usleep(10000);
}

void turnOff(int pin) {
	applyPWM(pin, 0);
}

float controlTemperature(Information *info) {
	float pid = calculatePID(info->referenceTemperature, info->internalTemperature);
	
	if(pid > 0) {
		if(status.resistor == 0) {
			turnOff(FAN_PIN);
		}
		
		applyPWM(RESISTOR_PIN, pid);
		return pid;
	}
	
	else if(pid <= -40) {
		if(status.fan == 0) {
			turnOff(RESISTOR_PIN);
		}
		
		applyPWM(FAN_PIN, (-1.0) * pid);
		return pid;
	}
	
	else {
		turnOff(RESISTOR_PIN);
		turnOff(FAN_PIN);
		
		return 0;
	}
}

int IOStart() {
	if(wiringPiSetup() == -1)
		return 1;
	
	pinMode(RESISTOR_PIN, OUTPUT);
	pinMode(FAN_PIN, OUTPUT);
	
	softPwmCreate(RESISTOR_PIN, 1, 100);
	softPwmCreate(FAN_PIN, 1, 100);
	
	return 0;
}
