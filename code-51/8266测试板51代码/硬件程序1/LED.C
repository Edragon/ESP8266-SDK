#include	"STC15Fxxxx.H"
#define uchar unsigned char
#define MAIN_Fosc		22118400L
sbit led=P1^0;
sbit led1=P1^1;
sbit led2=P3^7;
sbit led3=P3^4;
void delay(unsigned int t)
{unsigned int i;
   for(t;t>0;t--)
   for(i=100;i>0;i--);
                   }
main()
{unsigned int time=2000;

 while(1)
 {led=~led;
  delay(time);
  led1=~led1;
  delay(time);
  led2=~led2;
  delay(time);
  led3=~led3;
 }
}				    


