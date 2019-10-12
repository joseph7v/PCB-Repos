/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#ifndef _LOWPOWER_H
#define _LOWPOWER_H
#include "sys.h"

#define BACKUP_AREA   RTC_BKP_DR1
#define RESET_FROM_PWRDOWN 0  //�ӵ�������
#define RESET_FROM_STANDBY 1   //�Ӵ���ģʽ����
#define NEED_REATTACH_NETWORK	0  //��Ҫ������������
extern RTC_HandleTypeDef hrtc;

void Sys_Entry_Standby(void);
void RTC_Init(u16 WKUP_Seconds);
void PreSleepProcessing(uint32_t ulExpectedTime);
void PostSleepProcessing(uint32_t ulExpectedTime);
void RealWaitingTimeProcessing(uint32_t ulCompleteTickPeriods);


#endif


