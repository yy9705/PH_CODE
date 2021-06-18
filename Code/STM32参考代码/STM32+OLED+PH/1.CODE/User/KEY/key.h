#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 


#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键1
//#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键2
//#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键3
#define WK_UP   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键0(WK_UP) 

 

#define KEY1_PRES 	1	//KEY1按下
//#define KEY2_PRES	  2	//KEY2按下
//#define KEY3_PRES	  3//KEY2按下
#define WKUP_PRES   0//KEY_UP按下(即WK_UP/KEY_UP)


void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
