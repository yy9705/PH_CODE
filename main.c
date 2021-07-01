/******************************************
	���ߣ����زر���
	���̣�ILoveMCU.taobao.com
******************************************/


#include	"config.h"
#include	"delay.h"
#include  "uart.h"

/*************	����˵��	**************

��������ʾLED��˸  


******************************************/

/*************	���س�������	**************/
sbit DQ=P1^0; 				//���ݴ����߽ӵ�Ƭ������Ӧ������ 


/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/******************** IO���ú��� **************************/
void	GPIO_config(void)
{
	P1M0 = 0x00; 		//����׼˫���
	P1M1 = 0x00;

	P3M0 = 0x00;
	P3M1 = 0x00;
}

void Init_DS18B20(void) 
{
//	bit x;
	DQ=1; 					//DQ���ø� 
	delay_us(12);
	DQ=0; 					//���͸�λ���� 
	delay_us(600);				//��ʱ��>480us)
	//delay_ms(1);
	DQ=1;
	delay_us(15);
//	x=DQ;
//	printf("x=%d\r\n", x);
	delay_us(200);
}

//��һ���ֽ�
unsigned char ReadOneChar(void)  			//�����������ȴӸ������͵�ƽ1us���ϣ���ʹ��������Ϊ�ߵ�ƽ���Ӷ��������ź�
{
	unsigned char i=0; 		//ÿ����������̵ĳ���ʱ��Ϊ60us������������֮�������1us���ϵĸߵ�ƽ�ָ���
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//һ���ֽ���8λ 
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

//дһ���ֽ�
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i; 		//�����ߴӸߵ�ƽ�����͵�ƽ������д��ʼ�źš�15us֮�ڽ�����д��λ�͵��������ϣ�
	unsigned char temp;
	for(i=0;i<8;i++) 		//��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
	{
		temp=dat>>i;
		temp &= 0x01;
		DQ=0; 				//�ڿ�ʼ��һ��д����ǰ������1us���ϵĸߵ�ƽ�ָ��ڡ� 
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

//���¶�ֵ����λ��tempL;��λ��tempH;��
float ReadTemperature(void) 
{ 
	short s_tem;
	float result;
	unsigned char temp[2];		//���ڴ��ԭʼ����
	
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ���
	WriteOneChar(0x44); 				//�����¶�ת��
	delay_ms(200); 						//ת����Ҫһ��ʱ�䣬��ʱ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ��� 
	WriteOneChar(0xbe); 				//���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ�� 
	temp[0]=ReadOneChar(); 				//�����¶ȵĵ�λLSB
	temp[1]=ReadOneChar(); 				//�����¶ȵĸ�λMSB	
	s_tem=temp[1]<<8;
	s_tem=s_tem | temp[0];
	s_tem=s_tem&4095;
	if((s_tem&2048) == 2048)
	{
		s_tem=(s_tem^4095)&4095;
		result=-1*(s_tem+1)*0.0625;
	}
//	xiaoshu1 = (temp[0]&0x0f)*10/16; 		//С����һλ
//	xiaoshu2 = (temp[1]&0x0f)*100/16%10;	//С���ڶ�λ
//	xiaoshu=xiaoshu1*10+xiaoshu2; 		//С����λ
	else
	result=s_tem*0.0625;
	
	return	result; 
}




/******************** ������ **************************/
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
//IAP_CONTR = 0; //�ر� IAP ����
//IAP_CMD = 0; //�������Ĵ���
//IAP_TRIG = 0; //��������Ĵ���
//IAP_ADDRH = 0x80; //����ַ���õ��� IAP ����
//IAP_ADDRL = 0;
//}
//char IapRead(int addr)
//{
//char dat;
//IAP_CONTR = 0x80; //ʹ�� IAP
//IAP_TPS = 12; //���ò����ȴ����� 12MHz
//IAP_CMD = 1; //���� IAP ������
//IAP_ADDRL = addr; //���� IAP �͵�ַ
//IAP_ADDRH = addr >> 8; //���� IAP �ߵ�ַ
//IAP_TRIG = 0x5a; //д��������(0x5a)
//IAP_TRIG = 0xa5; //д��������(0xa5)
//_nop_();
//dat = IAP_DATA; //�� IAP ����
//IapIdle(); //�ر� IAP ����
//
//return dat;
//}
//void IapProgram(int addr, char dat)
//{
//IAP_CONTR = 0x80; //ʹ�� IAP
//IAP_TPS = 12; //���ò����ȴ����� 12MHz
//IAP_CMD = 2; //���� IAP д����
//IAP_ADDRL = addr; //���� IAP �͵�ַ
//IAP_ADDRH = addr >> 8; //���� IAP �ߵ�ַ
//IAP_DATA = dat; //д IAP ����
//IAP_TRIG = 0x5a; //д��������(0x5a)
//IAP_TRIG = 0xa5; //д��������(0xa5)
//_nop_();
//IapIdle(); //�ر� IAP ����
//}
//void IapErase(int addr)
//{
//	IAP_CONTR = 0x80; //ʹ�� IAP
//	IAP_TPS = 12; //���ò����ȴ����� 12MHz
//	IAP_CMD = 3; //���� IAP ��������
//	IAP_ADDRL = addr; //���� IAP �͵�ַ
//	IAP_ADDRH = addr >> 8; //���� IAP �ߵ�ַ
//	IAP_TRIG = 0x5a; //д��������(0x5a)
//	IAP_TRIG = 0xa5; //д��������(0xa5)
//	_nop_(); //
//	IapIdle(); //�ر� IAP ����
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
