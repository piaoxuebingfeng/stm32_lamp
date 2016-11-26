#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 
#define KEY_B3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//读取按键B3
#define KEY_B12  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键B12
#define KEY_B13  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键B13
#define KEY_B14  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键B14 
#define KEY_C10  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//读取按键C10
 
#define KEYB12_PRES	1		//KEYB12_PRES  
#define KEYB13_PRES	2		//KEYB13_PRES 
#define KEYB14_PRES	3		//KEYB14_PRES  
#define KEYB3_PRES	4		//KEYB3_PRES 

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
