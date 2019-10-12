#include "led.h"
/**
 * @description: LED IO initial
 * @param {type} none
 * @return: none
 */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //����
	GPIO_Init(GPIOE, &GPIO_InitStructure);			   //��ʼ��

	GPIO_SetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4); //GPIOE2,E3,E4���øߣ�����
}
/**
 * @description: LED control
 * @param {type} none
 * @return: none
 */
void LED_GREEN_ON(void)
{
	ledGREEN = 0;
}
void LED_GREEN_OFF(void)
{
	ledGREEN = 1;
}
void LED_GREEN_NOT(void)
{
	ledGREEN = !ledGREEN;
}

void LED_YELLOW_ON(void)
{
	ledYELLOW = 0;
}
void LED_YELLOW_OFF(void)
{
	ledYELLOW = 1;
}
void LED_YELLOW_NOT(void)
{
	ledBLUE = !ledBLUE;
}

void LED_BLUE_ON(void)
{
	ledBLUE = 0;
}
void LED_BLUE_OFF(void)
{
	ledBLUE = 1;
}
void BLED_LUE_NOT(void)
{
	ledBLUE = !ledBLUE;
}
