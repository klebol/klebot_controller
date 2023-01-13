/*
 * SPI.h
 *
 *      Author: Michal Klebokowski
 */
#ifndef SPI_H_
#define SPI_H_

//
//SPI pins
//
#define MOSI_DIR_REG DDRB
#define MOSI_PIN (1<<PB3)

#define MISO_DIR_REG DDRB
#define MISO_PIN (1<<PB4)

#define SCK_DIR_REG DDRB
#define SCK_PIN (1<<PB5)

#define SS_DIR_REG DDRB
#define SS_PIN (1<<PB2)

//
//SPI Write/Read functions
//

void SPI_Init (void);									//Initialization
void SPI_Send(uint8_t *Data, uint8_t Length);			//SPI sending function
void SPI_Read(uint8_t *DataStorage, uint8_t Length);	//SPI reading function

#endif /* SPI_H_ */
