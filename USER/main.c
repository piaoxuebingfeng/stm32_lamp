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
extern u8 wifi_flag;
static u8 color_i;//修改颜色的值
u8 white_flag;


void WIFI_AP_STA();
void WIFI_AP_STA_yanchanghong();
void ADC_POWER();
void LED_shanshuo();
void RGB_LED_shanshuo();
void huxi_white();
void huxi_white_2();
void huxi_white_3();
void huxi_color();
void delay_white();
void Uart2_process();
void Uart1_process();
u8 smart_config_key();
void adc_light_process();
void print_temp_humi();
static u8 light_test_flag = 1;
int main(void)
 {	
	u8 t,sendi;
	//u16  V_timers=0;     //电量检测时间间隔  10s检测一次
	u16 times=0; 

	u8 i,j ;
	u8 key_value;
	delay_init();	    	 //延时函数初始化
 	Adc_Init();		  		//ADC初始化	 	
	RBG_LED_Init();		  	 //初始化与LED连接的硬件接口
	STA_LED_Init();
	KEY_Init();
	I2C_GPIO_Config();	 
	NVIC_Configuration();// 设置中断优先级分组
  //TIM3_Int_Init(4999,7199);
	TIM4_Int_Init(4999,7199);
	 // 72000000/7200=10000Hz   10000/5000=2Hz  为0.5s     10Khz的计数频率，计数到5000为500ms  
	uart1_init(9600);	 //串口初始化为9600
	uart2_init(9600);
	//double_GRB();
	 LED0=0;
	print_temp_humi();
	send24_GRB(0,0,0);
	delay_ms(1000);
	send24_GRB(0,0,0);
	delay_ms(5);
	
	send_circle();
	
	send_semicircle();
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_printf(USART2,"AT+CIPMUX=1\r\n");
	send_semicircle();
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_printf(USART2,"AT+CIPSERVER=1,8080\r\n");
	//send_semicircle();
	//USART_ClearFlag(USART2, USART_FLAG_TC);
  //USART_printf(USART2,"AT+CWSMARTSTART=1\r\n");
	send24_GRB(5,5,5);
	delay_ms(500);
	sendi_GRB(0,0,0,48);
	light_test_flag=0;

	while(1)
	{
		//ADC_POWER();	//电量检测函数
		Uart2_process();
		Uart1_process();
		key_value=KEY_Scan(0);
		if(key_value==1)
			send24_GRB(0,0,0);
		else if(key_value == 2)
			send24_GRB(0,255,0);
		smart_config_key();
		if(light_test_flag == 1)
			adc_light_process();
		//print_temp_humi();
	}
}
void print_temp_humi()
{
	u16 temp,humi;
	//新增SI7021系列温湿度传感器采集
	if(SI7021_Read( &temp, &humi ))
	{
		printf( "温度：%.1f 湿度：%.1f%%\r\n", temp / 10.0, humi/ 10.0 );
	}
	else
	{
		 printf( "read SI7021 ERROR\r\n");
	}
}
//按键长按
u8 smart_config_key()
{
	u16 i;
	if(KEY_C4==0)
	{
		while(KEY_C4==0)
		{
			i++;
			delay_ms(100);
			if(i>15)
			{
				USART_ClearFlag(USART2, USART_FLAG_TC);
				USART_printf(USART2,"AT+CWSMARTSTART=1\r\n");
				send24_GRB(0,255,255);
			}
		}
	}
}

void adc_light_process()
{
		u16 adcx,light_intensity;
		float adc_f;
		u16 color_light;
		adcx=Get_Adc_Average(ADC_Channel_1,30);
		adc_f=(float)adcx*3.3/4096;
		printf("ADC:%.2f\r\n",adc_f);
		light_intensity=(u16)(adc_f*100);
		if(light_intensity <=200)
				color_light=0;
		else
				color_light=light_intensity-200;	
		send24_GRB(color_light,color_light,color_light);
}
void Uart1_process()
{
	u8 len;
	u8 Red;
	u8 Blue;	 
	u8 Green;
	u8 ID;
	if(USART1_RX_STA&0x8000)
	{
		len=USART1_RX_STA&0x3fff;//得到此次接收到的数据长度
		ID=USART1_RX_BUF[4];
		
	//测试板子时 用的串口通信数据
	if(USART1_RX_BUF[0]=='L')
	{				
		 LED2=0;
			huxi_white_3();//呼吸渐暗
	}
	if(USART1_RX_BUF[0]=='l')
	{				
		 LED2=1;
		huxi_white_3();
	}
	if(USART1_RX_BUF[0]=='D')
	{				
		 LED3=0;
		send24_GRB(255,255,255);
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM3
		TIM_IT_Update ,
		ENABLE  //使能
		);
		//delay_white();
			
	}
	if(USART1_RX_BUF[0]=='d')
	{				
		 LED3=1;
//		TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM3, //TIM3
//		TIM_IT_Update ,
//		DISABLE  //使能
//		);
		//delay_white();
	}

	if((USART1_RX_BUF[0]=='G')&&(USART1_RX_BUF[1]=='R')&&(USART1_RX_BUF[2]=='B'))//GRB
	{

		Green=(USART1_RX_BUF[3]-0x30)*100+(USART1_RX_BUF[4]-0x30)*10+(USART1_RX_BUF[5]-0x30)*1;
		Red=(USART1_RX_BUF[6]-0x30)*100+(USART1_RX_BUF[7]-0x30)*10+(USART1_RX_BUF[8]-0x30)*1;
		Blue=(USART1_RX_BUF[9]-0x30)*100+(USART1_RX_BUF[10]-0x30)*10+(USART1_RX_BUF[11]-0x30)*1;
		send24_GRB(Green,Red,Blue);
		Green=0;
		Red=0;
		Blue=0;
	}
	USART1_RX_STA=0;
	}
} 

void Uart2_process()
{
	u8 len;
	u8 Red;
	u8 Blue;	 
	u8 Green;
	u8 ID;
	if(USART2_RX_STA&0x8000)
	{
		len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
		ID=USART2_RX_BUF[4];
		
	if(USART2_RX_BUF[8]=='L')
	{	
			 light_test_flag=0;
			 delay_ms(10);
		 	 send24_GRB(60,50,45);  //打开灯环
			 delay_ms(10);
			 TIM_Cmd(TIM4, ENABLE);
					TIM_ITConfig(  //使能或者失能指定的TIM中断
					TIM4, //TIM4
					TIM_IT_Update ,
					ENABLE
				);
	}
	if(USART2_RX_BUF[8]=='l')
	{		
			//light_test_flag=1;
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update ,
		DISABLE
		);
		 send24_GRB(0,0,0);
	}
	if(USART2_RX_BUF[8]=='M')//对光线传感器进行控制 打开光线感应开关
	{				
		 	light_test_flag=1;
	}
	if(USART2_RX_BUF[8]=='m')//对光线传感器进行控制 关闭光线感应开关
	{				
		 light_test_flag=0;
	}
	if(USART2_RX_BUF[8]=='D')
	{				
		 LED3=0;
		send24_GRB(255,255,255);
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM3
		TIM_IT_Update ,
		ENABLE  //使能
		);
		//delay_white();
			
	}
	if(USART2_RX_BUF[8]=='d')
	{				
		 LED3=1;
//		TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM3, //TIM3
//		TIM_IT_Update ,
//		DISABLE  //使能
//		);
		//delay_white();
	}

	if((USART2_RX_BUF[9]=='G')&&(USART2_RX_BUF[10]=='R')&&(USART2_RX_BUF[11]=='B'))//GRB
	{
		light_test_flag=0;
		Green=(USART2_RX_BUF[12]-0x30)*100+(USART2_RX_BUF[13]-0x30)*10+(USART2_RX_BUF[14]-0x30)*1;
		Red=(USART2_RX_BUF[15]-0x30)*100+(USART2_RX_BUF[16]-0x30)*10+(USART2_RX_BUF[17]-0x30)*1;
		Blue=(USART2_RX_BUF[18]-0x30)*100+(USART2_RX_BUF[19]-0x30)*10+(USART2_RX_BUF[20]-0x30)*1;
		send24_GRB(Green,Red,Blue);
		Green=0;
		Red=0;
		Blue=0;
	}
	USART2_RX_STA=0;
	memset(USART2_RX_BUF,0,USART_REC_LEN);
	}
}
void RGB_LED_shanshuo()
{
	send24_GRB(255,0,0);
	delay_ms(300);
	send24_GRB(0,255,0);
	delay_ms(300);
	send24_GRB(0,0,255);
	delay_ms(300);
	send24_GRB(255,255,255);
	delay_ms(300);
	send24_GRB(0,0,0);
	delay_ms(300);
	send24_GRB(255,255,255);
	delay_ms(300);
	send24_GRB(0,0,0);
}
void LED_shanshuo()
{
   LED0=0;
	 delay_ms(200);
	 LED0=1;
	 LED2=0;
	 delay_ms(200);
	 LED2=1;
	 LED3=0;
	 delay_ms(200);
	 LED3=1;
	 LED4=0;
	 delay_ms(200);
	 LED4=1;
	 LED3=0;
	 delay_ms(200);
	 LED3=1;
	 LED2=0;
	 delay_ms(200);
	 LED2=1;
	 LED0=0;
	 delay_ms(200);
	 LED0=1;
}
void ADC_POWER()  //30s 检测一次
{
	//电池电量检测 
	u16 adcx;
	float temp;
	adcx=Get_Adc_Average(ADC_Channel_4,10);
	temp=(float)adcx*(3.3/4096);
	adcx=temp;
	printf(" %f\r\n",temp);
	if(temp>2.05)  //满电量
	{
		printf(" 100\r\n");
	}
	if(temp>1.9&&temp<2.05)  //   75%以上
	{
		printf(" 75\r\n");
	}
	if(temp>1.7&&temp<1.9)  //50%   以上
	{
		printf(" 50\r\n");
	}
	if(temp>1.5&&temp<1.7)//    25%  以上
	{
		printf(" 25\r\n");
	}
	if(temp>1.375&&temp<1.5)     //没电！！！
	{
		printf(" 0\r\n");
	}
}

void huxi_white_2()
{
	u8 i;
	for(i=0;i<=240;i+=5)
	{
		send24_GRB(i,i,i);
		delay_ms(20);
	}
	delay_ms(2000);
	for(i=240;i>=5;i-=5)
	{
		send24_GRB(i,i,i);
		delay_ms(20);
	}
	send24_GRB(0,0,0);
	delay_ms(2000);
}
void delay_white()
{
	u16 i;
	send24_GRB(255,255,255);
	for(i=0;i<60;i++)
		delay_ms(1000);//使用定时器来进行延时
	send24_GRB(0,0,0);	
}
void huxi_color()
{
	u8 i,j,k;
	for(i=0;i<250;i+=2)
	{
		if(i<=100)
		{
			send24_GRB(i,0,0);
			delay_ms(20);
		}
		if((i<=200)&&(i>100))
		{
			send24_GRB(i,i-100,0);
			delay_ms(20);
		}
		if((i<250)&&(i>200))
		{
			send24_GRB(i,i-100,i-200);
			delay_ms(20);
		}
	}
	delay_ms(2000);
	for(i=250;i>=2;i-=2)
	{
		if((i<250)&&(i>=200))
		{
			send24_GRB(i,i-100,i-200);
			delay_ms(20);
		}
		if((i<200)&&(i>=100))
		{
			send24_GRB(i,i-100,0);
			delay_ms(20);
		}
		if((i<100)&&(i>=4))
		{
			send24_GRB(i,0,0);
			delay_ms(20);
		}
	}
	send24_GRB(0,0,0);
	delay_ms(2000);
}
void huxi_white_3()
{
	u8 i,j,k;
	j=240;
	for(k=0;k<120;k++)
	{
		for(i=0;i<=j;i+=2)
		{
			send24_GRB(i,i,i);
			delay_ms(20);
		}
		delay_ms(2000);
		for(i=j;i>=4;i-=2)
		{
			send24_GRB(i,i,i);
			delay_ms(20);
		}
		send24_GRB(0,0,0);
		delay_ms(2000);
		j-=2;
	}
}
void huxi_white()//暂时先不用了   使用huxo_white_2 作为呼吸函数
{
	send24_GRB(0,0,0);
	delay_ms(50);
	send24_GRB(15,15,15);
	delay_ms(50);
	send24_GRB(30,30,30);
	delay_ms(50);
	send24_GRB(45,45,45);
	delay_ms(50);
	send24_GRB(60,60,60);
	delay_ms(50);
	send24_GRB(75,75,75);
	delay_ms(50);
	send24_GRB(90,90,90);
	delay_ms(50);
	send24_GRB(105,105,105);
	delay_ms(50);
	send24_GRB(120,120,120);
	delay_ms(50);
	send24_GRB(135,135,135);
	delay_ms(50);
	send24_GRB(150,150,150);
	delay_ms(50);
	send24_GRB(165,165,165);
	delay_ms(50);
	send24_GRB(180,180,180);
	delay_ms(50);
	send24_GRB(195,195,195);
	delay_ms(50);
	send24_GRB(210,210,210);
	delay_ms(50);
	send24_GRB(225,225,225);
	delay_ms(50);
	send24_GRB(240,240,240);
	delay_ms(50);
	send24_GRB(255,255,255);
	
	delay_ms(1000);
	send24_GRB(240,240,240);
	delay_ms(50);
	send24_GRB(225,225,225);
	delay_ms(50);
	send24_GRB(210,210,210);
	delay_ms(50);
	send24_GRB(195,195,195);
	delay_ms(50);
	send24_GRB(180,180,180);
	delay_ms(50);
	send24_GRB(165,165,165);
	delay_ms(50);
	send24_GRB(150,150,150);
	delay_ms(50);
	send24_GRB(135,135,135);
	delay_ms(50);
	send24_GRB(120,120,120);
	delay_ms(50);
	send24_GRB(105,105,105);
	delay_ms(50);
	send24_GRB(90,90,90);
	delay_ms(50);
	send24_GRB(75,75,75);
	delay_ms(50);
	send24_GRB(60,60,60);
	delay_ms(50);
	send24_GRB(45,45,45);
	delay_ms(50);
	send24_GRB(30,30,30);
	delay_ms(50);
	send24_GRB(15,15,15);
	delay_ms(50);
	send24_GRB(0,0,0);
	
}

void WIFI_AP_STA()
{
	u8 t;
	u8 len;
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CWMODE=2\r\n");
//	delay_ms(5);
//	LED0=0;
//	delay_ms(1000);
//	LED0=1;
//	delay_ms(1000);
#if 1
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CWMODE=3\r\n");
	delay_ms(5);
	LED0=0;
	delay_ms(1000);
	LED0=1;
	delay_ms(1000);
	
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+RST\r\n");
//	delay_ms(5);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
	 
	 USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+RST\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CWSAP=\"YCHS\",\"123456789\",11,3\r\n");
//	delay_ms(5);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);

	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CWSAP=\"qiaojijun\",\"123456789\",11,3\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
#endif	 
	 
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPMUX=1\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CIPSERVER=1,8080\r\n");
//	delay_ms(100);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
	 
	 USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPSERVER=1,8080\r\n");
	delay_ms(100);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 
	 
//	 USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CIPSTO=2880\r\n");
//	delay_ms(100);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
	 
	 	 USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPSTO=2880\r\n");
	delay_ms(100);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
}


void WIFI_AP_STA_yanchanghong()
{
	u8 t;
	u8 len;
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CWMODE=2\r\n");
//	delay_ms(5);
//	LED0=0;
//	delay_ms(1000);
//	LED0=1;
//	delay_ms(1000);
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CWMODE=3\r\n");
	delay_ms(5);
	LED0=0;
	delay_ms(1000);
	LED0=1;
	delay_ms(1000);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+RST\r\n");
//	delay_ms(5);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
//	 LED0=0;
//	 delay_ms(1000);
	 
	 USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+RST\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 LED0=0;
	 delay_ms(1000);
	 
//	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CWSAP=\"YCH\",\"123456789\",11,3\r\n");
//	delay_ms(5);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);

	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CWSAP=\"QJJ2021\",\"20150703\",11,3\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 
	 
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPMUX=1\r\n");
	delay_ms(5);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPSERVER=1,8080\r\n");
	delay_ms(100);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
	 
//	 USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CIPSERVER=1,8080\r\n");
//	delay_ms(100);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
	 
	 
//	 USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_printf(UART4,"AT+CIPSTO=2880\r\n");
//	delay_ms(100);
//	 LED0=0;
//	 delay_ms(1000);
//	 LED0=1;
//	 delay_ms(1000);
	 
	 	 USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_printf(UART4,"AT+CIPSTO=0\r\n");
	delay_ms(100);
	 LED0=0;
	 delay_ms(1000);
	 LED0=1;
	 delay_ms(1000);
}



