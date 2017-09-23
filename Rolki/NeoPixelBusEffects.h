/*
 * NeoPixelBusEffects.h
 *
 *  Created on: 23.09.2017
 *      Author: m4rc1n88
 */

#ifndef NEOPIXELBUSEFFECTS_H_
#define NEOPIXELBUSEFFECTS_H_

#include "NeoPixelBrightnessBus.h"



template<typename T_COLOR_FEATURE, typename T_METHOD> class NeoPixelBusEffects :
		public NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>
{
public:
	NeoPixelBusEffects(uint16_t countPixels, uint8_t pin) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pin),
		_program(1),
		_step(0),
		_brightensLevel(4),
		_speedLevel(4),
		_segmentLength(2)

    {
		tabColor[0] = red;
		tabColor[1] = green;
    };

	NeoPixelBusEffects(uint16_t countPixels, uint8_t pinClock, uint8_t pinData) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pinClock, pinData),
		_program(1),
		_step(0),
		_brightensLevel(4),
		_speedLevel(4),
		_segmentLength(2)
    {
		tabColor[0] = red;
		tabColor[1] = green;
    };

	NeoPixelBusEffects(uint16_t countPixels) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels),
		_program(1),
		_step(0),
		_brightensLevel(4),
		_speedLevel(4),
		_segmentLength(2)
    {
		tabColor[0] = red;
		tabColor[1] = green;
    };


	void SetStrip(RgbColor color){
		for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++ )NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, color);
	};


	void blink(){
		uint16_t switchNum;
		switchNum = (16384/(1 << _speedLevel));

		if ( (_step / switchNum) % 2) SetStrip( tabColor[0] );
		else SetStrip( tabColor[1]);
	}


	void Run(){

		blink();
		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::Show();
		_step++;


	}

/*
	void jump(RgbColor *color, uint8_t num){
	uint16_t switchNum;
	switchNum = (16384/(1 << g_speedLevel));
	//if(g_debugLevel > 2)Serial.print("Function Jump3 g_step: ");
	//if(g_debugLevel > 2)Serial.print(g_step);
	//if(g_debugLevel > 2)Serial.print("switch Num: ");

	//if(g_debugLevel > 2)Serial.print(switchNum);
	//if(g_debugLevel > 2)Serial.print("case: ");
	//if(g_debugLevel > 2)Serial.print((g_step/switchNum)%3);

	// protection befor g_step overflow in next iteration(two color in row or cut some colors) not tested
	if( (( g_step / switchNum ) % num == 0) && (g_step + num * switchNum > 0xFFFFFF) )g_step = 0;
	SetColor(color[(g_step / switchNum) % num]);
	}



	void pulse(RgbColor color)
	{
	  //Serial.println("Pulse");
	  uint16_t switchNum;
	  uint32_t actualColor;
	  uint32_t blueComponent, greenComponent, redComponent;
	  switchNum = (65536/ (1 << g_speedLevel));
	  //Serial.println( (g_step % switchNum) * 255 / switchNum );

	  SetColor(color);
	  if ( (g_step/switchNum)%2 )
	  {
	    //Serial.println(1);
	    strip.SetBrightness((g_step % switchNum) * 255 / switchNum );
	  }
	  else
	  {
	    //Serial.println(2);
	    strip.SetBrightness(255 - (g_step % switchNum) * 255 / switchNum );
	  }
	  strip.Show();


	//  Serial.print("Step: ");
	//  Serial.print(g_step);
	//  Serial.print("switch num: ");
	//  Serial.print(switchNum);
	//  Serial.print("Color: ");
	//  Serial.println(actualColor,HEX);
	  //setColor(actualColor);

	}


	void switchColor(RgbColor *color, uint8_t num){
	  uint32_t switchNum;
	  uint8_t actualBrightness;
	  uint8_t maxBrightness = 0xFF / (1 << (8 - g_brightnessLevel));
	  switchNum = (65536/ (1 << g_speedLevel));
	  actualBrightness = (g_step % switchNum) * maxBrightness / switchNum;
	//  Serial.print("Step: ");
	//  Serial.print(g_step);
	//  Serial.print(" Switch num: ");
	//  Serial.print(switchNum);
	//  Serial.print(" BLevel: ");
	//  Serial.print(g_brightnessLevel);
	//  Serial.print(" BMax: ");
	//  Serial.print(maxBrightness);
	//  Serial.print(" BActual: ");
	//  Serial.print(actualBrightness, HEX);
	  // protection befor g_step overflow in next iteration(two color in row or cut some colors from table
	  //must be add
	    //if ( (g_step + 2 * num * switchNum > 0xFFFFFFF) && ( (g_step / switchNum) % (num * 2) == 0))
	  Serial.print(" if: ");
	  SetColor( color[ ((g_step / switchNum) % (num * 2)) / 2] );
	  if( !( ( (g_step / switchNum) % (num * 2) ) % num) )
	  {
	    Serial.println(0);
	    strip.SetBrightness(actualBrightness);
	  }
	  else
	  {
	    strip.SetBrightness(maxBrightness - actualBrightness);
	    Serial.println(1);
	  }
	}

*/

	//virtual ~NeoPixelBusEffects();

protected:
	uint8_t _program;
	uint32_t _step;
	uint8_t _brightensLevel;
	uint8_t _speedLevel;
	uint8_t _segmentLength;


private:
	RgbColor red = RgbColor(255, 0, 0);
	RgbColor green = RgbColor(0, 255, 0);
	RgbColor blue = RgbColor(0, 0, 255);
	RgbColor white = RgbColor(255);
	RgbColor black = RgbColor(0);

	RgbColor tabColor[2];// = {RgbColor(255, 0, 0), RgbColor(255, 0, 0)};

};

#endif /* NEOPIXELBUSEFFECTS_H_ */
