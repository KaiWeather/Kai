#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define VERSION 1.0

// Serial Port on the Arduino
// Serial 0 is the USB connect.
// Serial 1 is the external UART (LEO,MEGA)
#define SERIAL_PORT

// UART Serial Communication speed
#define BAUDRATE 9600

/*========================
  ██╗    ██╗██╗███████╗██╗
  ██║    ██║██║██╔════╝██║
  ██║ █╗ ██║██║█████╗  ██║
  ██║███╗██║██║██╔══╝  ██║
  ╚███╔███╔╝██║██║     ██║
   ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝
  ========================*/

//Uncomment to enable WIFI Module
//Wifi Modes:
// 0: default php push data mode
// 1: Run each test utillity

//#define WIFI 0

#ifdef WIFI
	//CC3000 pin config
	#define CC3000_IRQ   19 //Needs to be an interrupt pin
	#define CC3000_VBAT  5
	#define CC3000_CS    10

	// UNO SCK/MISO/MOSI Setup
	//#define CC3000_CLK   13
	//#define CC3000_MISO  12
	//#define CC3000_MOSI  11

	// MEGA SCK/MISO/MOSI Setup
	#define CC3000_CLK   52
	#define CC3000_MISO  50
	#define CC3000_MOSI  51

	// Wifi network Settings
	#define WLAN_SSID       "SSID"
	#define WLAN_PASS       "PASS"

	//Pick you Wifi encryption
	//#define WLAN_SECURITY   WLAN_SEC_UNSEC
	//#define WLAN_SECURITY   WLAN_SEC_WEP
	//#define WLAN_SECURITY   WLAN_SEC_WPA
	#define WLAN_SECURITY   WLAN_SEC_WPA2

	//Timeout for server connection
	#define IDLE_TIMEOUT_MS  3000

	//Website Access
	#define WEBSITE      "WEBSITE"
	#define WEBPAGE      "WEBPAGE/push.php"

#endif

/*================================
  ██╗  ██╗██████╗ ███████╗███████╗
  ╚██╗██╔╝██╔══██╗██╔════╝██╔════╝
   ╚███╔╝ ██████╔╝█████╗  █████╗  
   ██╔██╗ ██╔══██╗██╔══╝  ██╔══╝  
  ██╔╝ ██╗██████╔╝███████╗███████╗
  ╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝
  ================================*/

//Uncomment to enable XBEE Module
//#define XBEE

#ifdef XBEE

	//XBEE Connection Setup
	#define XBEE_SERIAL 0
	#define XBEE_BAUDRATE 9600

	/* XBEE API Mode
	 * 0 : API disabled : Serial Data
	 * 1 : API Enable
	 * 2 : API Enable with escaped control characters
	 */
	#define XBEE_API 0

#endif

/*===========================================================
  ███████╗███████╗███╗   ██╗███████╗ ██████╗ ██████╗ ███████╗
  ██╔════╝██╔════╝████╗  ██║██╔════╝██╔═══██╗██╔══██╗██╔════╝
  ███████╗█████╗  ██╔██╗ ██║███████╗██║   ██║██████╔╝███████╗
  ╚════██║██╔══╝  ██║╚██╗██║╚════██║██║   ██║██╔══██╗╚════██║
  ███████║███████╗██║ ╚████║███████║╚██████╔╝██║  ██║███████║
  ╚══════╝╚══════╝╚═╝  ╚═══╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝
  ===========================================================*/

#define CAPTURE_INTERVAL 5000 //ms - how long till the next time it will capture new sensor data

//Uncomment which sensors to enable
//#define RAIN_FALL 12 // Leo use 8, Mega use D12 (Pinchangeint)
#define WIND_SPEED A5
#define WIND_DIRECTION A0
#define HUMID_HTU
//#define WATER_TEMP 4
//#define WATER_DEPTH A3    //Water Depth have two values one for the data and another for a referance
//#define WATER_DEPTH_REF A4
//#define WATER_FLOW

//Include any custom sensors here




#endif