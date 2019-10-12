#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RTC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140726
//����:RTC_Get_Week����,���ڸ�����������Ϣ,�õ�������Ϣ.
////////////////////////////////////////////////////////////////////////////////// 

//#define HEART  0
//#define DATA_ACQ  1 
//#define DATA_UDT  2
//#define PICR_DUP  3
//#define AUTO_PHO  4
//#define SCAN_CAM  5

//#define SUSPEND  0
//#define TOWORK   1
//typedef struct Tast_Set
//{
//	u16 no;
//	u16 work_flag;
//	u32 delta_time;
//	u32 remain_time;
//}tast_set; //����ṹ��  


//tast_set MY_Task(6);
//u32 BUF[18];  // 72���ֽ�
//memcpy(BUF,MY_Task,72);  // ���ṹ���е����ݸ��Ƶ���������
 
/**
 * ��Flash�ж����������ҵ��뷨��ֱ�Ӱѽṹ��д��Flash,�ȵ�ʱ��ֱ�Ӵ�Flash�ж�����
 */
//void Task_Init(void)
//{
//	Tast_Set[HEART] -> no = HEART;
//	Tast_Set[HEART] -> work_flag = TOWORK;
//	Tast_Set[HEART] -> delta_time = 100;
//	Tast_Set[HEART] -> delta_time = 50;
//}

//ʱ��ṹ��
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	vu16 msec;
	//������������
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;	//1~7,������1~����	
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

u8 My_RTC_Init(void);						//RTC��ʼ��
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������

void Sys_Enter_Standby(void);

void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);
void calendar_get_time(_calendar_obj *calendarx);
void calendar_get_date(_calendar_obj *calendarx);
#endif

















