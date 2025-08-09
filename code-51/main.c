/*************	本地常量声明	**************/
#define MAIN_Fosc		22118400L	//定义主时钟
#define	RX1_Lenth		32			//串口接收缓冲长度
#define	BaudRate1		115200UL	//选择波特率
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 重装值， 对应300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 重装值， 对应300KHZ
#include	"STC15Fxxxx.H"
/*************	本地变量声明	**************/
u8	idata RX1_Buffer[RX1_Lenth];	//接收缓冲
u8	TX1_Cnt;	//发送计数
u8	RX1_Cnt;	//接收计数
bit	B_TX1_Busy;	//发送忙标志
/*************	端口引脚定义	**************/
sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P3^7;//LED3
sbit DK1=P3^3;//继电器
sbit BEEP=P3^4;//蜂鸣器
sbit K1=P1^3;//按键1
sbit K2=P1^2;//按键2
sbit K3=P1^4;//按键3
char led1bl,led2bl,led3bl;
unsigned char LYMS[13]={0x41,0x54,0x2B,0x43,0x57,0x4D,0x4F,0x44,0x45,0x3D,0x32,0x0D,0x0A};//AT+CWMODE=2 设置成路由模式
unsigned char SZLY[38]={0x41,0x54,0x2B,0x43,0x57,0x53,0x41,0x50,0x3D,0x22,0x45,0x53,0x50,0x38,0x32,0x36,0x36,0x22,0x2C,0x22,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x22,0x2C,0x31,0x31,0x2C,0x33,0x0D,0x0A};//AT+CWSAP="ESP8266","0123456789",11,0 设置路由
unsigned char RST[8]={0x41,0x54,0x2B,0x52,0x53,0x54,0x0D,0x0A};//AT+RST 重启
unsigned char SZDLJ[13]={0x41,0x54,0x2B,0x43,0x49,0x50,0x4D,0x55,0x58,0x3D,0x31,0x0D,0x0A};//AT+CIPMUX=1 设置成多连接
unsigned char KQFU[21]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x52,0x56,0x45,0x52,0x3D,0x31,0x2C,0x35,0x30,0x30,0x30,0x0D,0x0A};//AT+CIPSERVER=1,5000 开启TCP服务端口
unsigned char FSSJ[11]={0x41,0x54,0x2B,0x43,0x49,0x50,0x53,0x45,0x4E,0x44,0x3D};//AT+CIPSEND= 发送数据
void Delay1(unsigned long ms)
{
	unsigned char i, j,k;
	for(k=0;k<ms;k++)
	{
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
}
void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*100;i++);
}
void main(void)
{
	char i=0;
	B_TX1_Busy = 0;
	RX1_Cnt = 0;
	TX1_Cnt = 0;
	S1_8bit();				//8位数据
	S1_USE_P30P31();		//UART1 使用P30 P31口	默认
	AUXR &= ~(1<<4);	//Timer stop		波特率使用Timer2产生
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (u8)(Timer2_Reload >> 8);
	TL2 = (u8)Timer2_Reload;
	AUXR |=  (1<<4);	//Timer run enable
	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断
	P3M1 = 0x00;
  P3M0 = 0xFF;
	RX1_Cnt=0;
	DK1=0;
	BEEP=0;
	Delay2(1000);
	for(i=0;i<13;i++)//AT+CWMODE=2 设置成路由模式
	{
		SBUF=LYMS[i];Delay1(1);
	}
	Delay2(1000);
	for(i=0;i<38;i++)//AT+CWSAP="ESP8266","0123456789",11,0 设置路由
	{
		SBUF=SZLY[i];Delay1(1);
	}
	Delay2(1000);
	for(i=0;i<8;i++)//AT+RST 重启
	{
		SBUF=RST[i];Delay1(1);
	}
	Delay2(5000);
	for(i=0;i<13;i++)//AT+CIPMUX=1 设置成多连接
	{
		SBUF=SZDLJ[i];Delay1(1);
	}
	Delay2(2000);
	for(i=0;i<21;i++)//AT+CIPSERVER=1,5000 开启TCP服务端口
	{
		SBUF=KQFU[i];Delay1(1);
	}
	Delay2(2000);
	while (1)
	{
		if(K1==0)
		{
			Delay1(1);
			if(K1==0)
			{
				LED1=!LED1;
			}
			while(K1==0);
		}
		if(K2==0)
		{
			Delay1(1);
			if(K2==0)
			{
				LED2=!LED2;
			}
			while(K2==0);
		}
		if(K3==0)
		{
			Delay1(1);
			if(K3==0)
			{
				LED3=!LED3;
			}
			while(K3==0);
		}
	}
}

/********************* UART1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	char i,a,b,c;
	if(RI)
	{
		RI = 0;
		RX1_Buffer[RX1_Cnt] = SBUF;		//保存一个字节
		if(RX1_Buffer[0]==0x45)
		{
			RX1_Cnt++;
		}
		else
		{
			RX1_Cnt=0;
		}
		if(RX1_Cnt>=10)
		{
			if(RX1_Buffer[0]==0x45&&RX1_Buffer[1]==0x53&&RX1_Buffer[2]==0x50)
			{
				if(RX1_Buffer[4]==0x4C&&RX1_Buffer[5]==0x45&&RX1_Buffer[6]==0x44)//判断LED
				{
					if(RX1_Buffer[7]==0x31)//判断LED1
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							LED1=0;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							LED1=1;
						}
					}
					if(RX1_Buffer[7]==0x32)//判断LED2
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							LED2=0;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							LED2=1;
						}
					}
					if(RX1_Buffer[7]==0x33)//判断LED3
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							LED3=0;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							LED3=1;
						}
					}
				}
				if(RX1_Buffer[4]==0x4A&&RX1_Buffer[5]==0x44&&RX1_Buffer[6]==0x51)//判断继电器
				{
					if(RX1_Buffer[7]==0x31)//判断LED1
					{
						if(RX1_Buffer[3]==0x4B)//判断开
						{
							DK1=1;
						}
						if(RX1_Buffer[3]==0x47)//判断关
						{
							DK1=0;
						}
					}
				}
				if(RX1_Buffer[3]==0x46&&RX1_Buffer[4]==0x4D&&RX1_Buffer[5]==0x51&&RX1_Buffer[6]==0x43&&RX1_Buffer[7]==0x53)//判断蜂鸣器
				{
					BEEP=1;Delay2(100);BEEP=0;Delay2(100);BEEP=1;Delay2(100);BEEP=0;Delay2(100);
				}
				if(RX1_Buffer[3]==0x43&&RX1_Buffer[4]==0x58&&RX1_Buffer[5]==0x53&&RX1_Buffer[6]==0x4A)//查询数据
				{
					if(LED1==0){a=0x4B;}else{a=0x47;}
					if(LED2==0){b=0x4B;}else{b=0x47;}
					if(LED3==0){c=0x4B;}else{c=0x47;}
					for(i=0;i<11;i++)//AT+CIPSEND= 发送数据
					{
						SBUF=FSSJ[i];Delay1(1);
					}
					SBUF=0x30;Delay1(1);
					SBUF=0x2C;Delay1(1);
					SBUF=0x32;Delay1(1);
					SBUF=0x32;Delay1(1);
					SBUF=0x0D;Delay1(1);
					SBUF=0x0A;Delay1(1);
					
					SBUF=0x45;Delay1(1);
					SBUF=0x53;Delay1(1);
					SBUF=0x50;Delay1(1);
					SBUF=0x4C;Delay1(1);
					SBUF=0x45;Delay1(1);
					SBUF=0x44;Delay1(1);
					SBUF=0x31;Delay1(1);
					SBUF=a;Delay1(1);
					SBUF=0x4C;Delay1(1);
					SBUF=0x45;Delay1(1);
					SBUF=0x44;Delay1(1);
					SBUF=0x32;Delay1(1);
					SBUF=b;Delay1(1);
					SBUF=0x4C;Delay1(1);
					SBUF=0x45;Delay1(1);
					SBUF=0x44;Delay1(1);
					SBUF=0x33;Delay1(1);
					SBUF=c;Delay1(1);
					SBUF=0x50;Delay1(1);
					SBUF=0x53;Delay1(1);
					SBUF=0x45;Delay1(1);
					//45 53 50 4C 45 44 31 4B 4C 45 44 32 4B 4C 45 44 33 4B 50 53 45 
				}
				RX1_Cnt=0;
			}
			else
			{
				RX1_Cnt=0;
			}
			RX1_Cnt=0;
		}
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}
