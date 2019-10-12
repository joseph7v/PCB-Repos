#include "led.h" 
/**
* ������Vision4.0
* LED1 PE2 �̵�
* LED2 PE3 �Ƶ�
*/

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

	// GPIOE2,E3��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��

	GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3);//GPIOE2,E3���øߣ�����
}

void LED_YELLOW_ON(void)
{
	LED_Y=0;
}
void LED_YELLOW_OFF(void)
{
	LED_Y=1;
}

void LED_YELLOW_NOT(void)
{
	LED_Y=!LED_Y;
}

void LED_GREEN_ON(void)
{
	LED_G=0;
}
void LED_GREEN_OFF(void)
{
	LED_G=1;
}

void LED_GREEN_NOT(void)
{
	LED_G=!LED_G;
}





