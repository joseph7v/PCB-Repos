#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)	//PE0 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) //PE1
#define WK_UP 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PA0

#define KEY1_PRES	1
#define KEY2_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
