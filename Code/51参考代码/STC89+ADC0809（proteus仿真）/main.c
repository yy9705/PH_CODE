/******************** (C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * ADC0809:OE -> P1.0; EOC -> P1.1; ST -> P1.2;CLK -> P1.3; A -> P1.4; B -> P1.5; C->P1.6;
 *         OUT1~OUT8 -> P3口
 * LCD1602:RS -> P0.0; RW -> P0.1; E -> P0.2;
 *         D0~D7 -> P2口
 * PH模块: VCC -> 5V; GND -> GND; PO -> IN3;
 *
 * 实验平台：
 *
 * 作者    ：智男电子
**********************************************************************************/
#include <reg52.h>
#include <adc0809.h>
#include <1602.h>
#include <intrins.h>
#include <math.h>
unsigned char disbuff[5]={0};
uint num;
float PH_V ,PH;
/**************延时1s函数***************/
void Delay_1s()
{
     unsigned char a,b,c;
    for(c=167;c>0;c--)
        for(b=171;b>0;b--)
            for(a=16;a>0;a--);
}

/**************PH值采集函数***************/
void PH_Collection()
{
	Red_0809 () ;
	PH_V =num * 5.0 / 256;
	PH=-5.887*PH_V+21.677;
	if(PH<=0){PH=0;}
	if(PH>=14){PH=14;}
}
/**************PH值显示函数***************/
void PH_Display()
{
   
	disbuff[0]=(int)(PH*100)/1000+'0';
	disbuff[1]=(int)(PH*100)%1000/100+'0';	
	disbuff[2]='.';
	disbuff[3]=(int)(PH*100)%100/10+'0';
	disbuff[4]=(int)(PH*100)%10+'0';

	DisplaySingleChar(0x0,1,'P');
	DisplaySingleChar(0x1,1,'H');
	DisplaySingleChar(0x2,1,'=');
	DisplaySingleChar(0x3,1,disbuff[0]);
	DisplaySingleChar(0x4,1,disbuff[1]);
	DisplaySingleChar(0x5,1,disbuff[2]);
	DisplaySingleChar(0x6,1,disbuff[3]);
	DisplaySingleChar(0x7,1,disbuff[4]);
} 

/**************主函数***************/
void main()
{ 	
	TMOD = 0x02; //中断初始化，adc0809产生时钟信号
	TH0  = 0x14;
	TL0  = 0x00;
	IE   = 0x82;
	TR0  = 1;
	P1   = 0x3f;
	LCDInit();	 //LCD初始化
  DisplayString(0x0,0,"  Welcome  PH   ");
	DisplayString(0x0,1,"Detection system");
	Delay_1s();
	WriteCmd(LCD_CLS);		 //清屏
	
	while(1)
	{
      PH_Collection();
		  PH_Display();
  }
}

void Timer0_INT() interrupt 1
{
 	CLK = !CLK;
}
