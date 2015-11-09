/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * Project: 		TWI (I2C) Library
 * Hardware:		Arduino UNO
 * Micro:			ATMEGA328P
 * IDE:			Atmel Studio 6.2
 *
 * Name:    		twi.h
 * Purpose: 		TWI (I2C) Library Header
 * Date:			21-10-2015
 * Author:		Marcel van der Ven
 *
 * Hardware setup:	
 *
 * Note(s):
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/


#ifndef TWI_H_
#define TWI_H_


#include "common.h"

/************************************************************************/
/* Defines				                                                                  */
/************************************************************************/

/* Status codes Master mode */
#define		START_SEND_STATUS			0x08
#define		REPEATED_START_SEND_STATUS	0x10
#define		ADDRESS_WRITE_ACK_STATUS	0x18
#define		ADDRESS_WRITE_NACK_STATUS	0x20
#define		DATA_SEND_ACK_STATUS		0x28
#define		DATA_SEND_NACK_STATUS		0x30
#define		LOST_ARBITRATION_STATUS		0x38
#define		ADDRESS_READ_ACK_STATUS		0x40
#define		ADDRESS_READ_NACK_STATUS	0x48
#define		DATA_RECEIVED_ACK_STATUS	0x50
#define		DATA_RECEIVED_NACK_STATUS	0x58

/* Status codes Slave mode */
#define		ACK_OWN_ADDRESS_STATUS				0x60
#define		SLAVE_ARBITRATION_LOST_STATUS		0x68
#define		ACK_GENERAL_CAL_STATUS				0x70


/* TWI Prescalars */
#define		PRESCALAR_1					0x01
#define		PRESCALAR_4					0x04
#define		PRESCALAR_16				0x16
#define		PRESCALAR_64				0x64

/* Timeouts for the TWI protocol */
#define TIMEOUT_START						1000
#define TIMEOUT_ADDRESS_TRANSMISSION		1000

/* Result */
#define SUCCEEDED		0x00
#define TIMEOUT_ERROR	0x01
#define TWI_ERROR		0x02

/************************************************************************/
/* Type Definitions			                                                                  */
/************************************************************************/
enum TwiMode {MASTER_TRANSMITTER,MASTER_RECEIVER,SLAVE_TRANSMITTER,SLAVE_RECEIVER};
enum Prescalar{PRESCALAR_1,PRESCALAR_4,PRESCALAR_16,PRESCALAR_64};
enum TwiSpeed{LOW_SPEED, STANDARD_MODE, FAST_MODE, FAST_MODE_PLUS, HIGH_SPEED_MODE};

/************************************************************************/
/* API					                                                                  */
/************************************************************************/
void InitializeTwi(Prescalar prescalar, TwiSpeed speed);
void SetMode(TwiMode mode, BOOL respondToGeneralCall);
void TwiSend1Byte(BYTE address, BYTE byteToSend);
void TwiSendBytes(BYTE address, BYTE* bytesToSend, BYTE numberOfBytes);

/* Low Level */
int SendStart(void);
void TransmitAddress(BYTE address);
int SendData(BYTE data);
void SendStop(void);




#endif /* TWI_H_ */