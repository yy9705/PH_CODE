/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * LCD1602:RS -> P2.5; RW -> P2.6; E -> P2.7;
 *         D0~D7 -> P0口
 * PH模块: VCC -> 5V; GND -> GND; PO -> P1.1;
 * 按键:   key1 -> P2.0; key2 -> P2.1; key3 -> P2.2;
 * LED：   LED -> P1.4
 * 功能描述：测量PH值液晶显示；
             按键设定PH值上下限值，超过设定范围指示灯亮进行报警
             串口接收测量所得的PH值（波特率9600）；
 *           可连接上位机显示PH值；
 * 作者    ：智男电子
**********************************************************************************/
#include <STC12C5A60S2.h>
#include <intrins.h>
#include <lcd.h>


#define	uchar	unsigned char
#define uint	unsigned int

#define MAIN_Fosc		11059200L	//定义主时钟, 模拟串口和和延时会自动适应。5~35MHZ  22118400L 
 
#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0
#define ADC_FLAG	(1 << 4)	//软件清0
#define ADC_START	(1 << 3)	//自动清0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7

sbit k1 = P2^0;
sbit k2 = P2^1;
sbit k3 = P2^2;
sbit ph_led = P1^4;


/**************变量定义***************************/
float PH=0.0;
float PH_value=0.0;
float ph_set1=0.0;
float ph_set2=0.0;
unsigned char disbuff[5]={0}; 


void  DelayUs(unsigned int time);   //延时子函数
void  Delay1(unsigned int time);		 //延时主程序
void  press_k1();
void  press_k2();
void  press_k3();
uint	adc10_start(uchar channel);	//channel = 0~7
int s1;


/*
 * 函数名：PH_Value_Conversion
 * 描述  ：获取PH模块输出电压值
 *         
 * 输入  ：无
 * 输出  ：无
 * 说明  ：
 */
void PH_Value_Conversion()
{
		PH=(PH*5)/1024;
	  PH_value=-5.887*PH+21.677;
	
	  if((PH_value<=0)){PH_value=0;}
		if((PH_value>14)){PH_value=14.1;}
		
		/*显示酸碱度*/
		disbuff[0]=(int)(PH_value*100)/1000+'0';
		disbuff[1]=(int)(PH_value*100)%1000/100+'0';	
		disbuff[2]='.';
		disbuff[3]=(int)(PH_value*100)%100/10+'0';
		disbuff[4]=(int)(PH_value*100)%10+'0';
		
		DisplaySingleChar(0x03,0,disbuff[0]);
		DisplaySingleChar(0x04,0,disbuff[1]);
		DisplaySingleChar(0x05,0,disbuff[2]);
		DisplaySingleChar(0x06,0,disbuff[3]);
		DisplaySingleChar(0x07,0,disbuff[4]);		
}

/*
 */

void Display_PH1()
{
	uint ph_h1, ph_h2,ph_h3,ph_h4;
	ph_h1 = (int)(ph_set1 * 100)/1000;
	ph_h2 = (int)(ph_set1 * 100)%1000/100;
	ph_h3 = (int)(ph_set1 * 100)%100/10;
	ph_h4 = (int)(ph_set1 * 100)%10;
	DisplaySingleChar(0x00,1,ph_h1+0x30);
  DisplaySingleChar(0x01,1,ph_h2+0x30);
	DisplaySingleChar(0x02,1,'.');
	DisplaySingleChar(0x03,1,ph_h3+0x30);
	DisplaySingleChar(0x04,1,ph_h4+0x30);	
}
void Display_PH2()
{
	uint ph_h1, ph_h2,ph_h3,ph_h4;
	ph_h1 = (int)(ph_set2 * 100)/1000;
	ph_h2 = (int)(ph_set2 * 100)%1000/100;
	ph_h3 = (int)(ph_set2 * 100)%100/10;
	ph_h4 = (int)(ph_set2 * 100)%10;
	DisplaySingleChar(0x07,1,ph_h1+0x30);
  DisplaySingleChar(0x08,1,ph_h2+0x30);
	DisplaySingleChar(0x09,1,'.');
	DisplaySingleChar(0x0a,1,ph_h3+0x30);
	DisplaySingleChar(0x0b,1,ph_h4+0x30);	
}
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
void main(void)
{
	 
	LCDInit();                              //显示屏初始化
	DisplayString(0x0,0,"  Welcome...  ");
	DisplayString(0x0,1,"water quality");
	Delay1(500);
	WriteCmd(LCD_CLS);
	P1ASF = 0x07;			//12C5A60AD/S2系列模拟输入(AD)选择
	ADC_CONTR = ADC_360T | ADC_ON;
	ph_set1 = 4.0;
	ph_set2 = 9.0;	
	DisplayString(0x0,0,"PH:");
	while(1)
	{

		press_k1();
		press_k2();
		press_k3();
		Display_PH1();
		Display_PH2();
   //显示设置位置指示符
		if (s1 == 0)
		{
			DisplayString(0x05,1,"*");
			DisplayString(0x0c,1," ");
		}
		if (s1 == 1)
		{
			DisplayString(0x05,1," ");
			DisplayString(0x0c,1,"*");
		}
		//PH采集转换
		PH = adc10_start(1);
    Delay1(1000);		
		PH_Value_Conversion();
		//PH上下限判定
		if(PH_value>ph_set1 && PH_value<ph_set2)
		{
			ph_led = 1; //指示灯灭
		}
		else
		{
			ph_led = 0;//指示灯亮
		}
						
	}
}
	

///********************* 做一次ADC转换 *******************/
uint	adc10_start(uchar channel)	//channel = 0~7
{
	uint	adc;
	uchar	i;

	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 

	i = 250;
	do{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
			adc = (uint)ADC_RES;
			adc = (adc << 2) | (ADC_RESL & 3);
			return	adc;
		}
	}while(--i);
	return	1024;
}


//========================================================================
// 描述: 键盘扫描。
void press_k1()		//设置按键扫描子函数	 
{
	if (k1 == 0)
	{
		Delay1(100);
		if (k1 == 0)
		{
			s1++;
			if (s1 == 2)
			{
				s1 = 0 ;
			}
		}
	}
}

void press_k2()		//+按键扫描子函数
{
 if (k2 == 0)
	{
	 Delay1(100);
	 if (k2 == 0)
	  {
	   if (s1 == 0)
	    {
				ph_set1+=0.01;
				if(ph_set1 > 14.1)
				{
					ph_set1 = 4;
				}					
			}
	 	 if (s1 == 1)
			{
				ph_set2+=0.01;
				if(ph_set2 > 14.1)
				{
					ph_set2 = 9;
				}		
			}
		}
	}
}

void press_k3()		//-按键扫描子函数
{
 if (k3 == 0)
	{
	 Delay1(100);
	 if (k3 == 0)
	  {
	   if (s1 == 0)
	    {
				ph_set1-=0.01;
				if(ph_set1 <= 0)
				{
					ph_set1 = 4;
				}				
			}
	 	 if (s1 == 1)
			{
				ph_set2-=0.01;
				if(ph_set2 <= 0)
				{
					ph_set2 = 9;
				}
			}
		}
	}
}
//延时函数//
void Delay1(unsigned int time)
{
    unsigned int timeCounter = 0;
	for (timeCounter = time;timeCounter > 0 ;timeCounter --)
		DelayUs(255);
}

void DelayUs(unsigned int time)
{
	unsigned int timeCounter = 0;
	for (timeCounter = 0;timeCounter < time;timeCounter ++)
		_nop_();
}



