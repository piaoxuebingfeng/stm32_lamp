#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "RGB_LED.h"
#include "key.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "I2C.h"
#include "SI7021.h"
#include <stdio.h>
#include <string.h>
#include "esp8266.h"


char send_buf[MAX_SEND_BUF_LEN];


/**
  * @brief  ESP8266初始化
**/
void ESP8266_Init(void)
{
		printf("%s\r\n","[ESP8266_Init]ENTER AT.");
		SendCmd(AT,"OK",10); 			
		printf("%s\r\n","[ESP8266_Init]EXIT AT.");
		
		printf("%s\r\n","[ESP8266_Init]ENTER CWMODE.");
		SendCmd(CWMODE,"OK",10);		
		printf("%s\r\n","[ESP8266_Init]EXIT CWMODE.");
	
		printf("%s\r\n","[ESP8266_Init]ENTER RST.");
		SendCmd(RST,"OK",40);	
		printf("%s\r\n","[ESP8266_Init]EXIT RST.");
	
		printf("%s\r\n","[ESP8266_Init]ENTER CIFSR.");
		SendCmd(CIFSR,"OK",20);	
		printf("%s\r\n","[ESP8266_Init]EXIT CIFSR.");
	
		printf("%s\r\n","[ESP8266_Init]ENTER CWJAP.");
		SendCmd(CWJAP,"OK",40);	
		printf("%s\r\n","[ESP8266_Init]EXIT CWJAP.");
	
		printf("%s\r\n","[ESP8266_Init]ENTER CIPSTART.");
		SendCmd(CIPSTART,"OK",20);
		printf("%s\r\n","[ESP8266_Init]EXIT CIPSART.");
		
		printf("%s\r\n","[ESP8266_Init]ENTER CIPMODE.");
		SendCmd(CIPMODE,"OK",10);
		printf("%s\r\n","[ESP8266_Init]EXIT CIPMODE.");
}

/**
  * @brief  发送一条AT指令
**/
void SendCmd(char* cmd, char* result, int timeOut)
{
		u32 count=0;
	
    while(1)
    {
        memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
				USART2_RX_STA=0;
				USART_ClearFlag(USART2, USART_FLAG_TC);
				USART_printf(USART2,"%s",(uint8_t *)cmd);	
        delay_ms(50);
				for(count=0;count<timeOut;count++)
				{
						delay_ms(50);
						if((NULL != strstr((const char *)USART2_RX_BUF, (const char *)result)))
						{
								break;
						}
				}		
        if(count<timeOut)
				{
						break;
				}
    }
}

///**
//  * @brief  和平台建立设备连接
//**/
//void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
//{
//		u32 count=0;
//	
//		memset(USART2_RX_BUF,0,strlen((const char *)USART2_RX_BUF));			
//		
//		printf("%s\r\n","[ESP8266_DevLink]ENTER device link...");
//		usart2_write(USART2,CIPSEND,strlen(CIPSEND));  //向ESP8266发送数据透传指令
//		for(count=0;count<timeOut;count++)
//		{
//				delay_ms(50);
//				if((NULL != strstr((const char *)USART2_RX_BUF,">")))
//				{
//						break;
//				}
//		}	

//		send_pkg = PacketConnect1(devid,auth_key);
//		delay_ms(200);
//		usart2_write(USART2,send_pkg->_data,send_pkg->_write_pos);  //发送设备连接请求数据
//		delay_ms(500);
//		DeleteBuffer(&send_pkg);
//		delay_ms(200);
//		usart2_write(USART2,"+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
//		delay_ms(50);
//		printf("%s\r\n","[ESP8266_DevLink]EXIT device link...");
//}

/**
  * @brief  检测ESP8266连接状态
**/
int ESP8266_CheckStatus(int timeOut)
{
		u32 res=0;
		u32 count=0;
	
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
		USART2_RX_STA=0;
		printf("%s\r\n","[ESP8266_CheckStatus]ENTER check status...");
		USART_ClearFlag(USART2, USART_FLAG_TC);
	  USART_printf(USART2,CIPSTATUS);
		for(count=0;count<timeOut;count++)
		{
				delay_ms(100);
				if((NULL != strstr((const char *)USART2_RX_BUF,"STATUS:4")))  //失去连接
				{
						res=-4;
						break;
				}
				else if((NULL != strstr((const char *)USART2_RX_BUF,"STATUS:3")))  //建立连接
				{
						res=0;	
						break;
				}
				else if((NULL != strstr((const char *)USART2_RX_BUF,"STATUS:2")))  //获得IP
				{
						res=-2;
						break;				
				}
				else if((NULL != strstr((const char *)USART2_RX_BUF,"STATUS:5")))  //物理掉线
				{
						res=-5;
						break;
				}
				else if((NULL != strstr((const char *)USART2_RX_BUF,"ERROR")))   
				{
						res=-1;
						break;
				}
				else
				{
						;
				}
		}	
		printf("%s\r\n","[ESP8266_CheckStatus]EXIT check status...");
		return res;	
}

///**
//  * @brief  向平台上传LED当前状态数据
//**/
//void ESP8266_SendDat(void)
//{		
//		u32 count=0;

//		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
//	
//		printf("%s\r\n","[ESP8266_SendDat]ENTER Senddata...");
//		usart2_write(USART2,CIPSEND,strlen(CIPSEND));  //向ESP8266发送数据透传指令
//		for(count=0;count<40;count++)
//		{
//				delay_ms(100);
//				if((NULL != strstr((const char *)USART2_RX_BUF,">")))
//				{
//						break;
//				}
//		}	
//	
//		GetSendBuf();		
//		send_pkg = PacketSavedataSimpleString(DEVICEID,send_buf);   
//		usart2_write(USART2,send_pkg->_data,send_pkg->_write_pos);	//向平台上传数据点
//		DeleteBuffer(&send_pkg);
//		delay_ms(500);

//		usart2_write(USART2,"+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
//		delay_ms(200);
//		printf("%s\r\n","[ESP8266_SendDat]EXIT Senddata...");
//}









