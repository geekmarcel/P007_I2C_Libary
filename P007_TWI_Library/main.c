/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * Project: 		Post 7 TWI Library
 * Hardware:		Arduino UNO
 * Micro:			ATMEGA328P
 * IDE:			Atmel Studio 6.2
 *
 * Name:    		main.c
 * Purpose: 		TWI Library Example
 * Date:			21-10-2015
 * Author:		Marcel van der Ven
 *
 * Hardware setup:		
 *
 * Note(s):
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/* Defines				                                                                  */
/************************************************************************/
#define F_CPU	16000000UL

/************************************************************************/
/* Includes				                                                                  */
/************************************************************************/
#include <avr/io.h>
#include "util/delay.h"
#include "common.h"

/***************************************************************************
*  Function:		Setup()
*  Description:		
*  Receives:		Nothing
*  Returns:		Nothing
***************************************************************************/
void Setup()
{
	/* Setup PORTD */
	/* Set all pins as output for now */
	//DDRD = 0b11111111;
}

/***************************************************************************
*  Function:		Main(void)
*  Description:		Main function of the program.
*  Receives:		Nothing
*  Returns:		Optionally the exit code.
***************************************************************************/
int main(void)
{
	/* Setup and initialization */
	Setup();
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}