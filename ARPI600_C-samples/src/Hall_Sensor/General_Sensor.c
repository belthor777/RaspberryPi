 
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


#define	Clock	27
#define	DataIn	28
#define	DataOut	29


#define    Wait1us        delayMicroseconds(1);
#define    Wait2us        delayMicroseconds(2);
#define    Wait4us        {Wait2us;Wait2us;}
#define    Wait8us        {Wait4us;Wait4us;}
#define    Wait10us  {Wait8us;Wait2us;}

unsigned int ADCSelChannel(unsigned char Channel)
{
	unsigned int ConvertValue;
	unsigned char i, Chan;
	unsigned char ConvertValueL, ConvertValueH;
	unsigned char delay;
 
 	ConvertValueL = ConvertValueH = 0; //初始化转换结果
 	delay = 0;
		digitalWrite(Clock ,0);
		Wait2us;
		Wait2us;
		Channel = Channel << 4;
		for (i = 0; i < 4; i ++) //输入需要转换的通道的编码
		{
			Chan = Channel;
			Chan = Chan >> 7;
			digitalWrite(DataIn,Chan & 0x01);
			Wait2us;
			digitalWrite(Clock ,1);
			digitalWrite(Clock ,0);
			Channel = Channel << 1;
		}
		for (i = 0; i < 6;i ++) //输入转换时钟
		{
			digitalWrite(Clock ,1);
			digitalWrite(Clock ,0);
		}
		{
			Wait10us;Wait10us;Wait10us;Wait10us;Wait10us;
			Wait10us;Wait10us;Wait10us;Wait10us;Wait10us;
		}
		{
			Wait10us;
			digitalWrite(Clock ,0);
			Wait1us;
			Wait1us;
			for (i = 0; i < 2; i ++) //读取高二位bit值
			{
				digitalWrite(Clock ,1);
				ConvertValueH <<= 1;
				if (digitalRead(DataOut))
				ConvertValueH |= 0x1;
				digitalWrite(Clock ,0);
				Wait1us;
			} 
			for (i = 0; i < 8; i ++) //读取低八位bit值
			{
				digitalWrite(Clock ,1);
				ConvertValueL <<= 1;
				if (digitalRead(DataOut))
				ConvertValueL |= 0x1;
				digitalWrite(Clock ,0);
				Wait1us;
			} 
			ConvertValue = ConvertValueH;
			ConvertValue <<= 8;
			ConvertValue |= ConvertValueL;
			return ConvertValue; //返回转换结果
		}
}


int main(){
	unsigned int re;
	unsigned char d1,d2,d3,d4;

if (wiringPiSetup() < 0)
{
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
}

pinMode (DataOut,INPUT);
pullUpDnControl(DataOut, PUD_UP);

pinMode (Clock,OUTPUT);
pinMode (DataIn,OUTPUT);


while(1)
{
  printf("AD: %d%d%d%d \n",d1,d2,d3,d4);
  re=ADCSelChannel(6);
  d1=re/1000;
  d2=re/100%10;
  d3=re/10%10;
  d4=re%10;
  delay (100);
  }

}
