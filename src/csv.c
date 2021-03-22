#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "csv.h"

FILE *csvFile;

int startCSV() {
	csvFile = fopen("./data.csv", "w");
	fprintf(csvFile, "DATE,TI,TR,TE,FAN,RESISTOR\n");
	
	if(csvFile == NULL)
		return 1;
	
	return 0;
}

void writeCSV(Information *info, float pid) {
	time_t currentTime;
	struct tm *timeInfo;
	
	time(&currentTime);
	timeInfo = localtime(&currentTime);
	
	fprintf(csvFile, "%02d/%02d/%d %02d:%02d:%02d,", timeInfo->tm_mday, timeInfo->tm_mon, 1900 + timeInfo->tm_year, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
	fprintf(csvFile, "%.2f,%.2f,%.2f,", info->internalTemperature, info->referenceTemperature, info->environmentTemperature);
	
	if(pid > 0)
		fprintf(csvFile, "%.2f,%.2f\n", 0.00, pid);
		
	else if(pid <= 40)
		fprintf(csvFile, "%.2f,%.2f\n", pid, 0.00);
	
	else
		fprintf(csvFile, "%.2f,%.2f\n", 0.00, 0.00);
}

void closeCSV() {
	fclose(csvFile);
}