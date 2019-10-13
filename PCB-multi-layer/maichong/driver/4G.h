/******************** (C) COPYRIGHT 2016 CETC27-CETIIC-ZYGD ********************
* File Name          : 4G.h
* Author             : WZJ
* Version            : V1.0
* Date               : 11/07/2016
* Description        : 4Gģ����ԶEC20����
*******************************************************************************/

#ifndef __4G_H__
#define __4G_H__

#include "stm32f10x_lib.h"

#define Version                                     "1"             //�汾�ţ�V1.0 ��1��99
#define Context_Number                              16              //1-16  EC20֧���������������16
#define Connect_Number                              11              //0-11  EC20ÿ��������֧�������������12
#define Channel_Number                              4               //0-11  EC20ÿ��������֧�������������12
#define GPRS_Reconnect_Time                         150             //�����������λ����
#define GPRS_React_Time                             3               //ע������������λ����
//  struct WirelessConnect
//  {
// //       vu8 Connecting_Chn;                                                 //���ڽ������ӵ�ͨ����
//      vu8 Channel_Switch[Channel_Number];                     //��ͨ������
//      vu8 Connect_State[Channel_Number];                      //��ͨ������״̬
//  };

#define GPIO_CTRL                         GPIOC
#define GPIO_PIN_GPRS_RESET               GPIO_Pin_7
#define GPIO_PIN_GPRS_Pow_Ctrl            GPIO_Pin_1
#define GPIO_PIN_GPRS_LED                 GPIO_Pin_5
#define GPRS_Buf_Size                                           1500
#define GPRS_Command_Size                           250


extern u8 IP_Port[4][6], flag_OK, flag_update;
extern u8 filldata_buf[];
//extern u8 APN_Lenth, APN_Username_Lenth, APN_Password_Lenth;
extern char APN[], APN_Username[], APN_Password[];
extern char GPRS_Receive_Data[], GPRS_Receive_Command[];
extern u8 Server_Command_Chn;
extern vu8 GPRS_State[], Connect_Channel_On[]; //Connect_On[4],
extern u8 flag_OK;
extern u16 GPRS_Update_Pointer;
extern vu16 GPRS_Reset_Count;   //20170523

extern void GPRS_Supervise(void);
extern void GPRS_AT(char *ptr, u8 len);
extern void GPRS_Send(u8 chn);
extern u8 GPRS_Close_Connect(u8 chn);
extern void GPRS_Query_Signal(void);
extern void GPRS_Send_AllChn(void);
extern void GPRS_Send_SingleChn(u8 chn);

u8 GPRS_Query_SendSuccess(u8 chn);

#endif

/******************** (C) COPYRIGHT 2016 CETC27-CETIIC-ZYGD ********************/
