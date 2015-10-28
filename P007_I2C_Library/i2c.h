/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * Project: 		I2C Library
 * Hardware:		Arduino UNO
 * Micro:			ATMEGA328P
 * IDE:			Atmel Studio 6.2
 *
 * Name:    		i2c.h
 * Purpose: 		I2C Library Header
 * Date:			21-10-2015
 * Author:		Marcel van der Ven
 *
 * Hardware setup:	
 *
 * Note(s):
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/


#ifndef I2C_H_
#define I2C_H_


#include "common.h"

/************************************************************************/
/* Defines				                                                                  */
/************************************************************************/


/************************************************************************/
/* Type Definitions			                                                                  */
/************************************************************************/


/************************************************************************/
/* API					                                                                  */
/************************************************************************/
int SendStart(void);
void TransmitAddress(BYTE address);
int SendData(BYTE data);
void SendStop(void);




#endif /* I2C_H_ */