      
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

#include<wiringPi.h>

#define Din 0
#define u8 unsigned char
#define   DHT11_IO_OUT() pinMode(Din,OUTPUT)
#define   DHT11_DQ_OUT_HIGH   digitalWrite(Din ,1)
#define   DHT11_DQ_OUT_LOW   digitalWrite(Din ,0)
#define   DHT11_IO_IN()  {pinMode(Din,INPUT);pullUpDnControl(Din,PUD_UP);}
#define   DHT11_DQ_IN digitalRead(Din)
void delay_ms(unsigned int i) 
{
	delay(i);
}
void delay_us(unsigned int i) 
{
	delayMicroseconds(i);
}

//Reset DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT_LOW; 	//GPIO.0=0
    delay_ms(20);    	//Pull down Least 18ms
    DHT11_DQ_OUT_HIGH; 	//GPIOA.0=1 
	delay_us(30);     	//Pull up 20~40us
}

u8 DHT11_Check(void) 	   
{ 	
	u8 retry=0;
/*	DHT11_IO_IN();//SET INPUT	 

	while(!digitalRead(Din))
	retry=1;
	while (digitalRead(Din));
	
	return retry;*/
	
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11 Pull down 40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)
		return 1;
	else 
		retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11 Pull up 40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
		return 1;//chack error	    
	return 0;
	
}

u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//wait become Low level
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//wait become High level
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//wait 40us
	if(DHT11_DQ_IN)
		return 1;
	else 
		return 0;		   
}

u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

u8 DHT11_Read_Data(u8 *temperature,u8 *humidity)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humidity=buf[0];
			*temperature=buf[2];
		}
	}
	else 
		return 1;
	return 0;	    
}
	 
u8 DHT11_Init(void)
{	     
	DHT11_Rst();  
	return DHT11_Check();
} 


int main(void)
{
	u8 temperature;  	    
	u8 humidity; 

if (wiringPiSetup() < 0)
{
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
}
	printf("Waveshare!\r\n");
	
	while(DHT11_Init());
  	while(1)
	{
		DHT11_Read_Data(&temperature,&humidity);	
		printf("temperature=%d\r\n",temperature);
		printf("humidity=%d\r\n",humidity);
 		delay_ms(100);
	}
}





