/*
 * blink.c
 *
 *  Created on: 09.08.2015
 *      Author: Thomas Ludwig <thomas@ludwig.im>
 */

#include <stdio.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LED	0

int main (void)
{
	printf ("Raspberry Pi blink\n") ;

	wiringPiSetup () ;
	pinMode (LED, OUTPUT) ;

	for (;;)
	{
	digitalWrite (LED, HIGH) ;	// On
	delay (500) ;		// mS
	digitalWrite (LED, LOW) ;	// Off
	delay (500) ;
	}
	return 0 ;
}
