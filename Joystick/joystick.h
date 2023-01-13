/*
 * joystick.h
 *
 *  Created on: 22 wrz 2022
 *      Author: miqix
 */

#ifndef JOYSTICK_JOYSTICK_H_
#define JOYSTICK_JOYSTICK_H_

//
//ADC Channels definitions
//

#define JOY1_ADC_CHANNEL_X 0
#define JOY1_ADC_CHANNEL_Y 1
#define JOY2_ADC_CHANNEL_X 2
#define JOY2_ADC_CHANNEL_Y 3


//
// Axis defs
//
#define X_AXIS 0
#define Y_AXIS 1

//
//Joystick structure
//

typedef struct{
	uint8_t ValueX;
	uint8_t ValueY;
	uint8_t IdleValueX;
	uint8_t IdleValueY;
	uint8_t ADC_ChannelX;
	uint8_t ADC_ChannelY;
}Joystick_t;


//
//Joystick's declaration
//
Joystick_t Joystick1;
Joystick_t Joystick2;

//
//ADC MEASUREMENT FUNCTIONS (Edit for your MCU)
//
void ADC_Init(void);
uint8_t ADC_Read(uint8_t Channel);

//
//Joystick functions
//

void Joy_ReadXY (Joystick_t *JoystickID);								//Read data from ADC and write it to Joystick structure (use in main loop)
void Joy_SetIdleValues(Joystick_t *JoystickID);							//Read data from ADC at the begging of program to measure Joystick idle position
int8_t Joy_GetDeviationPercent(Joystick_t *JoystickID, uint8_t axis);	//Calculate actual deviation from idle value in percents (0 - 100)
uint8_t Joy_GetX (Joystick_t *JoystickID);								//Get actual X-axis value
uint8_t Joy_GetY (Joystick_t *JoystickID);								//Get actual Y-axis value



#endif /* JOYSTICK_JOYSTICK_H_ */
