#ifndef _F205_KEY_H_
#define _F205_KEY_H_




#include "MyTypedef_F4xx.h"
#include "usertypedef.h"


/*
���ܣ������仯�¼�
����������1�а��������仯������ֵ������2�����������µ�ʱ��
����ֵ����
*/
typedef void (*KeyEventCallback)(uint8,uint16);  				//*����������������ĺ���ָ���������*/

#define SHORT_DOWN_TIME_MS			300
#define CLICK_TIME_OUT					1000

#define KEY1		0x01
#define KEY2	  0x02
//#define KEY3		0x04
//#define KEY4	  0x08


#ifdef KEY1
#define KEY1_IOCOF		{RCC_AHB1Periph_GPIOE,GPIOE,GPIO_Pin_0,GPIO_Mode_IN,GPIO_Speed_2MHz,GPIO_OType_PP,GPIO_PuPd_UP};
#endif

#ifdef KEY2
#define KEY2_IOCOF		{RCC_AHB1Periph_GPIOE,GPIOE,GPIO_Pin_1,GPIO_Mode_IN,GPIO_Speed_2MHz,GPIO_OType_PP,GPIO_PuPd_UP};
#endif

#ifdef KEY3
#define KEY3_IOCOF		{RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_9,GPIO_Mode_IN,GPIO_Speed_2MHz,GPIO_OType_OD,GPIO_PuPd_UP};
#endif

#ifdef KEY4
#define KEY4_IOCOF		{RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_9,GPIO_Mode_IN,GPIO_Speed_2MHz,GPIO_OType_OD,GPIO_PuPd_UP};
#endif


void F205_KEYInit(KeyEventCallback upx,KeyEventCallback downTime,KeyEventCallback longdownx,KeyEventCallback downCount);
void F205_KEYRun(uint16 divtime);




#endif
