#include "key.h"
#include "delay.h"
/**
 * @description: key IO initial
 * @param {type}  none
 * @return: none 
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // KEY_WKUP
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @description: Key value scan
 * @param u16 divtime Scan interval
 * @return: key value
 */
u8 KEY_Scan(u16 divtime)
{
	static u8 key_up = 1; //�������ɿ���־
	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY_WKUP == 0))
	{
		delay_ms(divtime); //ȥ����
		key_up = 0;
		if (KEY1 == 0)
			return 1;
		else if (KEY2 == 0)
			return 2;
		else if (KEY_WKUP == 0)
			return 3;
	}
	else if (KEY1 == 1 && KEY2 == 1 && KEY_WKUP == 1)
		key_up = 1;
	return 0;
}

uint8_t key1_down = 0;	 // key1 state
uint8_t key2_down = 0;	 // key2 state
uint8_t key_wkup_down = 0; // key_wkup state
//����ɨ��������
void KeyScan_Task(void *p_arg)
{
	OS_ERR err;
	u8 value;
	const u16 divtime = 30;
	KEY_Init();
	printf("KeyScan_Task run\r\n");
	while (1)
	{
		value = KEY_Scan(divtime);
		switch (value)
		{
		case 0:
			break;
		case 1:
			printf("key1_down\r\n");
			key1_down = 1;
			break;
		case 2:
			printf("key2_down\r\n");
			key2_down = 1;
			break;
		case 3:
			printf("key_wkup_down\r\n");
			key_wkup_down = 1;
			break;
		}
		OSTimeDly(divtime, OS_OPT_TIME_DLY, &err);
	}
}
