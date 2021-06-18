/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * LCD1602:RS -> PA11; RW -> PA12; E -> PA15;
 *         D0~D7 -> PB3~PB10
 * PH模块: VCC -> 5V; GND -> GND; PO -> PA0;

 * 功能描述：测量PH值显示并串口发送
             串口接收测量所得的值（波特率115200）；
 * 作者    ：智男电子
**********************************************************************************/
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include <string.h>
#include "delay.h"
#include "bsp_SysTick.h"
#include "math.h"
#include "bsp_adc.h"
#include "key.h"
#include "OLED_I2C.h"
 
GPIO_InitTypeDef  GPIO_InitStructure; 
unsigned char AD_CHANNEL=0;
unsigned long PH_num=0,PU_V=0;
float PH_Value=0;
u8 ph_temp=0;
u16 ph_result=0,tu_result=0;


char  PH_Buff[6];   //PH数据存储数组



char dissbuff[6];



// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[4];

// 用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[4];  



/***************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO  Clock */					 		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//屏蔽所有作为JTAG口的GPIO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//屏蔽PB口上IO口JTAG功能


}

void PH_Value_Conversion()  //采集电压后根据手册提供函数关系计算PH值
{
		PH_Value=-5.7541*ADC_ConvertedValueLocal[0]+16.654;
	   
		if(PH_Value<=0.0){PH_Value=0.0;}
		if(PH_Value>=14.0){PH_Value=14.0;}
		
	PH_Buff[0]=(int)(PH_Value*100)/1000+'0';	
	PH_Buff[1]=(int)(PH_Value*100)%1000/100+'0';
	PH_Buff[2]='.';
	PH_Buff[3]=(int)(PH_Value*100)%100/10+'0';
	PH_Buff[4]=(int)(PH_Value*100)%10+'0';	
		dissbuff[1] = PH_Buff[0];
		dissbuff[2] = PH_Buff[1];
		dissbuff[3] = PH_Buff[2];
		dissbuff[4] = PH_Buff[3];
		dissbuff[5] = PH_Buff[4];	
	
}


void Display_Data()
{
	OLED_ShowStr(32,0,PH_Buff,2);//测试6*8字符

}
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	 
	
		GPIO_Configuration();
    /* 配置USART1 */
    USART1_Config();
	    /* 初始化系统定时器 */
    SysTick_Init();
		// ADC 初始化
		I2C_Configuration();
	  OLED_Init();
	  ADCx_Init();  
	dissbuff[0] = '#';     //串口发送字符串的标志位，用于让上位机区分PH 浊度 和 温度 

 	
		OLED_CLS();//清屏
	
		OLED_ShowCN(16,0,0);//显示“欢”
		OLED_ShowCN(32,0,1);//显示“迎”
		OLED_ShowCN(48,0,2);//显示“使”
		OLED_ShowCN(64,0,3);//显示“用”
		
		
		OLED_ShowCN(0,2,4);//显示“水”
		OLED_ShowCN(16,2,5);//显示“质”  
		OLED_ShowCN(32,2,6);//显示“检”
		OLED_ShowCN(48,2,7);//显示“测”  
		OLED_ShowCN(64,2,8);//显示“系”
		OLED_ShowCN(80,2,9);//显示“统”  
		
		delay_ms(2000);  //延时2S
		
    OLED_CLS();//清屏
		
		OLED_ShowStr(0,0,"PH:",2);

	
  while(1)
	{	
		int i=0;
		

		ADC_ConvertedValueLocal[i]=(float)ADC_ConvertedValue[0]/4096*3.3;


		PH_Value_Conversion();


    Display_Data();		

		delay_ms(200);
		printf("%s",dissbuff);	//调用串口发送字符串
	
		
	}	
}
/*********************************************END OF FILE**********************/
