/*
 * klebot_transmiter.h
 *
 *  Created on: 26 wrz 2022
 *      Author: miqix
 */

#ifndef KLEBOT_TRANSMITER_H_
#define KLEBOT_TRANSMITER_H_

#include "Joystick/joystick.h"

typedef enum {
	StopMove,
	DriveForwardStraight,
	DriveBackwardStraight,
	DriveForwardLeft,
	DriveForwardRight,
	DriveBackwardLeft,
	DriveBackwardRight,
	RotateLeft,
	RotateRight
}Klebot_MovementPattern_t;


void KleTransmiter_SendMovementPatternCommand(Klebot_MovementPattern_t patternID);
void KleTransmiter_JoySteering(Joystick_t *JoystickID);

#endif /* KLEBOT_TRANSMITER_H_ */
