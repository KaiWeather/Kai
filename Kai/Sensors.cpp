#include <Arduino.h>
#include "Sensors.h"



#ifdef WATER_TEMP
	#include <DallasTemperature.h>
	float water_temp(DallasTemperature sensors)
	{
		sensors.requestTemperatures();
		return sensors.getTempCByIndex(0);
	}
	void printTemperature(DallasTemperature sensors,DeviceAddress deviceAddress)
	{
		//tell sensor to update temperature
		sensors.requestTemperatures();
		float temp = sensors.getTempC(deviceAddress);
		Serial.print("Temp C: ");
		Serial.print(temp);
		Serial.print("\t Temp F: ");
		Serial.print(DallasTemperature::toFahrenheit(temp)); // Converts Celcius to Fahrenheit
		Serial.print("\t");
	}
#endif

void senspac::update(float atemp,float wtemp, float humid, float rfall, float wspd, float wdir, int wdeb, int wflow)
{
	ATEMP = atemp;
	WTEMP = wtemp;
	HUMID = humid;
	RFALL = rfall;
	WSPD = wspd;
	WDIR = wdir;
	WDEB = wdeb;
	WFLOW = wflow;
}


void senspac::print()
{
	//print out each sensor data
	if(ATEMP != -404) {
	Serial.print("ATemp = ");
	Serial.print(ATEMP);
	Serial.print(" C \t");
	}
	if(WTEMP != -404) {
	Serial.print("WTemp = ");
	Serial.print(WTEMP);
	Serial.print(" C \t");
	}
	if(HUMID != -404) {
	Serial.print("Hum = ");
	Serial.print(HUMID);
	Serial.print(" % \t");
	}
	if(RFALL != -404) {
	Serial.print("Rain = ");
	Serial.print(RFALL);
	Serial.print(" mm/h \t");
	}
	if(WSPD != -404) {
	Serial.print("WSpd = ");
	Serial.print(WSPD);
	Serial.print(" m/s \t");
	}
	if(WDIR != -404) {
	Serial.print("WDir = ");
	Serial.print(WDIR);
	Serial.print(" Deg \t");
	}
	if(WDEB != -404) {
	Serial.print("Depth = ");
	Serial.print(WDEB);
	Serial.print(" mm \t");
	}
	if(WDEB != -404) {
	Serial.print("Flow = ");
	Serial.print(WDEB);
	Serial.print(" l/min \t");
	}
	Serial.println();
}

String senspac::csv()
{
	String data = String("");
	int calls = 0;
	//print out each sensor data
	if(ATEMP != -404) {
		data += String(location) + ",0," + ATEMP;
		calls++;
	}
	if(WTEMP != -404) {
		data += ',' + String(location) + ",1," + WTEMP;
		calls++;
	}
	if(HUMID != -404) {
		data += ',' + String(location) + ",2," + HUMID;
		calls++;
	}
	if(RFALL != -404) {
		data += ',' + String(location) + ",3," + RFALL;
		calls++;
	}
	if(WSPD != -404) {
		data += ',' + String(location) + ",4," + WSPD;
		calls++;
	}
	if(WDIR != -404) {
		data += ',' + String(location) + ",5," + WDIR;
		calls++;
	}
	if(WDEB != -404) {
		data += ',' + String(location) + ",6," + WDEB;
		calls++;
	}
	if(WDEB != -404) {
		data += ',' + String(location) + ",7," + WDEB;
		calls++;
	}
	//Serial.println(data);
	csvlen = calls;
	return data;
}

String senspac::csv(int pos)
{
	String data = String("");
	//print out each sensor data
	switch(pos)
	{
		case 0:
			if(ATEMP != -404) {
				data += String(location) + ",0," + ATEMP;
			}
			if(WTEMP != -404) {
				data += ',' + String(location) + ",1," + WTEMP;
			}
			if(HUMID != -404) {
				data += ',' + String(location) + ",2," + HUMID;
			}
			break;
		case 1:
			if(RFALL != -404) {
				data += String(location) + ",3," + RFALL;
			}
			if(WSPD != -404) {
				data += ',' + String(location) + ",4," + WSPD;
			}
			if(WDIR != -404) {
				data += ',' + String(location) + ",5," + WDIR;
			}
			break;
		case 2:
			if(WDEB != -404) {
				data += String(location) + ",6," + WDEB;
			}
			if(WDEB != -404) {
				data += ',' + String(location) + ",7," + WDEB;
			}
			break;
	}
	Serial.println(data);
	return data;
}

int senspac::rain()
{
	return RFALL;
}
