#include <Arduino.h>

#ifdef RAIN_FALL
#include <PinChangeInt.h>
volatile float dailyrainin = 0;
volatile unsigned long raintime, rainlast, raininterval;

void rainIRQ()
{
	//debounce the signal, check when the last IRQ
	raintime = millis();
	raininterval = raintime - rainlast;
	if (raininterval > 10) 
	{
		//dailyrainin += 0.2794; // mm of rain
		dailyrainin += 0.011; //Each dump is 0.011" of water
		rainlast = raintime; // set up for next event
	}
}
float rain_fall()
{
	return dailyrainin;
}
#endif

#ifdef WIND_SPEED
	float wind_speed() 
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
		return WSValue;
	}
#endif

#ifdef WIND_DIRECTION
	float wind_direction() 
	{
		int WDValue;
		WDValue = analogRead(WIND_DIRECTION);

		if (WDValue < 75)
			return 112.5;
		if (WDValue < 88)
			return 67.5;
		if (WDValue < 109)
			return 90;
		if (WDValue < 155)
			return 157.5;
		if (WDValue < 215)
			return 135;
		if (WDValue < 267)
			return 202.5;
		if (WDValue < 348)
			return 180;
		if (WDValue < 435)
			return 22.5;
		if (WDValue < 531)
			return 45;
		if (WDValue < 615)
			return 247.5;
		if (WDValue < 665)
			return 225;
		if (WDValue < 743)
			return 337.5;
		if (WDValue < 804)
			return 0;
		if (WDValue < 854)
			return 292.5;
		if (WDValue < 914)
			return 315;
		if (WDValue < 982)
			return 270;

	return (-1); // does not exist, sensor not found
	}
#endif

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
