/**********************RC522实验开发板例程************************
*  CPU:STC89C52RC
*  晶振:11.0592MHZ

******************************************************************/
//实验目的：
/*-----------------------------------------------
  名称：MF RC522
  内容：通过标准程序静态显示字符
  引脚定义如下：
sbit     MF522_NSS  = P1^7;			//SDA
sbit     MF522_SCK  = P1^6;
sbit     MF522_SI   = P1^5;
sbit     MF522_SO   = P1^4;
sbit     MF522_RST  = P1^3;                  
//蜂鸣器OR指示灯
sbit     LED_GREEN  =    P2^3; 
------------------------------------------------*/
#include "lpcreg.h"
#include "main.h"
#include "mfrc522.h"


//#include <string.h>


typedef unsigned char uchar;
typedef unsigned int uint;





unsigned int PWN_Value;				//定义pwn值
unsigned char order = 0;

unsigned char g_ucTempbuf[20];

                        
void delay1(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	for(y=110;y>0;y--);	
} 


void main( )
{    
	 
     unsigned char status,i;
	 unsigned char id[4];

	 delay1(200);

     InitializeSystem();
     PcdReset();
     PcdAntennaOff();
	 delay1(10); 
     PcdAntennaOn();
	  
     while ( 1 )
     {   


    	status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
        if (status != MI_OK)
        {    
       		InitializeSystem();
           	PcdReset();
     		PcdAntennaOff();
			delay1(10); 
     		PcdAntennaOn(); 
			continue;
        }
			     
			
       	status = PcdAnticoll(g_ucTempbuf);//防冲撞
        if (status != MI_OK) {    
			continue;    
		}

		for(i = 0; i < 4; i++) {	 		//将IC卡序列号存取到id中
			id[i] = g_ucTempbuf[i];
		}

		if((id[0] == 0xBB && id[1] == 0x88 && id[2] == 0x1A && id[3] == 0xD3) || 
		   (id[0] == 0xB3 && id[1] == 0x44 && id[2] == 0x70 && id[3] == 0x36) ||
		   (id[0] == 0x15 && id[1] == 0xE2 && id[2] == 0x8E && id[3] == 0x15) ||
		   (id[0] == 0x2A && id[1] == 0xA9 && id[2] == 0x74 && id[3] == 0x19) ||
		   (id[0] == 0x52 && id[1] == 0xD4 && id[2] == 0x6C && id[3] == 0xFC) ||
		   (id[0] == 0xB5 && id[1] == 0x44 && id[2] == 0x90 && id[3] == 0x15) ||
		   (id[0] == 0x32 && id[1] == 0x9F && id[2] == 0x13 && id[3] == 0x64) ||
		   (id[0] == 0x85 && id[1] == 0x58 && id[2] == 0x90 && id[3] == 0x15) ||
		   (id[0] == 0x95 && id[1] == 0x35 && id[2] == 0x7B && id[3] == 0x15)
		) {
			doorOpen();

			delay1(4000);	  //等待2s

			servoRet();
		}

		PcdHalt();
    }
}


/////////////////////////////////////////////////////////////////////
//系统初始化
/////////////////////////////////////////////////////////////////////
void InitializeSystem()
{
	P0M1 = 0x0; P0M2 = 0x0;
	P1M1 = 0x0;	P1M2 = 0x0;                      
	P3M1 = 0x0; P3M2 = 0xFF; 
	P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;P2 = 0xFF;

 	TMOD=0x21;		   //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xFa;          //波特率为4800bps
	TL1=0xFa;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //允许T0中断
	TR1=1;			   //开启定时器
	TI=1;
	EA=1;			   	//开启总中断
	
	ES = 1;
	RI = 1;

}
  /*定时器初始化*/
void Init_Timer0()
{
	TMOD = 0x11;
	TH0 = (65536 - 500) / 256;
	TL0 = (65536 - 500) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
	PT0 = 1;
}


/*中断程序*/
void timer0(void) interrupt 1
{
	
	switch(order)
	{
		case 1: PWN_OUT = 1;
				TH0 = (65536 - PWN_Value) >> 8;
				TL0 = (uchar)(65536 - PWN_Value);
				break;

		case 2: PWN_OUT = 0;
				TH0 = (65536 - (5000 - PWN_Value)) >> 8;
				TL0 = (uchar)(65536 - (5000 - PWN_Value));
				break;
		case 3:
				TH0 = 60536 >> 8;
				TL0 = (uchar)60536;
				break;
		case 4: 
				TH0 = 60536 >> 8;
				TL0 = (uchar)60536;
				break;
		case 5:
				TH0 = 60536 >> 8;
				TL0 = (uchar)60536;
				order = 0;
				break;
		default:order = 0;
				break;
	}
	order++;
}

void doorOpen()
{
    Init_Timer0();
    PWN_Value = 2000;
}

void servoRet()
{
    Init_Timer0();
    PWN_Value = 500;
}