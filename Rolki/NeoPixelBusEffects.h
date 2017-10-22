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
#include "Defintion.h"
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
		case TURN_OFF:
			SetStrip(black);
			break;
		case SIMPLE:
			SetStrip(tabColor[0]);
			break;
		case BLINK:
			blink();
			break;
		case BLINK_2_COLORS:
			blink();
			break;
		case PULSE:
			switchColor(tabColor, 1);
			break;
		case SWITCH_3:
			switchColor(tabColor, 3);
			break;
		case SWITCH_7:
			switchColor(tabColor, 7);
			break;
		case JUMP_3:
			jumpColor(tabColor, 3);
			break;
		case JUMP_7:
			jumpColor(tabColor, 7);
			break;
		case SWAP_2_COLORS:
			swapTwoColor();
			break;
		case SNAKE:
			snake();
			break;
		case HSV_SNAKE:
			HSLSnake();
			break;
		case HSV_TO_WHITE:
			HslToWhite();
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

		// protection from _step overflow in next iteration(two color in row or miss some colors) not tested
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
		if( !( ( (_step / switchNum) % (num * 2) ) % 2 ) ) {
			//Serial.println(0);
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(actualBrightness);
		} else {
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(maxBrightness - actualBrightness);
			//Serial.println(1);
		}
	}//End function switchColor

	void snake(){
		uint16_t v_switchNum;
		uint8_t v_startPixel;
		v_switchNum = (16384/(1 << _speedLevel));
		if( (( _step / v_switchNum ) % NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount() == 0) &&
				(_step + NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount() * v_switchNum > 0xFFFFFF) )_step = 0;
		v_startPixel = (_step / v_switchNum) % NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount();
		SetStrip(tabColor[0]);
		for(uint8_t i = 0; i < _segmentLength; i++ ){
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor( (i + v_startPixel) % NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(), tabColor[1]);
		}
	}


	void  HslToWhite(){
		uint16_t v_switchNum;
		RgbColor v_color;
		float v_f_progressL, v_f_progressH;
		v_switchNum = (65536/(1 << _speedLevel));
		v_f_progressL = (_step % v_switchNum)/static_cast<float>(v_switchNum);

		if((_step / v_switchNum) % 2 ) v_f_progressL = 1 - v_f_progressL;

	    for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++){
	    	v_f_progressH = i / static_cast<float>(NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount() - 1);
	    	//v_color = colorGamma.Correct(RgbColor(HslColor(v_f_progressH, 1.0f, v_f_progressL)));
	    	//Serial.print("Progress[");Serial.print(i);Serial.print("] = ");Serial.print(v_f_progressH);
	    	v_color = RgbColor(HslColor(v_f_progressH, 1.0f, v_f_progressL));
	    	NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, v_color);
	    }



	}

	void HSLSnake(){
		uint16_t switchNum;
		RgbColor v_color;
		float progress;
		switchNum = (65536/(1 << _speedLevel));

		for(uint8_t i = 0; i < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); i++ ){
			progress = (i * (switchNum / static_cast<float>(NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount() - 1)) + _step % switchNum)/switchNum;
			if(progress > 1.0f)progress -= 1.0f;
			//v_color = colorGamma.Correct(RgbColor(HslColor(progress, 1.0f, 0.5f)));
			v_color = RgbColor(HslColor(progress, 1.0f, 0.5f));
			NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(i, v_color);
		}

		NeoPixelBrightnessBus<T_COLOR_FEATURE, T_METHOD>::SetBrightness(0xFF / (1 << (8 - _brightnessLevel)));
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
		_step = 0;
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

	void setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue, uint8_t p_num){
		if(p_num > MAX_COLOR_NUM - 1)p_num = 0;
		tabColor[p_num] = RgbColor( p_red, p_green, p_blue);
	}

	void setDefaultColorTab(){
		tabColor[0] = red;
		tabColor[1] = green;
		tabColor[2] = blue;
		tabColor[3] = white;
		tabColor[4] = yellow;
		tabColor[5] = magenta;
		tabColor[6] = cyan;
		tabColor[7] = black;
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
