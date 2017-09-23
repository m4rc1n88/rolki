#include "Arduino.h"
//#include <ESP8266WiFi>
#include "NeoPixelBusEffects.h"
//#include <NeoPixelBrightnessBus.h>
#pragma once
#define PixelCount 12
#define PixelPin 2

#define colorSaturation 255 // saturation of color constants


NeoPixelBusEffects<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

RgbColor tabColor[5] = {red, green, blue, white, black};



// the setup function runs once when you press reset or power the board
//#define
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
	Serial.begin(115200);

	delay(500);
	Serial.println("Initialize strip");
	strip.Begin();
	Serial.println("Set color");
	strip.SetStrip(blue);
	Serial.println("Show color");
	strip.Show();
	Serial.println("Initailization finisehed");

}

// the loop function runs over and over again forever
void loop() {
	//delay(10);
	strip.Run();
	Serial.println("Loop");
}
