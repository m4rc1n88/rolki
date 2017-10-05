/*
 * NeoPixelBusEffect.cpp
 *
 *  Created on: 23.09.2017
 *      Author: m4rc1n88
 */
#include "NeoPixelBusEffect.h"

NeoPixelBusEffect::NeoPixelBusEffect(uint8_t PixelCount, uint8_t PixelPin ) {

	//strip = NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod>(PixelCount, PixelPin);
	_program = 1;
	_step = 0;
	_brightensLevel = 4;
	_speedLevel = 4;
	_segmentLength = 2;
	_color = RgbColor(255, 0 , 0);
	//strip.Begin();
	//for(int i = 0; i < PixelCount; i++) strip.SetPixelColor(i, red);
	//strip.Show();






	// TODO Auto-generated constructor stub

}

NeoPixelBusEffect::~NeoPixelBusEffect() {
	// TODO Auto-generated destructor stub
}

