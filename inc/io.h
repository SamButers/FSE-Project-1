#ifndef IO_H_
#define IO_H_

#include "utils.h"

#define RESISTOR_PIN 4
#define FAN_PIN 5

#define PIN_ON 0
#define PIN_OFF 1

#define KI 1.0
#define KP 5.0
#define KD 5.0

#define MAX_ERROR 100
#define MIN_ERROR -100

#define T 1

typedef struct Status {
	int fan, resistor;
} Status;

extern float totalError;
extern float previousError;
extern Status status;

float calculatePID(float referenceValue, float value);
void turnOn(int pin);
void turnOff(int pin);
void applyPWM(int pin, float pid);
float controlTemperature(Information *info);
void IOStart();
void IOClose();

#endif