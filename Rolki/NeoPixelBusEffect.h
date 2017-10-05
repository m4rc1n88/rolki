/*
 * NeoPixelBusEffect.h
 *
 *  Created on: 23.09.2017
 *      Author: m4rc1n88
 */

#ifndef NEOPIXELBUSEFFECT_H_
#define NEOPIXELBUSEFFECT_H_
#include "Arduino.h"
#include <NeoPixelBrightnessBus.h>

class NeoPixelBusEffect {
public:
	NeoPixelBusEffect(uint8_t, uint8_t );
	virtual ~NeoPixelBusEffect();


protected:
	//NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip;
	uint8_t _program;
	uint8_t _step;
	uint8_t _brightensLevel;
	uint8_t _speedLevel;
	uint8_t _segmentLength;
	RgbColor _color;
};

#endif /* NEOPIXELBUSEFFECT_H_ */
