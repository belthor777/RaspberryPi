 
/* 
             define from  wiringPi.h                     define from Board DVK511
                 3.3V | | 5V               ->                 3.3V | | 5V
                8/SDA | | 5V               ->                  SDA | | 5V
                9/SCL | | GND              ->                  SCL | | GND
                    7 | | 15/TX            ->                  IO7 | | TX
                  GND | | 16/RX            ->                  GND | | RX
                    0 | | 1                ->                  IO0 | | IO1
                    2 | | GND              ->                  IO2 | | GND
                    3 | | 4                ->                  IO3 | | IO4
                  VCC | | 5                ->                  VCC | | IO5
              MOSI/12 | | GND              ->                 MOSI | | GND
              MISO/13 | | 6                ->                 MISO | | IO6
               SCK/14 | | 10/CE0           ->                  SCK | | CE0
                  GND | | 11/CE1           ->                  GND | | CE1
              30/ID_SD| | 31/ID_SC         ->                ID_SD | | ID_SC
                    21| | GND              ->                  SDA | | 5V
                    22| | 26               ->                  SCL | | GND
                    23| | GND              ->                  IO7 | | TX
                    24| | 27               ->                  GND | | RX
                    25| | 28               ->                  IO0 | | IO1
                   GND| | 29               ->                  IO2 | | GND
                  */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>


// Use GPIO Pin 17, which is Pin 0 for wiringPi library

#define SIA 0
#define SIB 1
#define SW  2
// the event counter 
volatile int turn_flag;

// -------------------------------------------------------------------------
// myInterrupt:  called every time an event occurs
void myInterrupt0(void) {
   turn_flag=1;
}
void myInterrupt1(void) {
   turn_flag=2;
}
void myInterrupt2(void) {
   turn_flag=3;
}
// -------------------------------------------------------------------------
// main
int main(void) {
  // sets up the wiringPi library
  if (wiringPiSetup () < 0) {
      fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
      return 1;
  }

  // set Pin 17/0 generate an interrupt on high-to-low transitions
  // and attach myInterrupt() to the interrupt
  if ( wiringPiISR (SIA, INT_EDGE_BOTH, &myInterrupt0) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
 if ( wiringPiISR (SIB, INT_EDGE_BOTH, &myInterrupt1) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
  if ( wiringPiISR (SW, INT_EDGE_FALLING, &myInterrupt2) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
  
 /* 
    pinMode (dot,OUTPUT);

    digitalWrite(dot ,0);
    delay(500);
    digitalWrite(dot ,1);
    delay(500);
    digitalWrite(dot ,0);
*/
  	while (1)
   { 
		if(turn_flag)
		{
			switch(turn_flag)
			{
				case 1:	printf("Turn right!\r\n");break;
				case 2:	printf("Turn left!\r\n");break;
				case 3:	printf("Turn down!\r\n");break;
				default:break;
			}
			delay(500);
			turn_flag=0;
		}
	 }
	
  return 0;
}