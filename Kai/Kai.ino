/*
	Kai Arduino project fine
	Refer to Configurations.h for configuring your sensor
*/
#include "Configuration.h"
#include "Sensors.h"

//Header Setup
#ifdef RAIN_FALL
	#include <PinChangeInt.h>
#endif

#ifdef HUMID_HTU
	#include <Wire.h>
	#include "Adafruit_HTU21DF.h"
	Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#endif

#ifdef WATER_TEMP
	#include <OneWire.h>
	#include <DallasTemperature.h>
	OneWire oneWire(WATER_TEMP);
	DallasTemperature sensors(&oneWire);
	DeviceAddress Temp_0;
#endif

#ifdef WIFI
	#include <Adafruit_CC3000.h>
	#include <ccspi.h>
	#include <SPI.h>
	#include <string.h>
	#include "utility/debug.h"
	Adafruit_CC3000 cc3000 = Adafruit_CC3000(CC3000_CS, CC3000_IRQ, CC3000_VBAT, SPI_CLOCK_DIVIDER);
	uint32_t ip;

#endif

#ifdef XBEE
	#include <XBee.h>
	//Setup XBee
	XBee xbee = XBee();

	typedef struct XBeeStruct {
		int val[8];
		int location;
	} XBeeDataStruct;

	//Create the Data Structure
	static XBeeDataStruct XBeeData;

	// 64-bit addressing: This is the SH + SL address of remote XBee
	XBeeAddress64 addr64 = XBeeAddress64(COORDINATOR_SH, COORDINATOR_SL);
	// unless you have MY on the receiving radio set to FFFF, this will be received as a RX16 packet
	Tx64Request tx = Tx64Request(addr64, (uint8_t *)&XBeeData, sizeof(XBeeDataStruct));

	TxStatusResponse txStatus = TxStatusResponse();

	//Response Handler
	XBeeResponse response = XBeeResponse();
	// create reusable response objects for responses we expect to handle 
	Rx16Response rx16 = Rx16Response();
	Rx64Response rx64 = Rx64Response();
	//Receieve Variables
	uint8_t* data = 0;
	int len = 0;

	
	
	uint8_t SH[] = {'S','H'}; // serial high
	uint8_t SL[] = {'S','L'}; // serial low
	uint8_t ND[] = {'N','D'};	// Node Discover
	uint8_t NT[] = {'N','T'};	// Node Discover Timeout
	AtCommandRequest atRequest = AtCommandRequest(SH);
	AtCommandResponse atResponse = AtCommandResponse();
	int xbee_timeout = XBEE_TIMEOUT;

#endif

senspac sen;
bool wtemp_a = true;

long lastsecond;
bool SENSORCAPTURE;
int day=0,hour=0,min=0,sec=0;
bool SEND_ALLOW = true;
bool UPDATE_ALLOW = true;
bool SYNC_ALLOW = true;

#ifdef RAIN_FALL
#include <PinChangeInt.h>
volatile int dailyrainin = 0;
volatile unsigned long raintime, rainlast, raininterval;


int PUSH_INT = PUSH_DEFAULT;
int UPDATE_INT = UPDATE_DEFAULT;

void rainIRQ()
{
	//debounce the signal, check when the last IRQ
	raintime = millis();
	raininterval = raintime - rainlast;
	if (raininterval > 10) 
	{
		dailyrainin += 2794; //0.2794; // mm of rain
		//dailyrainin += 0.011; //Each dump is 0.011" of water
		rainlast = raintime; // set up for next event
	}
}
int rain_fall()
{
	return dailyrainin;
}
#endif

#ifdef WIND_SPEED
	int wind_speed() 
	{
		float WSValue;
		WSValue = analogRead(WIND_SPEED)*(5.0/ 1023.0);
		WSValue -= 0.4;
		if (WSValue > 0.04) // Adjust to remove false inputs while idle
		{
		WSValue /= 0.049382716;//(2-0.4)/32.4 = 0.049382716
		}
		else
		{
		WSValue = 0;
		}
		if (WSValue > 60) // Check if the wind speed is connected
			return -404;
		else
			return WSValue * 100;
	}
#endif

#ifdef WIND_DIRECTION
	int wind_direction() 
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
#endif

void error(int code) { // OUTPUTS a LED to indicate error
	//digitalWrite(OK_PIN, LOW);

	for(int i=0;i<= code;i++)
	{
		//digitalWrite(FAIL, HIGH);
		delay(100);
		//digitalWrite(FAIL, LOW);
	}
}
void currenttime()
{
	Serial.print("Current time: ");
	Serial.print(hour);
	Serial.print(":");
	Serial.print(min);
	Serial.print(":");
	Serial.println(sec);
}

void updatesensors()
{
	int wtemp;
	if (!sensors.getAddress(Temp_0, 0))
		wtemp =-404;
	else
	{
		sensors.requestTemperatures();
		wtemp = sensors.getTempC(Temp_0)*100;
	}
	sen.update(htu.readTemperature()*100,wtemp,htu.readHumidity()*100,dailyrainin,wind_speed(),wind_direction(),-404,-404);
	//sen.print();
}

#ifdef WIFI
	bool displayConnectionDetails(void)
	{
		uint32_t ipAddress, netmask, gateway, dhcpserv, dSerialerv;
	
		if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dSerialerv))
		{
			Serial.println(F("Unable to retrieve the IP Address!\r\n"));
			return false;
		}
		else
		{
			Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
			Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
			Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
			Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
			Serial.print(F("\nDSerialerv: ")); cc3000.printIPdotsRev(dSerialerv);
			Serial.println();
			return true;
		}
	}

	int push(String data)
	{
		//digitalWrite(CONNECTING, HIGH);
		Serial.print("Pushing Data ... ");
		String link = String(WEBPAGE) + String(PUSH) + String(AUTH) + String("&data=") + String(data) ;

		char linkBuf[link.length()+1];
		link.toCharArray(linkBuf, sizeof(linkBuf));

		//Serial.println(link);
		Serial.println(linkBuf);

		Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 80);
		if (www.connected()) {
			www.fastrprint(F("GET "));
			www.fastrprint(linkBuf);
			www.fastrprint(F(" HTTP/1.1\r\n"));
			www.fastrprint(F("Host: "));
		www.fastrprint(WEBSITE);
		www.fastrprint(F("\r\n"));
			www.fastrprint(F("\r\n"));
			www.println();
		} else {
			Serial.println(F("Connection failed"));		
			error(2);
			return -1;
		}
		String t = "";

		unsigned long lastRead = millis();
		while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
			while (www.available()) {
				char c = www.read();
				t += c;
				lastRead = millis();
			}
		}
		www.close();

		String tt = t.substring(t.length() - 5);
		Serial.println(tt);
		www.close();

		Serial.println("Push Complete");
		//digitalWrite(CONNECTING, LOW);
		return 0;
	}

	int sync() {
		//digitalWrite(CONNECTING, HIGH);
		Serial.println("Preparing to sync device");

		Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 80);
		if (www.connected()) {
			www.fastrprint(F("GET "));
			www.fastrprint(TIME);
			www.fastrprint(F(" HTTP/1.1\r\n"));
			www.fastrprint(F("Host: "));
		www.fastrprint(WEBSITE);
		www.fastrprint(F("\r\n"));
			www.fastrprint(F("\r\n"));
			www.println();
		} else {
			Serial.println(F("Connection failed"));
			error(5);
			return -1;
		}

		String t = "";

		unsigned long lastRead = millis();
		while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
			while (www.available()) {
				char c = www.read();
				t += c;
				lastRead = millis();
			}
		}
		www.close();

		String tt = t.substring(t.length() - 8);
		hour = tt.toInt();
		min = tt.substring(5,3).toInt();
		sec = tt.substring(8,6).toInt() + SYNC_OFFSET;

		currenttime();

		Serial.println("Syncing Complete");
		//digitalWrite(CONNECTING, LOW);
		return 0;
	}

	int update() // will check and update the basic parameters
	{
		//digitalWrite(CONNECTING, HIGH);
		Serial.println("updating new parameters for the device");

		Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 80);
		if (www.connected()) {
			www.fastrprint(F("GET "));
			www.fastrprint(CHECK_LINK);
			www.fastrprint(F(" HTTP/1.1\r\n"));
			www.fastrprint(F("Host: "));
		www.fastrprint(WEBSITE);
		www.fastrprint(F("\r\n"));
			www.fastrprint(F("\r\n"));
			www.println();
		} else {
			Serial.println(F("Connection failed"));
			error(5);
			return -1;
		}

		String t = "";

		unsigned long lastRead = millis();
		while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
			while (www.available()) {
				char c = www.read();
				t += c;
				lastRead = millis();
			}
		}
		www.close();

		String tt = t.substring(t.length() - 5);
		int U = tt.toInt();
		int P = tt.substring(5,3).toInt();
		//Serial.println(tt);
		//Serial.println(U);
		//Serial.println(P);

		if(U != UPDATE_INT)
		{
			UPDATE_INT = U;
			Serial.print("New Update: ");
			Serial.println(UPDATE_INT);
		}
		if(P != PUSH_INT)
		{
			PUSH_INT = P;
			Serial.print("New Push: ");
			Serial.println(PUSH_INT);
		}

		Serial.println("Update Complete");
		//digitalWrite(CONNECTING, LOW);
		return 0;

	}
#endif

#ifdef XBEE
	String XBEE_MAC(void)
	{
		 // get SH
		atRequest.setCommand(SH);
		String SH = XBEE_CMD();
	
		// set command to SL
		atRequest.setCommand(SL);
		String SL = XBEE_CMD();
		return SH + SL;
	}

	String XBEE_CMD()
	{
		String R = "";
		String tmp = "";
		xbee.send(atRequest);
		// wait up to 5 seconds for the status response
		if (xbee.readPacket(5000)) {
			// got a response!

			// should be an AT command response
			if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
				xbee.getResponse().getAtCommandResponse(atResponse);

				if (atResponse.isOk()) {
					Serial.print("Command [");
					Serial.print(atResponse.getCommand()[0]);
					Serial.print(atResponse.getCommand()[1]);
					Serial.println("] was successful!");

					if (atResponse.getValueLength() > 0) {
						Serial.print("Command value length is ");
						Serial.println(atResponse.getValueLength(), DEC);

						Serial.print("Command value: ");
						for (int i = 0; i < atResponse.getValueLength(); i++) {
							tmp = String(atResponse.getValue()[i],HEX);
							if(tmp.length() == 1)
								tmp = "0" + tmp;
							//Serial.print(atResponse.getValue()[i], HEX);
							//Serial.print(" ");
							R += tmp;
						}
						Serial.println(R);
					}
				} 
				else {
					Serial.print("Command return error code: ");
					Serial.println(atResponse.getStatus(), HEX);
				}
			} else {
				Serial.print("Expected AT response but got ");
				Serial.print(xbee.getResponse().getApiId(), HEX);
			}	 
		} else {
			// at command failed
			if (xbee.getResponse().isError()) {
				Serial.print("Error reading packet.	Error code: ");	
				Serial.println(xbee.getResponse().getErrorCode());
			} 
			else {
				Serial.print("No response from radio");	
			}
		}
		return R;
	}

	void XBEE_SEND(void)
	{
		xbee.getNextFrameId();
		Serial.println("Sending data over XBee");
		xbee.send(tx);
		Serial.println("Checking for Status Response");

		if (xbee.readPacket(5000)) {
			if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
				xbee.getResponse().getZBTxStatusResponse(txStatus);
				Serial.println("Received Response");
				if ( txStatus.getStatus() == SUCCESS ) {
					Serial.println("Successful Sent");
				} 
				else {
					Serial.println("Failure Send, Check Remote Unit");
				}
			} 
			else {
				Serial.println("[Error]: No Response");
			}
		} 
		else {
			if (xbee.getResponse().isError()) {
				Serial.print("[Error]: Reading Packet: ");
				Serial.println(xbee.getResponse().getErrorCode());
			} 
			else {
				// local XBee did not provide a timely TX Status Response.	Radio is not configured properly or connected.
				Serial.println("[Error]: No Response Status Provided, Reconfigure/Reset XBee");
			}
		}
	}

	void XBEE_RECV() {
		xbee.readPacket();
	
		if (xbee.getResponse().isAvailable()) {
			if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
				//Serial.println("Received Response");

				Serial.print("Packet Info:");
				Serial.print("\tType = ");
				if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
					xbee.getResponse().getRx16Response(rx16);
					data = rx16.getData();
					len = rx16.getDataLength();
					Serial.print("RX16");
				} 
				else {
					xbee.getResponse().getRx64Response(rx64);
					data = rx64.getData();
					len = rx64.getDataLength();
					Serial.print("RX64");
				}

				Serial.print("\tSize = ");
				Serial.println(len);
				XBeeData = (XBeeDataStruct &)*data;
				//send the new data out to test
				//SendData();
			} 
			else {
				Serial.println("[Error]: Not The Correct Response");
			}
		} 
		else {
			if (xbee.getResponse().isError()) {
				Serial.print("[Error]: Reading Packet: ");
				Serial.println(xbee.getResponse().getErrorCode());
			} 
			else {
				// local XBee did not provide a timely TX Status Response.	Radio is not configured properly or connected.
				Serial.println("[Error]: No Response Status Provided, Reconfigure/Reset XBee");
			}
		}
	}

	void XBEE_ATTACH(int val1,int val2,int val3,int val4,int val5,int val6,int val7,int val8,int loc)
	{
		XBeeData.val[0] = val1;
		XBeeData.val[1] = val2;
		XBeeData.val[2] = val3;
		XBeeData.val[3] = val4;
		XBeeData.val[4] = val5;
		XBeeData.val[5] = val6;
		XBeeData.val[6] = val7;
		XBeeData.val[7] = val8;
		XBeeData.location = loc;
	}

	String XBEE_CSV()
	{
		String data = String("");
		//print out each sensor data
		if(XBeeData.val[0] != -404) {
			data += String(XBeeData.location) + ",0," + XBeeData.val[0];
		}
		if(XBeeData.val[1] != -404) {
			data += ',' + String(XBeeData.location) + ",1," + XBeeData.val[1];
		}
		if(XBeeData.val[2] != -404) {
			data += ',' + String(XBeeData.location) + ",2," + XBeeData.val[2];
		}
		if(XBeeData.val[3] != -404) {
			data += ',' + String(XBeeData.location) + ",3," + XBeeData.val[3];
		}
		if(XBeeData.val[4] != -404) {
			data += ',' + String(XBeeData.location) + ",4," + XBeeData.val[4];
		}
		if(XBeeData.val[5] != -404) {
			data += ',' + String(XBeeData.location) + ",5," + XBeeData.val[5];
		}
		if(XBeeData.val[6] != -404) {
			data += ',' + String(XBeeData.location) + ",6," + XBeeData.val[6];
		}
		if(XBeeData.val[7] != -404) {
			data += ',' + String(XBeeData.location) + ",7," + XBeeData.val[7];
		}
		//Serial.println(data);
		return data;
	}

	int XBEE_ND(void)
	{
		int nodeCount = 0;
		String R = "";
		Serial.println("\nNode Discovery Active");
	
		// get the Node Discover Timeout (NT) value and set to timeout
		atRequest.setCommand(NT);
		Serial.print("Sending Node Discover Timeout command to the XBee ");
		xbee.send(atRequest);
		Serial.println("");
		if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
			xbee.getResponse().getAtCommandResponse(response);
			if (atResponse.isOk()) {
				if (atResponse.getValueLength() > 0) {
					// NT response range should be from 0x20 - 0xFF, but
					// I see an inital byte set to 0x00, so grab the last byte
					xbee_timeout = atResponse.getValue()[atResponse.getValueLength() - 1] * 100;
					Serial.print("Timeout:");
					Serial.println(xbee_timeout);
				}
			}
		}
	
		atRequest.setCommand(ND);
		Serial.print("Sending command to the XBee ");
		xbee.send(atRequest);
		Serial.println("");
	
		while(xbee.readPacket(xbee_timeout)) {
			// should be receiving AT command responses
			if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
				xbee.getResponse().getAtCommandResponse(atResponse);
				if (atResponse.isOk()) {
					Serial.print("Found Node: ");
					R = "";
					for (int i = 0; i < atResponse.getValueLength(); i++) {
						R += String(atResponse.getValue()[i],HEX);
						}
						Serial.println(R);
					nodeCount++;
				}
			}
		}
	
		Serial.print("Results: ");
		Serial.print(nodeCount, DEC);
		Serial.println(" node(s) responded.");
		return 0;

	}
#endif

void setup(void)
{
	sen.location = 1;
	
	//Setup pins for debug
	pinMode(OK_PIN, OUTPUT );
	pinMode(PUSH_PIN, OUTPUT );
	pinMode(CONNECTING, OUTPUT );
	pinMode(FAIL, OUTPUT );

	//digitalWrite(OK_PIN, HIGH);

	//Setup the serial line
	#ifdef SERIAL_PORT
		//while (!Serial) ; //waits for the serial line to be active
		Serial.begin(BAUDRATE);
		Serial.print(F("Powering up Kai Weather Station V"));
		Serial.print(VERSION);
		Serial.println("!\n"); 
	#endif
	//setup XBEE
	#ifdef XBEE
		Serial.println(F("\nInitializing xbee..."));
		//digitalWrite(CONNECTING, HIGH);
		Serial1.begin(9600);
		xbee.setSerial(Serial1);
		//Serial2.begin(9600);
		//xbee.setSerial(Serial2);
		delay(5000); //Wait for Xbee to fully initalize
		//digitalWrite(CONNECTING, LOW);
		Serial.println(F("\nInitializing complete..."));
	#endif

	//setup Wifi
	#ifdef WIFI
		/* Initialise the module */
		Serial.println(F("\nInitializing wifi..."));
		//digitalWrite(CONNECTING, HIGH);

		if (!cc3000.begin())
		{
			Serial.println(F("Couldn't begin()! Check your wiring?"));
			error(3);
			while(1);
		}
		Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
		if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
			Serial.println(F("Failed!"));
			error(4);
			while(1);
		}
		Serial.println(F("Connected!"));
		/* Wait for DHCP to complete */
		Serial.println(F("Request DHCP"));
		while (!cc3000.checkDHCP())
		{
			delay(100); // ToDo: Insert a DHCP timeout!
		}	
		/* Display the IP address DNS, Gateway, etc. */	
			while (! displayConnectionDetails()) {
			delay(1000);
		}
		
		ip = 0;
		// Try looking up the website's IP address
		Serial.print(WEBSITE); Serial.print(F(" -> "));
		while (ip == 0) {
			if (! cc3000.getHostByName(WEBSITE, &ip)) {
				Serial.println(F("Couldn't resolve!"));
				error(5);
			}
			delay(500);
		}
		//uint32_t ip = cc3000.IP2U32(10,165,13,50);
		cc3000.printIPdotsRev(ip);
		Serial.println("\n");
		/*
		Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");	
		int replies = cc3000.ping(ip, 5);
		Serial.print(replies); Serial.println(F(" replies"));
		*/
		sync();
		//digitalWrite(CONNECTING, LOW);

	#endif

	//setup each available sensor if need be
	#ifdef RAIN_FALL
		Serial.println("Setting up Rain Fall INT");
		pinMode(RAIN_FALL, INPUT_PULLUP); // input from wind meters rain gauge sensor
		attachPinChangeInterrupt(RAIN_FALL, rainIRQ, FALLING);
	#endif

	#ifdef HUMID_HTU
		Serial.println("Setting up htu Sensor");
		if (!htu.begin()) {
			Serial.println("Couldn't find sensor!");
			error(1);
		}
	#endif

	#ifdef WATER_TEMP
		Serial.println("Setting up Water Temp Sensor");
		sensors.begin();
		if (!sensors.getAddress(Temp_0, 0))
		{
			Serial.println("Unable to find address for Device 0");
			wtemp_a = false;
			error(1);
		}
		sensors.setResolution(Temp_0, 12);
	#endif
	Serial.println("INIT Complete");
	//digitalWrite(OK_PIN, HIGH);
	lastsecond = millis();
}

void loop() {

	#ifdef XBEE
		//Serial.print(XBEE_MAC());
		//XBEE_ND();
		//while(1);
	#endif

	//maintain clock
	if(millis() - lastsecond >= 1000)//wait for 1 second to pass by
	{
		//currenttime();
		lastsecond += 1000; // 1 second
		sec++;
		if(sec >= 60)
		{
			sec=0;
			min++;
			SEND_ALLOW = true;
			UPDATE_ALLOW = true;
			SYNC_ALLOW == true;
			if(min >= 60)
			{
				min=0;
				hour++;
				if(hour >= 24)
				{
					hour=0;
					day++;
				}
			}
		}
	}

	//Check when to update each sensor
	if(min%PUSH_INT == 0 && sec == 0 && SEND_ALLOW == true)
	{
		//digitalWrite(PUSH_PIN, HIGH);
		currenttime();
		SEND_ALLOW = false;
		//updates the sensor data
		updatesensors();

		
		#ifdef WIFI
			//Serial.println(sen.csv());
			push(sen.csv());
		#else
			sen.print();
		#endif

		//Reset rainfall since we do not want repeat data next push
		if (sen.rain() != dailyrainin) //updated rain since last time
		{
			dailyrainin -= sen.rain();
		}
		else
		{
			dailyrainin = 0;
		}

		//digitalWrite(PUSH_PIN, LOW);
	}


	#ifdef WIFI
		if(min%UPDATE_INT == 0 && sec == 0 && UPDATE_ALLOW == true)
		{
			UPDATE_ALLOW = false;
			update();
		}

		if(sec == 0 && min == 0 && hour == 0 && SYNC_ALLOW == true)
		{
			SYNC_ALLOW == false;
			sync();
		}
	#endif
}
