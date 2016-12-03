#include "led.h"		    
//WS2812B RGB-LED IO��ʼ��
void RBG_LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //RGB_LED-->PD.2 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 �����
}

//STA LED IO��ʼ��
void STA_LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //STA_LED-->PA.8 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_ResetBits(GPIOA,GPIO_Pin_8); 						 //PA.8 �����
	
}
