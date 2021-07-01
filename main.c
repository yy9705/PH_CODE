/******************************************
	作者：神秘藏宝室
	店铺：ILoveMCU.taobao.com
******************************************/


#include	"config.h"
#include	"delay.h"
#include  "uart.h"

/*************	功能说明	**************

本程序演示LED闪烁  


******************************************/

/*************	本地常量声明	**************/
sbit DQ=P1^0; 				//数据传输线接单片机的相应的引脚 


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/



/******************** IO配置函数 **************************/
void	GPIO_config(void)
{
	P1M0 = 0x00; 		//设置准双向口
	P1M1 = 0x00;

	P3M0 = 0x00;
	P3M1 = 0x00;
}

void Init_DS18B20(void) 
{
//	bit x;
	DQ=1; 					//DQ先置高 
	delay_us(12);
	DQ=0; 					//发送复位脉冲 
	delay_us(600);				//延时（>480us)
	//delay_ms(1);
	DQ=1;
	delay_us(15);
//	x=DQ;
//	printf("x=%d\r\n", x);
	delay_us(200);
}

//读一个字节
unsigned char ReadOneChar(void)  			//主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
	unsigned char i=0; 		//每个读周期最短的持续时间为60us，各个读周期之间必须有1us以上的高电平恢复期
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//一个字节有8位 
	{
		DQ=0;
		delay_us(5);
		dat>>=1; 
		DQ=1;
		delay_us(5);	
		if(DQ) 
		dat|=0x80; 
		delay_us(50);
	} 
	return(dat);
}

//写一个字节
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i; 		//数据线从高电平拉至低电平，产生写起始信号。15us之内将所需写的位送到数据线上，
	unsigned char temp;
	for(i=0;i<8;i++) 		//在15~60us之间对数据线进行采样，如果是高电平就写1，低写0发生。 
	{
		temp=dat>>i;
		temp &= 0x01;
		DQ=0; 				//在开始另一个写周期前必须有1us以上的高电平恢复期。 
//		DQ=dat&0x01; 
		delay_us(5);
		if(temp == 1)
		DQ=1;
		delay_us(55);
//		dat>>=1;
		DQ=1;
		delay_us(5);
	} 
}

//读温度值（低位放tempL;高位放tempH;）
float ReadTemperature(void) 
{ 
	short s_tem;
	float result;
	unsigned char temp[2];		//用于存放原始读数
	
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作
	WriteOneChar(0x44); 				//启动温度转换
	delay_ms(200); 						//转换需要一点时间，延时 
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作 
	WriteOneChar(0xbe); 				//读温度寄存器（头两个值分别为温度的低位和高位） 
	temp[0]=ReadOneChar(); 				//读出温度的低位LSB
	temp[1]=ReadOneChar(); 				//读出温度的高位MSB	
	s_tem=temp[1]<<8;
	s_tem=s_tem | temp[0];
	s_tem=s_tem&4095;
	if((s_tem&2048) == 2048)
	{
		s_tem=(s_tem^4095)&4095;
		result=-1*(s_tem+1)*0.0625;
	}
//	xiaoshu1 = (temp[0]&0x0f)*10/16; 		//小数第一位
//	xiaoshu2 = (temp[1]&0x0f)*100/16%10;	//小数第二位
//	xiaoshu=xiaoshu1*10+xiaoshu2; 		//小数两位
	else
	result=s_tem*0.0625;
	
	return	result; 
}




/******************** 主函数 **************************/
int main(void)
{
	float temp_result;
	P_SW1 = 0x00; //RXD/P3.0, TXD/P3.1

	GPIO_config();
	Uart_Init();
	
	while(1)
	{
		temp_result=ReadTemperature();
		//delay_ms(200);
		printf("Temperuture Get is:%f\n", temp_result);
	}
}
//
//
//
//
//
//#include "reg51.h"
//#include "intrins.h"
//sfr P0M1 = 0x93;
//sfr P0M0 = 0x94;
//sfr P1M1 = 0x91;
//sfr P1M0 = 0x92;
//sfr P2M1 = 0x95;
//sfr P2M0 = 0x96;
//sfr P3M1 = 0xb1;
//sfr P3M0 = 0xb2;
//sfr P4M1 = 0xb3;
//sfr P4M0 = 0xb4;
//sfr P5M1 = 0xc9;
//sfr P5M0 = 0xca;
//sfr IAP_DATA = 0xC2;
//sfr IAP_ADDRH = 0xC3;
//sfr IAP_ADDRL = 0xC4;
//sfr IAP_CMD = 0xC5;
//sfr IAP_TRIG = 0xC6;
//sfr IAP_CONTR = 0xC7;
//sfr IAP_TPS = 0xF5;
//void IapIdle()
//{
//IAP_CONTR = 0; //关闭 IAP 功能
//IAP_CMD = 0; //清除命令寄存器
//IAP_TRIG = 0; //清除触发寄存器
//IAP_ADDRH = 0x80; //将地址设置到非 IAP 区域
//IAP_ADDRL = 0;
//}
//char IapRead(int addr)
//{
//char dat;
//IAP_CONTR = 0x80; //使能 IAP
//IAP_TPS = 12; //设置擦除等待参数 12MHz
//IAP_CMD = 1; //设置 IAP 读命令
//IAP_ADDRL = addr; //设置 IAP 低地址
//IAP_ADDRH = addr >> 8; //设置 IAP 高地址
//IAP_TRIG = 0x5a; //写触发命令(0x5a)
//IAP_TRIG = 0xa5; //写触发命令(0xa5)
//_nop_();
//dat = IAP_DATA; //读 IAP 数据
//IapIdle(); //关闭 IAP 功能
//
//return dat;
//}
//void IapProgram(int addr, char dat)
//{
//IAP_CONTR = 0x80; //使能 IAP
//IAP_TPS = 12; //设置擦除等待参数 12MHz
//IAP_CMD = 2; //设置 IAP 写命令
//IAP_ADDRL = addr; //设置 IAP 低地址
//IAP_ADDRH = addr >> 8; //设置 IAP 高地址
//IAP_DATA = dat; //写 IAP 数据
//IAP_TRIG = 0x5a; //写触发命令(0x5a)
//IAP_TRIG = 0xa5; //写触发命令(0xa5)
//_nop_();
//IapIdle(); //关闭 IAP 功能
//}
//void IapErase(int addr)
//{
//	IAP_CONTR = 0x80; //使能 IAP
//	IAP_TPS = 12; //设置擦除等待参数 12MHz
//	IAP_CMD = 3; //设置 IAP 擦除命令
//	IAP_ADDRL = addr; //设置 IAP 低地址
//	IAP_ADDRH = addr >> 8; //设置 IAP 高地址
//	IAP_TRIG = 0x5a; //写触发命令(0x5a)
//	IAP_TRIG = 0xa5; //写触发命令(0xa5)
//	_nop_(); //
//	IapIdle(); //关闭 IAP 功能
//}
//void main()
//{
//	P0M0 = 0x00;
//	P0M1 = 0x00;
//	P1M0 = 0x00;
//	P1M1 = 0x00;
//	P2M0 = 0x00;
//	P2M1 = 0x00;
//	P3M0 = 0x00;
//	P3M1 = 0x00;
//	P4M0 = 0x00;
//	P4M1 = 0x00;
//	P5M0 = 0x00;
//	P5M1 = 0x00;
//	IapErase(0x0400);
//	P0 = IapRead(0x0400); //P0=0xff
//	IapProgram(0x0400, 0x12);
//	P1 = IapRead(0x0400); //P1=0x12
//	while (1);
//}
