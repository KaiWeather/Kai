#include <Arduino.h>
#include "Sensors.h"
#include "Configuration.h"
#include <pins_arduino.h>

void senslib::update(int htu,int htuh, int dts, int R)
{
	//run each update routine
	ambient_temp(htu);
	water_temp(dts);
	humidity(htuh);
	rain_fall(R);
	wind_speed();
	wind_direction();
	wind_dir_lookup();
	water_depth();
	water_flow();
}

void senslib::print()
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

void senslib::csv()
{
	
}

//Sensor Update functions
void senslib::ambient_temp(int htu)
{
	if (ATEMP_A == true)
	{
		ATEMP = htu;// * 100;//100 shift data over for two decimal points in a int value
	}
	else
	{
		ATEMP = -404; // Unconnect error code (404 not found)
	}
	//return ATEMP;
}

void senslib::water_temp(int dts)
{
	if (WTEMP_A == true)
	{
		WTEMP = dts;// * 100;
	}
	else
	{
		WTEMP = -404; // Unconnect error code (404 not found)
	}
	//return WTEMP;
}

void senslib::humidity(int htuh)
{
	if (ATEMP_A == true)
	{
		HUMID = htuh;//htu.readHumidity() * 100;//100 shift data over for two decimal points in a int value
	}
	else
	{
		HUMID = -404; // Unconnect error code (404 not found)
	}
	//return HUMID;
}

void senslib::rain_fall(int R)
{
	RFALL = R;
	//return RFALL;
}

void senslib::wind_speed()
{
	float WSValue;
	WSValue = analogRead(WIND_SPEED)*(5.0/ 1023.0);
	WSValue -= 0.4;
	if (WSValue > 0)
	{
		WSValue /= 0.049382716;//(2-0.4)/32.4 = 0.049382716
	}
	else
	{
		WSValue = 0;
	}
	WSPD = WSValue * 100; //100 shift data over for two decimal points in a int value
	//return WSPD;
}

void senslib::wind_direction()
{
	WDIR = wind_dir_lookup();
}

int senslib::wind_dir_lookup()
{
	int WDValue;
	WDValue = analogRead(WIND_DIRECTION);
	if (WDValue < 75)
		return 11250;
	if (WDValue < 88)
		return 6750;
	if (WDValue < 109)
		return 9000;
	if (WDValue < 155)
		return 15750;
	if (WDValue < 215)
		return 13500;
	if (WDValue < 267)
		return 20250;
	if (WDValue < 348)
		return 18000;
	if (WDValue < 435)
		return 2250;
	if (WDValue < 531)
		return 4500;
	if (WDValue < 615)
		return 24750;
	if (WDValue < 665)
		return 22500;
	if (WDValue < 743)
		return 33750;
	if (WDValue < 804)
		return 0;
	if (WDValue < 854)
		return 29250;
	if (WDValue < 914)
		return 31500;
	if (WDValue < 982)
		return 27000;
	return (-404); // does not exist, sensor not found
}

void senslib::water_depth()
{

	WDEB = -404;//Need to fill in
}

void senslib::water_flow()
{
	WFLOW = -404;//Need to fill in
}
