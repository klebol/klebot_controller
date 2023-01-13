/*
 * uart.c
 *
 *  Created on: 18 lip 2022
 *      Author: Michal
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"

void uart_init( uint16_t ubrr)
{
	UBRR0H = (uint8_t)(ubrr>>8);			//set baud rate
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);			//Receiver and transmiter enable
	UCSR0B |= (1<<RXCIE0); 					//Reveive complete - flag (RXC0) interrupt enable
	UCSR0C = (3<<UCSZ00);					//Set frame format, 8-bit data, 1 stop bit
	sei();									//global interrupt enable
}

void uart_sendc(char data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );		//wait for empty hardware buffer
	UDR0 = data;							//write data to hardware buffer
}

void uart_sends(char* StringElement)
{
	while(*StringElement > 0)
	{
		uart_sendc(*StringElement);
		StringElement++;
	}
}

void uart_sendi(uint16_t value)
{
	char buffer[5];
	itoa(value,buffer,10);
	uart_sends(buffer);
}

char uart_receivec( void )
{
	while ( !(UCSR0A & (1<<RXC0)) );		//wait for receive complete flag
	return UDR0;							//return data
}



/* DATA RECEIVE = INTERRUPT(RECEIVE AND STORE IN BUFFER) + READ BUFF FUNCTION(READ FROM BUFFER)
 * When data is received, RXC0 bit in UCSR0A register is set, that causes this interrupt
 * Received data is stored in circular buffer (UartRXBuffer[]). Firt data is placed on the
 * firts position (UartRxHead + 1), position zero is in fact position 32 because mask is clearing
 * buffer's head at value 32. When buffer is full (head = tail) new data cant be stored
 * Stored data can be read using uart_read_buff function which returns oldest unread data */
ISR(USART_RX_vect)
{
	char Data;
	uint8_t TmpHead;
	Data = UDR0;									// Get data from hardware buf
	TmpHead = (UartRxHead + 1) & UART_RX_BUF_MASK; 	// Increase buffer head, AND with mask for clearing TmpHead when its reach buffer's max (31 + 1 -> 0 for 32 size))
	if(TmpHead == UartRxTail)						// Check if buffer is full
	{

	}
	else
	{
		UartRxHead = TmpHead;						// Update head value
		UartRxBuffer[UartRxHead] = Data;			// Write data to circular buffer
	}
}

char uart_readc(void)
{
	if(UartRxTail == UartRxHead)						//if no new data, return 0
	{
		return 0;
	}
	UartRxTail = (UartRxTail + 1) & UART_RX_BUF_MASK; 	//Increase buffer tail, AND with mask for clearing UartRxTail when its reach buffer's max (31 + 1 -> 0 for 32 size)
	return UartRxBuffer[UartRxTail];
}

/* DATA SEND = WRITE BUFF FUNCTION (WRITE DATA TO BUFFER) + INTERRUPT (SEND DATA FROM BUFFER)
 * Similar mechanism to receive mechanism 													*/

ISR(USART_UDRE_vect)
{
	if(UartTxTail != UartTxHead)							//if there is data to send...
	{
		UartTxTail = (UartTxTail + 1) & UART_TX_BUF_MASK;	//increase tail value
		UDR0 = UartTxBuffer[UartTxTail];					//send data to hardware buffer
	}
	else													//if there is no data to send ...
	{
		UCSR0B &= ~(1<<UDRIE0);								//disable this interrupt
	}
}

void uart_writec (char data)
{
	uint8_t TmpHead;
	TmpHead = (UartTxHead + 1) & UART_TX_BUF_MASK;			//new head value
	if(TmpHead == UartTxTail)								//if there is no space for new data to send
	{

	}
	else 													//if there is free space in buffer for new data...
	{
		UartTxHead = TmpHead;								//save new head value
		UartTxBuffer[UartTxHead] = data;					//write data to Tx Buffer
		UCSR0B |= (1<<UDRIE0);								//enable interrupt (from Data register empty) for sending data out
	}
}

void uart_writes(char* StringElement)
{
	while(StringElement > 0)
	{
		uart_writec(*StringElement);
		StringElement++;
	}
}


