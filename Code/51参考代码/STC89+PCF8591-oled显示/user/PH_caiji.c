/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * PCF8591:VCC -> 5V; GND -> GND; SCL -> P1.6;SDA -> P1.7; 
 *         OUT1~OUT8 -> P3口
 * OLED :CS -> P2.4; RST -> P2.2; DC -> P2.3;SCL -> P2.0;SDIN -> P2.1;
 *         D0~D7 -> P0口
 * PH模块: VCC -> 5V; GND -> GND; PO -> AIN0;
 *
 * 功能描述：测量PH值液晶显示；
             串口接收测量所得的PH值（波特率9600）；
 *           可连接上位机显示PH值；
 * 作者    ：智男电子
**********************************************************************************/

#include <reg52.h>
#include <include.h>

typedef unsigned char uchar;
typedef unsigned char uint;

/**************外部变量定义************************/
int num=0; 
/**************变量定义***************************/
double PH=0.0;
unsigned char disbuff[5]={0};
double ADC_Voltage=0.0;
/*
 * 函数名：delay_1s
 * 描述  ：延时函数1s
 *         
 * 输入  ：unsigned int time：延时量
 * 输出  ：无
 * 说明  ：液晶显示用延时函数
 */
void delay_1s()	  //延时1s
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}	

/*
 * 函数名：Get_Voltage_Value
 * 描述  ：获取PCF8591的转换电压值
 */
void Get_Voltage_Value()
{
			ISendByte(PCF8591,0x40);
      num=IRcvByte(PCF8591);  //ADC0 模数转换1      PH电极
		  ADC_Voltage =num * 5.0/ 255;  //转换为电压值
}
/*
 * 函数名：Character_Conversion
 * 描述  ：延时函数
 *         
 * 输入  ：unsigned char n：延时量
 * 输出  ：无
 * 说明  ：
 */
void PH_Value_Conversion()
{	
	PH=-5.887*ADC_Voltage+21.677;
	
	if(PH<=0){PH=0;}
	if(PH>=14){PH=14;}
		
   /*显示酸碱度*/
	disbuff[0]=(int)(PH*100)/1000+'0';
	disbuff[1]=(int)(PH*100)%1000/100+'0';	
	disbuff[2]='.';
	disbuff[3]=(int)(PH*100)%100/10+'0';
	disbuff[4]=(int)(PH*100)%10+'0';
	
}
void display_value()
{
	
  /*显示 PH 浓度值*/
	OLED_ShowChar(24,2,disbuff[0]);//显示ASCII字符
	OLED_ShowChar(33,2,disbuff[1]);//显示ASCII字符
	OLED_ShowChar(42,2,disbuff[2]);//显示ASCII字符
	OLED_ShowChar(51,2,disbuff[3]);//显示ASCII字符	
	OLED_ShowChar(60,2,disbuff[4]);//显示ASCII字符	
}
void StartUART( void )
{  							//波特率9600
     SCON = 0x50;
     TMOD = 0x20;
     TH1 = 0xFD;
     TL1 = 0xFD; 
     PCON = 0x00;
     TR1 = 1;
}
//**********通过串口将接收到数据发送给PC端**************
void R_S_Byte(uchar R_Byte)
{	
	 SBUF = R_Byte;  
     while( TI == 0 );				//查询法
  	 TI = 0;    
}
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
void main(void)
{
	int i;
	StartUART();
	OLED_Init();			//初始化OLED  
	OLED_Clear()  	; 

	
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(18,0,1);
	OLED_ShowCHinese(36,0,2);
	OLED_ShowCHinese(54,0,3);
	OLED_ShowCHinese(72,0,4);
	OLED_ShowCHinese(90,0,5);
	OLED_ShowCHinese(108,0,6);
	OLED_ShowString(0,2,"PH:");	
	
	while(1)
	{		
				Get_Voltage_Value();
				PH_Value_Conversion();
				display_value(); 
			for(i=0;i<5;i++)
				{
					R_S_Byte(disbuff[i]);
				}	
//				R_S_Byte('\r');//发送回车换行符
//				R_S_Byte('\n');
				delay_1s();
  }	
}
