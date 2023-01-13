/*
 * main.c
 *
 *  Created on: 19 sie 2022
 *      Author: Michal
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "nRF24/nRF24.h"
#include "nRF24/SPI/SPI.h"
#include "UART/uart.h"
#include "Joystick/joystick.h"
#include "klebot_transmiter.h"


#define KEY_UP (1<<PD5)
#define KEY_DOWN (1<<PD2)
#define KEY_RIGHT (1<<PD6)
#define KEY_LEFT (1<<PD4)

uint8_t address0[3] = {1,2,3};
uint8_t address1[3] = {3,2,1};
uint8_t payload[10];
uint8_t byte;
uint8_t i;
uint8_t SendPerm;
char UartData;


enum Motor {Front_L,Front_R,Rear_L,Rear_R,ALL};
enum Direction {Stop,Forward,Backward,Right,Left};

volatile uint8_t SoftTimer1;

void Debounce (volatile uint8_t *KPIN, uint8_t kmask,uint16_t rep_wait,uint8_t rep_time, uint8_t *output_flag);
void MakeFrame (uint8_t *frame, uint8_t operation, uint8_t value0, uint8_t value1,uint8_t value2, uint8_t value3, uint8_t value4);
void HW_Timer2Init(void);
void Joystick_monitor(void);

uint8_t KeyFlag;

uint8_t PWM = 250;

uint8_t Motor_number;
int main(void){

	DDRD &= ~(KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT);
	PORTD |= KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT;			//resistor pull-up

	DDRC |= (1<<PC5);
	PORTC &= ~(1<<PC5);

	PORTD |= (1<<PD3);
	EIMSK |= (1<<INT1);

	uart_init(__UBRR);
	SPI_Init();
	nRF24_Init();

	nRF24_SetRXAddress(0, address1);
	nRF24_SetTXAddress(address0);
	nRF24_TX_Mode();
	HW_Timer2Init();
	ADC_Init();



	_delay_ms(1);

	//nRF24_WriteTXPayload(payload1,3);

	Joy_SetIdleValues(&Joystick1);


	while(1){
		Joystick_monitor();

		UartData = uart_readc();

		Debounce(&PIND,KEY_UP,10,5,&KeyFlag);
		Debounce(&PIND,KEY_RIGHT,10,5,&KeyFlag);
		Debounce(&PIND,KEY_LEFT,10,5,&KeyFlag);
		Debounce(&PIND,KEY_DOWN,10,5,&KeyFlag);

		KleTransmiter_JoySteering(&Joystick1);

		//Joy_ReadXY(&Joy1);


		//_delay_ms(100);

		//uart_sends("X:");
		//uart_sendi(Joy1.ValueX);
		//uart_sends(" Y:");
		//uart_sendi(Joy1.ValueY);
		//uart_sends("\r\n");


		//JoystickSteering();

		if(KeyFlag || UartData)
		{


			if(UartData == 'g')
			{
				if(Motor_number > 0)
				{
					Motor_number--;
					uart_sends("Editing motor number: ");
					uart_sendi(Motor_number);
					uart_sends("\r\n");
					MakeFrame(payload,0,0,0,0,0,0);
				}
			}
			if(UartData == 't')
			{
				if(Motor_number < 3)
				{
					Motor_number++;
					uart_sends("Editing motor number: ");
					uart_sendi(Motor_number);
					uart_sends("\r\n");
					MakeFrame(payload,0,0,0,0,0,0);
				}
			}
			if(UartData == 'r')
			{
				if(PWM < 255)
				{
					PWM++;
					uart_sends("Motor ");
					uart_sendi(Motor_number);
					uart_sends(" PWM: ");
					uart_sendi(PWM);
					uart_sends("\r\n");
					MakeFrame(payload,2,Motor_number,PWM,0,0,0);

				}
			}

			if(UartData == 'f')
			{
				if(PWM > 0)
				{
					PWM--;
					uart_sends("Motor ");
					uart_sendi(Motor_number);
					uart_sends(" PWM: ");
					uart_sendi(PWM);
					uart_sends("\r\n");
					MakeFrame(payload,2,Motor_number,PWM,0,0,0);

				}
			}



			//uart_sendc(UartData);



			if(KeyFlag == KEY_UP)                //(UartData == 'w')
			{
				MakeFrame(payload,1,1,1,1,1,1);
			}
			else if(UartData == 's')
			{
				MakeFrame(payload,1,0,0,0,0,1);
			}
			else if(KeyFlag == KEY_LEFT) //(UartData == 'a')
			{
				MakeFrame(payload,3,3,0,0,0,0);
			}
			else if(KeyFlag == KEY_RIGHT) //(UartData == 'd')
			{
				MakeFrame(payload,3,4,0,0,0,0);
			}
			else if(KeyFlag == KEY_DOWN) //(UartData == 'x')
			{
				MakeFrame(payload,3,2,0,0,0,0);
			}

			KeyFlag = 0;
			//UartData = 0;
			nRF24_SendPacket(payload,7);
		}

		nRF24_Event();
	}
}

ISR(INT1_vect)
{

	nRF24_IRQ_Handler();
}

ISR(TIMER2_COMPA_vect)
{
	if(SoftTimer1)
	{
		SoftTimer1--;
	}

}

void nRF24_EventTxCallback(void)
{
	SendPerm = 0;
}

void HW_Timer2Init(void)
{
	TCCR2A |= (1<<WGM21);							//CTC Mode
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);		//preskaler 1024
	OCR2A = 155;										//Interrupt every 10ms (100Hz)
	TIMSK2 |= (1<<OCIE2A);							//Enable interrupt from Compare Match
	sei();											//Enable interrupts
}

void MakeFrame (uint8_t *frame, uint8_t operation, uint8_t value0, uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4)
{
	switch(operation)
	{
	case 1:				//movement
		*frame = 0xA1;
		break;

	case 2:				//PMW edit
		*frame = 0xA2;
		break;
	case 3:
		*frame = 0xA3;
		break;
	}
	frame++;
	*frame = value0;
	frame++;
	*frame = value1;
	frame++;
	*frame = value2;
	frame++;
	*frame = value3;
	frame++;
	*frame = value4;
}


void Debounce (volatile uint8_t *KPIN, uint8_t kmask,uint16_t rep_wait,uint8_t rep_time, uint8_t *output_flag){
	enum KS {idle,debounce,go_rep,wait_rep,rep};
	static enum KS key_state;
	static uint8_t last_key;
	uint8_t key_press;

	if(last_key && last_key != kmask) return; 							//anti-two button in the same time, if last key is not 0 and its different than last_key value, then return

	key_press = !(*KPIN & kmask);										//check if the key is pressed, store info in key_press variable

	if(key_press && !key_state)
	{																	//if button is pressed but was idle before
		key_state = debounce;											//set its state to being in debounce process
		SoftTimer1 = 3;		//for next 50ms

	}
	else if(key_state)
	{																	//if key state isnt idle
		if(key_press && key_state == debounce && !SoftTimer1)
		{																//if key is still pressed and state is debounce and the timer has finished counting 50ms
			key_state = go_rep;											//set state to go_rep
			last_key = kmask;											//confirm last_key value to current key
			SoftTimer1 = 1;												//wait another 30ms
		}
		else
		if(!key_press && key_state > debounce && key_state < rep)
		{																//if key is go_rep or wait_rep (waiting for confirm repeating proccess) and then suddenly has let gone

			key_state = idle;
			last_key = 0;
			*output_flag = kmask;
		}
		else
		if(key_press && key_state == go_rep && !SoftTimer1){
			key_state = wait_rep;
			if(!rep_time) rep_time = 20;
			if(!rep_wait) rep_wait = 150;
			SoftTimer1 = rep_wait;
		}
		else
		if(key_press && key_state == wait_rep && !SoftTimer1)
		{
			key_state = rep;
		}
		else
		if(key_press && key_state == rep && !SoftTimer1)
		{
			SoftTimer1 = rep_time;
			*output_flag = kmask;

		}
		else
		if(!key_press && key_state == rep)
		{
			key_state = idle;
			last_key = 0;
			*output_flag = 0;
		}
	}


}

void Joystick_monitor(void)
{
	uint8_t X = 0;
	uint8_t Y = 0;

	int8_t Xi = 0;
	int8_t Yi = 0;

	Joy_ReadXY(&Joystick1);

	X = Joy_GetX(&Joystick1);
	Y = Joy_GetY(&Joystick1);

	Xi = X - 125;
	Yi = Y - 130;

	uart_sendi(X);
	uart_sends("  ");
	uart_sendi(Y);
	uart_sendi(Joystick1.ValueX);
	uart_sends("  ");
	uart_sends("  ");
	uart_sendi(Joy_GetDeviationPercent(&Joystick1,X_AXIS));
	uart_sends("  ");
	uart_sendi(Joy_GetDeviationPercent(&Joystick1,Y_AXIS));

	uart_sends("\n\r");

}

