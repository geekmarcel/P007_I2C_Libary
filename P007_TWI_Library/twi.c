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
/* Defines
/************************************************************************/
#define F_CPU			16000000UL


/************************************************************************/
/* Includes
/************************************************************************/
#include <avr/io.h>
#include "util/delay.h"
#include "twi.h"
#include "string.h"
#include <avr/interrupt.h>

/************************************************************************/
/* Structures				                                
/************************************************************************/
struct TWI_Bus
{
	volatile PinSettings sda;
	volatile PinSettings scl;
	BYTE slaveAddress;
	TwiModeType mode;
	
	BOOL twiInitialized;
}twi;


/************************************************************************/
/* Interrupts
/************************************************************************/

/***************************************************************************
  Function:		ISR(TWI_vect)
  Description:	Interrupt service routine for the TWI interrupt.
  Receives:		Nothing
  Returns:		Nothing
***************************************************************************/
ISR(TWI_vect)
{
	
}


/************************************************************************/
/* Functions				                                            
/************************************************************************/	

/***************************************************************************
*  Function:		InitializeTwi()
*  Description:		Initializes TWI on the ATMEGA328P, also known as TWI on the Atmel.
*  Receives:		Nothing
*  Returns:		Nothing
***************************************************************************/
void InitializeTwi(PrescalarType prescalar, TwiSpeedType speed)
{
	/* Set prescalar bits */
	TWSR |= prescalar;
	
	/* Set SCL Frequency */
	switch(speed)
	{
		case LOW_SPEED:
			TWBR = (F_CPU / 2 * 10000) - 8 * prescalar;
			break;
		case STANDARD_MODE:
			TWBR = (F_CPU / 2 * 100000) - 8 * prescalar;
			break;
		case FAST_MODE:
			TWBR = (F_CPU / 2 * 400000) - 8 * prescalar;
			break;
		case FAST_MODE_PLUS:
			TWBR = (F_CPU / 2 * 1000000) - 8 * prescalar;
			break;
		case HIGH_SPEED_MODE:
			TWBR = (F_CPU / 2 * 3400000) - 8 * prescalar;
			break;
	}
	
	
}

/***************************************************************************
*  Function:		SetMode(TwiMode mode)
*  Description:		Sets the TWI Mode, Master/Slave and Transmitter or Receiver.
*				For the slave
*  Receives:		The mode to set.
*  Returns:		Nothing
***************************************************************************/
void SetMode(TwiModeType mode, BOOL respondToGeneralCall)
{	
	if(twi.twiInitialized == TRUE)
	{
		twi.mode = mode;

		switch (mode)
		{
			case MASTER_TRANSMITTER:
				/* Do Nothing */
				break;
			case MASTER_RECEIVER:
				/* Do nothing */
				break;
			case SLAVE_TRANSMITTER:
		
				/* Set the slave address and enable the general call address (address 0x00) if set */
				/* Then the LSB of the TWAR register should be set */
				TWAR = (address << 1) | respondToGeneralCall;
			
				/* Save own address */
				twi.slaveAddress = address;
			
				/* Set TWCR into slave mode */
				TWCR = (1 << TWEA) | (1 << TWEN);
			
				break;		
			case SLAVE_RECEIVER:
		
		
				break;
		}	
	}
}

/***************************************************************************
  Function:		TwiSend1Byte(void)
  Description:	Wrapper for sending 1 byte to the device with the given address.
  Receives:		BYTE address			:		Address of TWI device on the bus.
				BYTE* bytesToSend		:		Pointer to an array with the bytes to send.
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
void TwiSend1Byte(BYTE address, BYTE byteToSend)
{
	/* Check if TWI is initialized */
	if(twi.twiInitialized != TRUE)
		return;
		
	BYTE byteArray[1] = {byteToSend};
	
	return TwiSendBytes(address, byteArray, 1);
}

/***************************************************************************
  Function:		TwiSendBytes(void)
  Description:	Sends the given number of bytes over TWI.
  Receives:		BYTE* bytesToSend		:		Pointer to an array with the bytes to send.
				BYTE numberOfBytes		:		Number of bytes to send.
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
void TwiSendBytes(BYTE address, BYTE* bytesToSend, BYTE numberOfBytes)
{
	/* Check if TWI is initialized */
	if(twi.twiInitialized != TRUE)
		return;
		
	BYTE result = SUCCEEDED;
	
	/* Send start condition */
	result = SendStart();
	if(result != SUCCEEDED)
		return result;
		
	/* Send address of device */
	TransmitAddress(address);
	if(result != SUCCEEDED)
		return result;
	
	/* Send data bytes */
	for(int i = 0; i < numberOfBytes; i++)
	{
		result = SendData(bytesToSend[i]);
		if(result != SUCCEEDED)
			return result;
	}	
		
	/* Send stop condition */
	SendStop();
	if(result != SUCCEEDED)
		return result;
}

/***************************************************************************
  Function:		TwiSend(void)
  Description:	Sends the given number of bytes over TWI.
  Receives:		BYTE* bytesToSend		:		Pointer to an array with the bytes to send.
				BYTE numberOfBytes		:		Number of bytes to send.
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
void TwiRead(BYTE* bytesToSend, BYTE numberOfBytes)
{
	
}

/***************************************************************************
  Function:		SendStart(void)
  Description:	Sends the start condition.
  Receives:		Nothing
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
BYTE SendStart(void)
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
	if((TWSR & 0xF8) != START_SEND_STATUS)
		error = TWI_ERROR;
		
	return error;
}

/***************************************************************************
  Function:		TransmitAddress()
  Description:	Transmits the 7-bit + RW bit address.
  Receives:		8-bit address and RW bit
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
BYTE TransmitAddress(BYTE address)
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
	if((TWSR & 0xF8) != ADDRESS_WRITE_ACK_STATUS && (TWSR & 0xF8) != ADDRESS_READ_ACK_STATUS)
		error = TWI_ERROR;
		
	return error;		
}

/***************************************************************************
  Function:		SendData()
  Description:	Sends the data.
  Receives:		Pointer to the data.
  Returns:		Integer indicating success (0x00), timeout error (0x01) or TWI error (0x02).
***************************************************************************/
BYTE SendData(BYTE data)
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
	if((TWSR & 0xF8) != DATA_SEND_ACK_STATUS && (TWSR & 0xF8) != DATA_RECEIVED_ACK_STATUS)
		error = TWI_ERROR;
		
	return error;
}

/***************************************************************************
  Function:		SendStop()
  Description:	Sends the stop condition.
  Receives:		Nothing
  Returns:		Nothing
***************************************************************************/
void SendStop(void)
{
	/* Send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

