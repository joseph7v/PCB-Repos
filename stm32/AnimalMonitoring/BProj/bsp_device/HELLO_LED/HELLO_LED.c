#include "HELLO_LED.h"

void HELLO_LEDInit()
{  	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // ʹ��GPIOFʱ��

	// GPIOE2,E3,E4��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // ����
	GPIO_Init(GPIOE, &GPIO_InitStructure);  // ��ʼ��GPIO

	GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_3);  // GPIOF9,F10���øߣ�����
}

void HELLO_LED1_ON()
{
	ledGREEN = 0;
}
void HELLO_LED1_OFF()
{
	ledGREEN = 1;
}

void HELLO_LED1_NOT()
{
	ledGREEN = !ledGREEN;
}

void HELLO_LED2_ON()
{
	ledYELLOW = 0;
}
void HELLO_LED2_OFF()
{
	ledYELLOW = 1;
}

void HELLO_LED3_NOT()
{
	ledBLUE = !ledBLUE;
}

void BLUE_LED_ON(void)
{
	ledBLUE = 0;
}
void BLUE_LED_OFF(void)
{
	ledBLUE = 1;
}
void BLUE_LED_NOT(void)
{
	ledBLUE = !ledBLUE;
}





