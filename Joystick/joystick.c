/*
 * joystick.c
 *
 *  Created on: 22 wrz 2022
 *      Author: miqix
 */
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "joystick.h"

//
//Joystick definitions
//

Joystick_t Joystick1 = {
		.ADC_ChannelX = JOY1_ADC_CHANNEL_X,
		.ADC_ChannelY = JOY1_ADC_CHANNEL_Y,
};

Joystick_t Joystick2 = {
		.ADC_ChannelX = JOY2_ADC_CHANNEL_X,
		.ADC_ChannelY = JOY2_ADC_CHANNEL_Y,
};


//
//ADC MEASUREMENT FUNCTIONS (Edit for your MCU)
//

void ADC_Init(void)
{
	ADMUX |= (1<<REFS0); 	//Vcc as voltage reference
	ADCSRA |= (1<<ADEN); 	//Enable ADC
	//ADCSRA |= (1<<ADFR);	//ADC Free running mode
	ADCSRA |= (1<<ADPS1) | (1<<ADPS2);	//Preskaler (64)
}

uint8_t ADC_Read(uint8_t Channel)
{
	ADMUX = (ADMUX & 0xF8) | Channel;		//select channel for measurement
	ADCSRA |= (1<<ADSC);					//start conversion
	while(ADCSRA & (1<<ADSC));				//wait for conversion finish
	return ADCW >> 2;						//return ADC data register (reduced to 8 bit)
}

//
// Joystick functions
//
void Joy_SetIdleValues(Joystick_t *JoystickID)
{
	JoystickID->IdleValueX = ADC_Read(JoystickID->ADC_ChannelX);
	JoystickID->IdleValueY = ADC_Read(JoystickID->ADC_ChannelY);
}


void Joy_ReadXY (Joystick_t *JoystickID)
{
	JoystickID->ValueX = ADC_Read(JoystickID->ADC_ChannelX);
	JoystickID->ValueY = ADC_Read(JoystickID->ADC_ChannelY);
}

int8_t Joy_GetDeviationPercent(Joystick_t *JoystickID, uint8_t axis)
{
	int16_t Deviation;
	uint8_t CurrentValue;
	uint8_t IdleValue;

	if(axis == X_AXIS)
	{
		CurrentValue = JoystickID->ValueX;
		IdleValue = JoystickID->IdleValueX;
	}
	else if(axis == Y_AXIS)
	{
		CurrentValue = JoystickID->ValueY;
		IdleValue = JoystickID->IdleValueY;
	}
	else return 0;


	Deviation = CurrentValue - IdleValue;					//value of deviation (from idle value)
	if(Deviation >= 0)
	{
		return ( Deviation * 100) / (255 - IdleValue);		//formula for positive deviation percentage
	}
	else
	{
		return - (100-( (CurrentValue * 100) / IdleValue ));	//formula for negative deviation percentage
	}

}


uint8_t Joy_GetX (Joystick_t *JoystickID)
{
	return JoystickID->ValueX;
}

uint8_t Joy_GetY (Joystick_t *JoystickID)
{
	return JoystickID->ValueY;
}



