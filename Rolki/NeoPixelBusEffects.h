/*
 * NeoPixelBusEffects.h
 *
 *  Created on: 23.09.2017
 *      Author: m4rc1n88
 */

#ifndef NEOPIXELBUSEFFECTS_H_
#define NEOPIXELBUSEFFECTS_H_

#define MAX_COLOR_NUM 8

#include "NeoPixelBrightnessBus.h"

template<typename T_COLOR_FEATURE, typename T_METHOD> class NeoPixelBusEffects :
		public NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>
{
public:
	NeoPixelBusEffects(uint16_t countPixels, uint8_t pin) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pin),
		_program(1),
		_step(0),
		_brightnessLevel(4),
		_speedLevel(4),
		_segmentLength(2)

    {
		tabColor[0] = red;
		tabColor[1] = green;
		tabColor[2] = blue;
		tabColor[3] = white;
		tabColor[4] = black;
		tabColor[5] = yellow;
		tabColor[6] = magenta;
		tabColor[7] = cyan;

    };

	NeoPixelBusEffects(uint16_t countPixels, uint8_t pinClock, uint8_t pinData) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pinClock, pinData),
		_program(1),
		_step(0),
		_brightnessLevel(4),
		_speedLevel(4),
		_segmentLength(2)
    {
		tabColor[0] = red;
		tabColor[1] = green;
		tabColor[2] = blue;
		tabColor[3] = white;
		tabColor[4] = black;
		tabColor[5] = yellow;
		tabColor[6] = magenta;
		tabColor[7] = cyan;
    };

	NeoPixelBusEffects(uint16_t countPixels) :
    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>(countPixels),
		_program(1),
		_step(0),
		_brightnessLevel(4),
		_speedLevel(4),
		_segmentLength(2)
    {
		tabColor[0] = red;
		tabColor[1] = green;
		tabColor[2] = blue;
		tabColor[3] = white;
		tabColor[4] = black;
		tabColor[5] = yellow;
		tabColor[6] = magenta;
		tabColor[7] = cyan;
    };


	void SetStrip(RgbColor color){
		for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++ )NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, color);
	};


	void blink(){
		uint16_t switchNum;
		switchNum = (16384/(1 << _speedLevel));

		if ( (_step / switchNum) % 2) SetStrip( tabColor[0] );
		else SetStrip( tabColor[1]);
		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(0xFF / (1 << (8 - _brightnessLevel)));
	}


	void Run(){
		switch(_program){
		case 1:
			blink();
			break;
		case 2:
			switchColor(tabColor, 8);
			break;
		case 3:
			jumpColor(tabColor, 8);
			break;
		case 4:
			swapTwoColor();
			break;
		}
		//blink();
		//switchColor(tabColor, 8);
		//jumpColor(tabColor, 8);
		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::Show();
		_step++;
	}


	void jumpColor(RgbColor *color, uint8_t num){
		uint16_t switchNum;
		switchNum = (16384/(1 << _speedLevel));
		//if(g_debugLevel > 2)Serial.print("Function Jump3 _step: ");
		//if(g_debugLevel > 2)Serial.print(_step);
		//if(g_debugLevel > 2)Serial.print("switch Num: ");

		//if(g_debugLevel > 2)Serial.print(switchNum);
		//if(g_debugLevel > 2)Serial.print("case: ");
		//if(g_debugLevel > 2)Serial.print((_step/switchNum)%num);

		// protection from _step overflow in next iteration(two color in row or cut some colors) not tested
		if( (( _step / switchNum ) % num == 0) && (_step + num * switchNum > 0xFFFFFF) )_step = 0;
		SetStrip(color[(_step / switchNum) % num]);
		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(0xFF / (1 << (8 - _brightnessLevel)));
	}//End function jumpColor


	void switchColor(RgbColor *color, uint8_t num){
		uint32_t switchNum;
		uint8_t actualBrightness;
		uint8_t maxBrightness = 0xFF / (1 << (8 - _brightnessLevel));
		switchNum = (65536/ (1 << _speedLevel));
		actualBrightness = (_step % switchNum) * maxBrightness / switchNum;
/*		  Serial.print("Step: ");
		  Serial.print(_step);
		  Serial.print(" Switch num: ");
		  Serial.print(switchNum);
		  Serial.print(" BLevel: ");
		  Serial.print(_brightnessLevel);
		  Serial.print(" BMax: ");
		  Serial.print(maxBrightness);
		  Serial.print(" BActual: ");
		  Serial.print(actualBrightness);
		  Serial.print(" Col num: ");
		  Serial.print(((_step / switchNum) % (num * 2)) / 2);
		  Serial.print(" if: ");*/


		// protection befor g_step overflow in next iteration(two color in row or cut some colors from table
		//must be add
		//if ( (g_step + 2 * num * switchNum > 0xFFFFFFF) && ( (g_step / switchNum) % (num * 2) == 0))


		SetStrip( color[ ((_step / switchNum) % (num * 2)) / 2] );
		if( !( ( (_step / switchNum) % (num * 2) ) % 2 ) )	  {
			//Serial.println(0);
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(actualBrightness);
		} else {
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(maxBrightness - actualBrightness);
			//Serial.println(1);
		}
	}//End function switchColor

	void snake(){


	}

	void swapTwoColor(){
		uint16_t switchNum;
		switchNum = (16384/(1 << _speedLevel));

		if ( (_step / switchNum) % 2){
			for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++ ){
				NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, tabColor[(i / _segmentLength) % 2]);
			}
		}
		else {
			for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++ ){
				NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, tabColor[ !((i / _segmentLength) % 2)]);
			}
		}
		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(0xFF / (1 << (8 - _brightnessLevel)));

	}



	void setProgram(uint8_t p_program){
		_program = p_program;
	}

	void setBrightnessLevel(uint8_t p_brightnessLevel){
		_brightnessLevel = p_brightnessLevel;
	}

	void setSpeedLevel(uint8_t p_speedLevel){
		_speedLevel = p_speedLevel;
	}

	void setSegmentLength(uint8_t p_segmentLength){
		_segmentLength = p_segmentLength;
	}


	void setColor(uint32_t p_color, uint8_t p_num){
		if(p_num > MAX_COLOR_NUM - 1)p_num = 0;
		tabColor[p_num] = RgbColor( (p_color >> 16) & 0xFF, (p_color >> 8) & 0xFF, (p_color >> 0) & 0xFF);
	}

	void setDefaultColorTab(){
		tabColor[0] = red;
		tabColor[1] = green;
		tabColor[2] = blue;
		tabColor[3] = white;
		tabColor[4] = black;
		tabColor[5] = yellow;
		tabColor[6] = magenta;
		tabColor[7] = cyan;
	}
	//virtual ~NeoPixelBusEffects();

protected:
	uint8_t _program;
	uint32_t _step;
	uint8_t _brightnessLevel;
	uint8_t _speedLevel;
	uint8_t _segmentLength;


private:
	RgbColor red = RgbColor(255, 0, 0);
	RgbColor green = RgbColor(0, 255, 0);
	RgbColor blue = RgbColor(0, 0, 255);

	RgbColor yellow = RgbColor(255, 255, 0);
	RgbColor magenta = RgbColor(255, 0, 255);
	RgbColor cyan = RgbColor(0, 255, 255);

	RgbColor white = RgbColor(255);
	RgbColor black = RgbColor(0);

	RgbColor tabColor[MAX_COLOR_NUM];// = {RgbColor(255, 0, 0), RgbColor(255, 0, 0)};

};

#endif /* NEOPIXELBUSEFFECTS_H_ */
