#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED�˿ڶ���
#define LED_G PEout(2)	// �̵�	 
#define LED_Y PEout(3)	// �Ƶ�

void LED_Init(void);//��ʼ��		 	

void LED_YELLOW_ON(void);
void LED_YELLOW_OFF(void);
void LED_YELLOW_NOT(void);
void LED_GREEN_ON(void);
void LED_GREEN_OFF(void);
void LED_GREEN_NOT(void);

#endif
