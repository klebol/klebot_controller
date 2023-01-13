/*
 * SPI.c
 *
 *      Author: Michal Klebokowski
 */

#include <avr/io.h>
#include "SPI.h"

void SPI_Init (void)
{
	MOSI_DIR_REG |= MOSI_PIN;
	MISO_DIR_REG |= MISO_PIN;
	SCK_DIR_REG |= SCK_PIN;
	SS_DIR_REG |= SS_PIN;

	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);				//SPI enable, Master mode, preskaler for sck (Fosc/16)
}

void SPI_Send(uint8_t *Data, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length ; i++)
	{
		SPDR = *Data;										//write data to hardware SPI buffer
		while( !( SPSR & (1<<SPIF) ) );						//wait for transfer complete
		Data++;												//increase pointer address for next iteration
	}
}


void SPI_Read(uint8_t *DataStorage, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		SPDR = 0x00;
		while( !( SPSR & (1<<SPIF) ) );						//make sure any data is being sent
		*DataStorage = SPDR;								//read harware SPI buffer
		DataStorage++;										//increase pointer address for next iteration
	}

}


