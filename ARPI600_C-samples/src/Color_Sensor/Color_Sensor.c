 
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
#define u8 unsigned char
#define u16 unsigned int


#define Din 0
#define S3 4
#define S2 3
#define S1 2
#define S0 1
// the event counter 
int   g_count = 0;    // count the frequecy
int   g_array[3];     // store the RGB value
int   g_flag = 0;     // filter of RGB queue
float g_SF[3];        // save the RGB Scale factor

// Select the filter color
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;
    
  if(Level02 != 0)
    Level02 = HIGH;
    
  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}
 void TSC_WB(int Level0, int Level1)  //White Balance
 {
   g_count = 0;
   g_flag ++;
   TSC_FilterColor(Level0, Level1);
 }
PI_THREAD(myThread)
{
	while(1)
	{
	  sleep(1);
    switch(g_flag)
    {
    case 0:
      printf("->WB Start\n");
      TSC_WB(LOW, LOW);              //Filter without Red         
      break;
    case 1:
      printf("->Frequency R=%ld\n",g_count);
      g_array[0] = g_count;
      TSC_WB(HIGH, HIGH);            //Filter without Green        
      break;    
    case 2:
      printf("->Frequency G=%ld\n",g_count);        
      g_array[1] = g_count;
      TSC_WB(LOW, HIGH);             //Filter without Blue
      break; 
    case 3:
      printf("->Frequency B=%ld\n",g_count);

      printf("->WB End\n");
      g_array[2] = g_count;
      TSC_WB(HIGH, LOW);             //Clear(no filter)   
      break;
    default:     
      g_count = 0;        
      break; 
    }

  }
}

void myInterrupt(void) {
  g_count ++ ;
}

int main(void)
{
unsigned int i;
if (wiringPiSetup() == -1)
exit(1);

if ( wiringPiISR (Din, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
}

int x = piThreadCreate (myThread) ;
if (x != 0)
printf("it didn¡¯t start\n");

pinMode(S0,OUTPUT);
pinMode(S1,OUTPUT);
pinMode(S2,OUTPUT);
pinMode(S3,OUTPUT);

digitalWrite(S0, LOW);// OUTPUT FREQUENCY SCALING 2%
digitalWrite(S1, HIGH); 
delay(5000);


   printf("R:%ld\n",g_array[0]);
   printf("G:%ld\n",g_array[1]);
   printf("B:%ld\n",g_array[2]);
   
   g_SF[0] = 255.0/ g_array[0];     //R Scale factor
   g_SF[1] = 255.0/ g_array[1] ;    //G Scale factor
   g_SF[2] = 255.0/ g_array[2] ;    //B Scale factor
   printf("R:%f\n",g_SF[0]);
   printf("G:%f\n",g_SF[1]);
   printf("B:%f\n",g_SF[2]); 
while(1)
{
	 g_flag = 0;

   printf("R:%ld\n",(int)(g_array[0]*g_SF[0]));
   printf("G:%ld\n",(int)(g_array[1]*g_SF[1]));
   printf("B:%ld\n",(int)(g_array[2]*g_SF[2]));
   
   delay(5000); 
}
return 0;
}