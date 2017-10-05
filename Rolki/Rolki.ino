#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "NeoPixelBusEffects.h"

#pragma once
#define PixelCount 12
#define PixelPin 2

#define AP_MODE 1
#define CLIENT_MODE 2
#define MODE AP_MODE

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
		WiFi.softAP(networkName,password);
	#elif MOOD== CLIENT_MODE
		//Client mode will be defined
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
	#elif MOOD == CLIENT_MODE
		//Client mode will be defined
	#endif
}


void initUdp(){
#if MODE == AP_MODE
	Udp.begin(localPort);
#elif MOOD == CLIENT_MODE
	//Client mode will be defined
#endif
}


void initStrip(){
	strip.Begin();
	strip.SetStrip(blue);
	strip.Show();
}


void readUdpData(){
	uint8_t noBytes = Udp.parsePacket();
	if(noBytes){
		Udp.read(packetBuffer, noBytes);
		Serial.print("Recievd: ");Serial.print(noBytes);Serial.println("bytes");
		for(uint8_t i = 0; i < noBytes; i++){
			Serial.print("Data[");
			Serial.print(i);
			Serial.print("] = ");
			Serial.println(packetBuffer[i]);
		}
		if((packetBuffer[0] == 0x01) && (packetBuffer[0] == 0x01))strip.setProgram(1);
		else if((packetBuffer[0] == 0x02) && (packetBuffer[0] == 0x02))strip.setProgram(2);
		else if((packetBuffer[0] == 0x03) && (packetBuffer[0] == 0x03))strip.setProgram(3);

		//new parser must be implemented

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
