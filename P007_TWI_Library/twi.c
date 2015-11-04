/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * Project:		TWI (I2C) Library
 * Hardware:		Arduino UNO
 * Micro:			ATMEGA328P
 * IDE:			Atmel Studio 6.2
 *
 * Name:    		twi.c
 * Purpose: 		TWI (I2C) Library
 * Date:			21-10-2015
 * Version:		1.0	
 * Author:		Marcel van der Ven
 *
 * Release notes:	
 *
 * Note(s):
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/* Defines				                                                                  */
/************************************************************************/
#define F_CPU			16000000UL

/* Timeouts for the TWI protocol */
#define TIMEOUT_START						1000
#define TIMEOUT_ADDRESS_TRANSMISSION		1000

/* Result */
#define SUCCEEDED		0x00
#define TIMEOUT_ERROR	0x01
#define TWI_ERROR		0x02

/************************************************************************/
/* Includes				                                                                  */
/************************************************************************/
#include <avr/io.h>
#include "util/delay.h"
#include "twi.h"
#include "string.h"


/************************************************************************/
/* Structures				                                                                  */
/************************************************************************/
struct TWI_Bus
{
	volatile PinSettings sda;
	volatile PinSettings scl;
}twi;


/************************************************************************/
/* Functions				                                                                  */
/************************************************************************/	

/***************************************************************************
*  Function:		InitializeTwi()
*  Description:		Initializes TWI on the ATMEGA328P, also known as TWI on the Atmel.
*  Receives:		Nothing
*  Returns:		Nothing
***************************************************************************/
void InitializeTwi()
{
	
}

/***************************************************************************
*  Function:		SendStart()
*  Description:		Sends the start condition.
*  Receives:		Nothing
*  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
int SendStart(void)
{
	int error = SUCCEEDED;
	int counter = 0;
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	/* Wait for the start condition to be transmitted, timeout after the TIMEOUT value */
	while(!(TWCR & (1<<TWINT)))
	{
		_delay_us(10);
		counter++;
		
		if(counter == TIMEOUT_START)
			error = TIMEOUT_ERROR;
	}
	
	/* Check for errors */
	if((TWSR & 0xF8) != START)
		error = TWI_ERROR:
		
	return error;
}

/***************************************************************************
*  Function:		TransmitAddress()
*  Description:		Transmits the 7-bit + RW bit address.
*  Receives:		8-bit address and RW bit
*  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
void TransmitAddress(BYTE address)
{
	int error = SUCCEEDED;
	int counter = 0;
	
	/* Set address to transmit */
	TWDR = address;
	
	/* Transmit address */
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	/* Wait till the address is transmitted */
	while(!(TWCR & (1<<TWINT)))
	{
		_delay_us(10);
		counter++;
			
		if(counter == TIMEOUT_ADDRESS_TRANSMISSION)
			error = TIMEOUT_ERROR;
	}
		
	/* Check for errors */
	if((TWSR & 0xF8) != MT_SLA_ACK)
		error = TWI_ERROR;
		
	return error;		
}

/***************************************************************************
*  Function:		SendData()
*  Description:		Sends the data.
*  Receives:		Pointer to the data.
*  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
int SendData(BYTE data)
{
	int error = SUCCEEDED;
	int counter = 0;
	
	/* Load data */
	TWDR = data;
	
	/* Start transmission */
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	/* Wait till byte is transmitted */	
	while(!(TWCR & (1<<TWINT)))
	{
		_delay_us(10);
		counter++;
			
		if(counter == TIMEOUT_ADDRESS_TRANSMISSION)
			error = TIMEOUT_ERROR;
	}
		
	/* Check for errors */
	if((TWSR & 0xF8) != MT_DATA_ACK)
		error = TWI_ERROR:
		
	return error;
}

/***************************************************************************
*  Function:		SendStop()
*  Description:		Sends the stop condition.
*  Receives:		Nothing
*  Returns:		Nothing
***************************************************************************/
void SendStop(void)
{
	/* Send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

