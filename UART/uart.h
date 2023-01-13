/*
 * uart.h
 *
 *  Created on: 18 lip 2022
 *      Author: Michal
 */

#ifndef UART_H_
#define UART_H_

#define UART_BAUD 9600
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)

#define UART_RX_BUF_SIZE 16
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE - 1)
#define UART_TX_BUF_SIZE 16
#define UART_TX_BUF_MASK (UART_TX_BUF_SIZE - 1)


volatile uint8_t UartRxBuffer[UART_RX_BUF_SIZE];
volatile uint8_t UartRxHead;
volatile uint8_t UartRxTail;
volatile uint8_t UartTxBuffer[UART_TX_BUF_SIZE];
volatile uint8_t UartTxHead;
volatile uint8_t UartTxTail;

void uart_init(uint16_t ubrr);							//Initialization for UART
void uart_sendc(char data);								//Send one character
void uart_sends(char* StringElement);					//Send string
void uart_sendi(uint16_t value);						//Send integer
char uart_receivec( void );								//Receive one char
char uart_readc(void);								//Read received data from software buffer
void uart_writec(char data);						//Send data via software buffer
void uart_writes(char* StringElement);

#endif /* UART_H_ */
