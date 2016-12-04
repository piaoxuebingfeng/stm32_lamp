#include "key.h"
#include "delay.h"
//������ʼ������ 
//PC4��PC5 ���ó�����
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTBʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(KEY_C4==0||KEY_C5==0)
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_C4==0)return KEYC4_PRES;
		else if(KEY_C5==0)return KEYC5_PRES; 
	}	     
	return 0;// �ް�������
}
