/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */
#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern TIM_HandleTypeDef htim2; 

void TIM2_Init(u16 psc,u16 arr);
	 
#endif

