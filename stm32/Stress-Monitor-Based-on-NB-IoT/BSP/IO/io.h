/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#ifndef _IO_H
#define _IO_H
#include "sys.h"

//��������LM25956��ʹ��
#define LMCS_GPIO_PORT    GPIOC
#define LMCS_PIN          GPIO_PIN_1
#define LMCS_CLK_ENABLE   __HAL_RCC_GPIOC_CLK_ENABLE() 
#define LMCS_CLK_DISABLE   __HAL_RCC_GPIOC_CLK_DISABLE() 
#define LM_ON()        HAL_GPIO_WritePin(LMCS_GPIO_PORT, LMCS_PIN, GPIO_PIN_SET)  
#define LM_OFF()          HAL_GPIO_WritePin(LMCS_GPIO_PORT, LMCS_PIN, GPIO_PIN_RESET) 

//�������ڷ�������ʹ��
#define BEEP_GPIO_PORT    GPIOC
#define BEEP_PIN          GPIO_PIN_4
#define BEEP_CLK_ENABLE   __HAL_RCC_GPIOC_CLK_ENABLE() 
#define BEEP_CLK_DISABLE   __HAL_RCC_GPIOC_CLK_DISABLE() 
#define BEEP_ON()        HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_PIN, GPIO_PIN_SET)  
#define BEEP_OFF()          HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_PIN, GPIO_PIN_RESET) 


void IO_Init(void);
void Beep_Init(void);

#endif


