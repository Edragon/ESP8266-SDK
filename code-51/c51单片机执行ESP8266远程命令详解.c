 /***************************************************************************
c51单片机，远程命令详解

******************************************************************************/
#include <AT89X51.H>
#include <stdio.h>
unsigned int a;
int b=1;
int c =1;
int d =1;
int e=1;
int f=0;

void main(void)  
 {
	unsigned char ldat;
	 //PCON |=0X80;		//是否波特率倍增
	SCON=0x50;
	TMOD=0X20;				//设置定时器1为模式2，做为波特率发生器
	TL1=0xFD;				//9600波特率的初值,板子使用11.592M晶振，初值=256-12000000/32/12/2400
	TH1=0xFD;				//
	TI = 1;
	TR1=1;					//启动波特率发生
			//设置串口方式2
	 	do{
for (a=0; a<50000; a++);	//延时几秒，让模块有时间启动
printf ("AT+CIPMUX=1\n"); //发送命令AT+CIPMUX=1，让模块进入多连接模式
	if(RI){				//查询是否有数据接收
			ldat=SBUF;		//数据暂存于变量中
			RI=0;
				if (ldat=='K')//命令执行成功，就会返回OK,这里就是查询是否有K返回，若有K返回，就说明执行成功，可以执行下一条，否则继续循环
				{b=0 ;
				d=0;}
			   c++;
			if (c==1000)
			{c=1;
			d=0;}}

  }while (d);   
	}while(b) ;
b=1;
c =1;
d =1;//b、c、d复位，便于执行下一条命令



  	 	do{
for (a=0; a<10000; a++);
printf ("AT+CIPSERVER=1,7671\n"); 
 	do{
	if(RI){				//查询是否有数据接收
			ldat=SBUF;		//数据暂存于变量中
			RI=0;
				if (ldat=='K')//同上
				{b=0 ;
				d=0;}
			   c++;
			if (c==1000)
			{c=1;
			d=0;}}

  }while (d);   
	}while(b) ;	 
b=1;
c =1;
d =1;
	 	do{
for (a=0; a<10000; a++);
printf ("AT+CIPSTO=50\n"); 
 	do{
	if(RI){				//查询是否有数据接收
			ldat=SBUF;		//数据暂存于变量中
			RI=0;
				if (ldat=='K')//同上
				{b=0 ;
				d=0;}
			   c++;
			if (c==1000)
			{c=1;
			d=0;}}

  }while (d);   
	}while(b) ;
b=1;
c =1;
d =1;

//执行上述命令，模块进入服务模式，下面单片机就可以接收远程发来的数据

 	while(1)
	{	
	  	if(RI){				//查询是否有数据接收
			ldat=SBUF;		//数据暂存于变量中
			RI=0;			//重新清0等待接收
			
			
 //命令接收开始
			if (ldat=='}')	
			{ char k[50] ;//设置接收数据缓冲
   
   
   	 
   
   	   do{
	   
	   if(RI){				//查询是否有数据接收
			ldat=SBUF;		//数据暂存于变量中
			RI=0;			//重新清0等待接收
	   	  k[f]=ldat;//一个个字符接收
	f++	;
	if(ldat==0x0A){e=0;}	//若有新行字符，则结束接收，则说明命令接收完毕
		}
	   } while	(e)	;
   	 printf("%s\n", k);//执行远程发来的命令
	 f=0;
	 e=1;
			for (a=0; a<50; a++){
			k[a]=0;
			
			}
   	
  
}
//命令结束

		
			if (ldat==']')
{P1_4=1;
  printf ("AT+CIPSEND=0,1\n");
for (a=0; a<10000; a++);
printf ("A");
 }

 			if (ldat=='-')
{P1_4=!P1_4;

 }
	 			if (ldat=='[')
{P1_4=0;
  printf ("AT+CIPSEND=0,1\n");
for (a=0; a<10000; a++);
printf ("B");
 }

 			if (ldat=='{')
{
if(P1_4==0){
printf ("AT+CIPSEND=0,1\n");
for (a=0; a<10000; a++);
printf ("B");
}

else{
  printf ("AT+CIPSEND=0,1\n");
for (a=0; a<10000; a++);
printf ("A");
}
 }

	
		}
	}

	
}
