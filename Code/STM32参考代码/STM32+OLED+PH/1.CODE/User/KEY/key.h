#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 


#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����1
//#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����2
//#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����3
#define WK_UP   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����0(WK_UP) 

 

#define KEY1_PRES 	1	//KEY1����
//#define KEY2_PRES	  2	//KEY2����
//#define KEY3_PRES	  3//KEY2����
#define WKUP_PRES   0//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
