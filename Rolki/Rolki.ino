#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "NeoPixelBusEffects.h"
#include "Defintion.h"

#pragma once
#define PixelCount 24
#define PixelPin 2

#define AP_MODE 1
#define CLIENT_MODE 2
#define MODE AP_MODE

#define DUBUG_RECIEVED 2


#define colorSaturation 255 // saturation of color constants


NeoPixelBusEffects<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);
RgbColor tabColor[5] = {red, green, blue, white, black};

IPAddress ipServer(192,168,1,102);
IPAddress ipClient(192,168,1,101);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

uint16_t localPort = 12345;

const char password[] = "sparkfun";
const char networkName[] = "ESP";

byte packetBuffer[256];

WiFiUDP Udp;


void initWiFi(){
	#if MODE == AP_MODE
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(ipServer, gateway, subnet);
		WiFi.softAP(networkName, password);
	#endif
	#if MODE == CLIENT_MODE
		  WiFi.mode(WIFI_STA);
		  WiFi.config(ipClient, gateway, subnet);
		  WiFi.begin(networkName, password);
	#endif
}


void initSerial(){
	Serial.begin(115200);
}


void printWifiStatus() {
	#if MODE == AP_MODE
		IPAddress ip = WiFi.softAPIP();
		Serial.print("IP Address: ");
		Serial.println(ip);
	#elif MODE == CLIENT_MODE
		IPAddress ip = WiFi.localIP();
		Serial.print("IP Address: ");
		Serial.println(ip);
	#endif
}


void initUdp(){
#if MODE == AP_MODE
	Udp.begin(localPort);
#elif MODE == CLIENT_MODE
	Udp.begin(localPort);
#endif
}


void initStrip(){
	strip.Begin();
	strip.SetStrip(blue);
	strip.setProgram(HSL_TO_WHITE);
	strip.setSegmentLength(4);
	strip.setSpeedLevel(4);
	strip.Show();

}


void readUdpData(){
	uint8_t v_program;
	uint8_t noBytes = Udp.parsePacket();
	if(noBytes){
		Udp.read(packetBuffer, noBytes);
//		#if MODE == AP_MODE
//	    Udp.beginPacket(ipClient, localPort);
//	    Udp.write(packetBuffer, noBytes);
//	    Udp.endPacket();
//		#endif

		#if DUBUG_RECIEVED >= 1
		Serial.print("Recievd: ");
		Serial.print(noBytes);
		Serial.print("bytes from ");
		Serial.print(Udp.remoteIP());
		Serial.print(":");
		Serial.println(Udp.remotePort());
		#endif
		#if DUBUG_RECIEVED >= 2
		for(uint8_t i = 0; i < noBytes; i++){
			Serial.print("Data[");
			Serial.print(i);
			Serial.print("] = ");
			Serial.println(packetBuffer[i]);
		}
		#endif
		if(packetBuffer[0] == PROTECTION_BIT){
			Serial.println("CRC OK");
			v_program = packetBuffer[1];
			switch(v_program){
			case TURN_OFF:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Turn Off");
				#endif
				if(packetBuffer[2] == TURN_OFF_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setProgram(TURN_OFF);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case SIMPLE:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Simple");
				#endif
				if(packetBuffer[2] == SIMPLE_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setProgram(SIMPLE);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case BLINK:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Turn Blink");
				#endif
				if(packetBuffer[2] == BLINK_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setColor(0, 1);
					strip.setSpeedLevel(packetBuffer[6]);
					strip.setProgram(BLINK);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case BLINK_2_COLORS:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program blink 2 colors");
				#endif
				if(packetBuffer[2] == BLINK_2_COLORS_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setColor(packetBuffer[6], packetBuffer[7], packetBuffer[8], 1);
					strip.setSpeedLevel(packetBuffer[9]);
					strip.setProgram(BLINK_2_COLORS);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case PULSE:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Pulse");
				#endif
				if(packetBuffer[2] == PULSE_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setSpeedLevel(packetBuffer[6]);
					strip.setProgram(PULSE);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case HSL_SNAKE:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program HSL Snake");
				#endif
				if(packetBuffer[2] == HSL_SNAKE_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(HSL_SNAKE);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case HSL_TO_WHITE:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program HSL to white");
				#endif
				if(packetBuffer[2] == HSL_TO_WHITE_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(HSL_TO_WHITE);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case JUMP_3:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Jump 3");
				#endif
				if(packetBuffer[2] == JUMP_3_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setDefaultColorTab();
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(JUMP_3);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case JUMP_7:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Jump 7");
				#endif
				if(packetBuffer[2] == JUMP_7_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setDefaultColorTab();
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(JUMP_7);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case SWITCH_3:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Switch 3");
				#endif
				if(packetBuffer[2] == SWITCH_3_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setDefaultColorTab();
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(SWITCH_3);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case SWITCH_7:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Switch 7");
				#endif
				if(packetBuffer[2] == SWITCH_7_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setDefaultColorTab();
					strip.setBrightnessLevel(packetBuffer[3]);
					strip.setSpeedLevel(packetBuffer[4]);
					strip.setProgram(SWITCH_7);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case SNAKE:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Snake");
				#endif
				if(packetBuffer[2] == SNAKE_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setColor(packetBuffer[6], packetBuffer[7], packetBuffer[8], 1);
					strip.setSpeedLevel(packetBuffer[9]);
					strip.setSegmentLength(packetBuffer[10]);
					strip.setProgram(SNAKE);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;

			case SWAP_2_COLORS: //not tested
				#if DUBUG_RECIEVED >= 1
				Serial.println("Program Swap 2 Colors");
				#endif
				if(packetBuffer[2] == SWAP_2_COLORS_LEN){
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - OK");
					#endif
					strip.setColor(packetBuffer[3], packetBuffer[4], packetBuffer[5], 0);
					strip.setColor(packetBuffer[6], packetBuffer[7], packetBuffer[8], 1);
					strip.setSpeedLevel(packetBuffer[9]);
					strip.setSegmentLength(packetBuffer[10]);
					strip.setProgram(BLINK_2_COLORS);
				} else {
					#if DUBUG_RECIEVED >= 1
					Serial.println("Length of parameters - N_OK");
					#endif
				}
				break;


			default:
				#if DUBUG_RECIEVED >= 1
				Serial.println("Unknow Program");
				#endif
				break;
			}//end switch v_program
		} else {
			#if DUBUG_RECIEVED >= 1
			Serial.println("CRC N_OK");
			#endif
		}

	}
}


void setup() {

	initSerial();
	delay(100);
	initWiFi();
	delay(100);
	initUdp();
	delay(100);
	initStrip();

}


void loop() {
	//delay(10);
	strip.Run();
	readUdpData();

	//Serial.println("Loop");
}
