/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * LCD1602:RS -> PA11; RW -> PA12; E -> PA15;
 *         D0~D7 -> PB3~PB10
 * PHģ��: VCC -> 5V; GND -> GND; PO -> PA0;

 * ��������������PHֵ��ʾ�����ڷ���
             ���ڽ��ղ������õ�ֵ��������115200����
 * ����    �����е���
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


char  PH_Buff[6];   //PH���ݴ洢����



char dissbuff[6];



// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[4];

// ���ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[4];  



/***************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************/
 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO  Clock */					 		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//����������ΪJTAG�ڵ�GPIO��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//����PB����IO��JTAG����


}

void PH_Value_Conversion()  //�ɼ���ѹ������ֲ��ṩ������ϵ����PHֵ
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
	OLED_ShowStr(32,0,PH_Buff,2);//����6*8�ַ�

}
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	 
	
		GPIO_Configuration();
    /* ����USART1 */
    USART1_Config();
	    /* ��ʼ��ϵͳ��ʱ�� */
    SysTick_Init();
		// ADC ��ʼ��
		I2C_Configuration();
	  OLED_Init();
	  ADCx_Init();  
	dissbuff[0] = '#';     //���ڷ����ַ����ı�־λ����������λ������PH �Ƕ� �� �¶� 

 	
		OLED_CLS();//����
	
		OLED_ShowCN(16,0,0);//��ʾ������
		OLED_ShowCN(32,0,1);//��ʾ��ӭ��
		OLED_ShowCN(48,0,2);//��ʾ��ʹ��
		OLED_ShowCN(64,0,3);//��ʾ���á�
		
		
		OLED_ShowCN(0,2,4);//��ʾ��ˮ��
		OLED_ShowCN(16,2,5);//��ʾ���ʡ�  
		OLED_ShowCN(32,2,6);//��ʾ���족
		OLED_ShowCN(48,2,7);//��ʾ���⡱  
		OLED_ShowCN(64,2,8);//��ʾ��ϵ��
		OLED_ShowCN(80,2,9);//��ʾ��ͳ��  
		
		delay_ms(2000);  //��ʱ2S
		
    OLED_CLS();//����
		
		OLED_ShowStr(0,0,"PH:",2);

	
  while(1)
	{	
		int i=0;
		

		ADC_ConvertedValueLocal[i]=(float)ADC_ConvertedValue[0]/4096*3.3;


		PH_Value_Conversion();


    Display_Data();		

		delay_ms(200);
		printf("%s",dissbuff);	//���ô��ڷ����ַ���
	
		
	}	
}
/*********************************************END OF FILE**********************/
