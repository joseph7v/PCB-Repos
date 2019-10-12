/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */


#include "key.h"
#include "led.h"

//IIC��ʼ��
void KEY_Init(void)
{
    GPIO_InitTypeDef gpioinitstruct;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC13��ʼ������
    gpioinitstruct.Pin=GPIO_PIN_13;
    gpioinitstruct.Mode=GPIO_MODE_INPUT;  //�������
    gpioinitstruct.Pull=GPIO_PULLUP;          //����
    gpioinitstruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&gpioinitstruct);
 
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����

//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0))
	{
		HAL_Delay(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRESSED;

	}	  
	else if(!key_up&&(KEY0==1))
		key_up = 1;
 	return 0;// �ް�������
}

//��������
//����������£�LED��ȡ��
void key_test(void)
{
	if(KEY_Scan(0) == KEY0_PRESSED)
	{
		HAL_GPIO_TogglePin(LED2_GPIO_PORT,LED2_PIN);
	}
}


