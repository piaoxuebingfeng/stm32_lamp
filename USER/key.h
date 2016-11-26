#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 
#define KEY_B3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//��ȡ����B3
#define KEY_B12  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ȡ����B12
#define KEY_B13  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����B13
#define KEY_B14  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����B14 
#define KEY_C10  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//��ȡ����C10
 
#define KEYB12_PRES	1		//KEYB12_PRES  
#define KEYB13_PRES	2		//KEYB13_PRES 
#define KEYB14_PRES	3		//KEYB14_PRES  
#define KEYB3_PRES	4		//KEYB3_PRES 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
