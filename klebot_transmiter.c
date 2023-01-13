/*
 * klebot_transmiter.c
 *
 *  Created on: 26 wrz 2022
 *      Author: miqix
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "klebot_transmiter.h"
#include "nRF24/nRF24.h"
#include "nRF24/SPI/SPI.h"
#include "UART/uart.h"




void KleTransmiter_SendMovementPatternCommand(Klebot_MovementPattern_t patternID)
{
	uint8_t BytesToSend[2];
	BytesToSend[0] = 0xA3;
	BytesToSend[1] = patternID;
	nRF24_SendPacket(BytesToSend,2);
}


void KleTransmiter_JoySteering(Joystick_t *JoystickID)
{
	int8_t DeviationX, DeviationY;
	DeviationX = Joy_GetDeviationPercent(JoystickID, X_AXIS);
	DeviationY = Joy_GetDeviationPercent(JoystickID, Y_AXIS);

	if(abs(DeviationX) < 10)
	{
		if(DeviationY > 85)
		{
			KleTransmiter_SendMovementPatternCommand(RotateRight);
		}
		else if(DeviationY < -85)
		{
			KleTransmiter_SendMovementPatternCommand(RotateLeft);
		}
		else if(abs(DeviationY) < 10)
		{
			return;
		}
	}
	else if(DeviationX > 85)
	{
		if(DeviationY > 85)
		{
			KleTransmiter_SendMovementPatternCommand(DriveForwardRight);
		}
		else if(DeviationY < -85)
		{
			KleTransmiter_SendMovementPatternCommand(DriveForwardLeft);
		}
		else if(abs(DeviationY) < 10)
		{
			KleTransmiter_SendMovementPatternCommand(DriveForwardStraight);
		}
	}
	else if(DeviationX < -85)
	{
		if(DeviationY > 85)
		{
			KleTransmiter_SendMovementPatternCommand(DriveBackwardRight);
		}
		else if(DeviationY < -85)
		{
			KleTransmiter_SendMovementPatternCommand(DriveBackwardLeft);
		}
		else if(abs(DeviationY) < 10)
		{
			KleTransmiter_SendMovementPatternCommand(DriveBackwardStraight);
		}
	}




}
