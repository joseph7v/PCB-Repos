/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#ifndef __BC95_H
#define __BC95_H
#include "common.h"

#define AT_TICKLESS_ON 1	//�Ƿ�ʼ��BC95�����ȴ�usart�ź�ʱ������tickless
#define  AT_CHECK_EVERYTIME		300 //�����ڷ���ATָ���ÿ��200ms����һ�����鿴�Ƿ�ʱ�䳬�����趨�ĵȴ�ʱ��	
#define WAITINGTIME_Y    3//��ʾ�ȴ�ʱ���������
#define WAITINGTIME_X    12//��ʾ�ȴ�ʱ��ĺ�����
#define WAITINGTIME_DISP_DIGITS    4//��ʾ�ȴ�ʱ��ĺ�����
//���岽��
u8 AttachNetwork(void);
//��������
u8 AT_NRB(void);
u8 AT_Test(void);
//����ATָ���������
u8 CDP_Get_Cfg(char * ip_port);
u8 CDP_Cfg(char * ip_port);
u8 CFUN_Get_Set(void);
u8 CFUN_Set(void);
u8 NBAND_Get_Set(char * nband);
u8 NBAND_Set(char * nband);
u8 CGATT_Get_Set(void);
u8 CGATT_Set(void);
//���ݷ���
u8 MSG_Send(char * msg,char * len);
u8 intMSG_Send(u16 num,u8 len);//����int  message
//���ݽ���
u8 CMD_Get(void);
//��̵ĺ���
void StartWaitMsg(void);
void StopWaitMsg(void);

#endif

