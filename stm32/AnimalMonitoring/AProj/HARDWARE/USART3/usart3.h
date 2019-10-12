#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  

#define USART3_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬

void usart3_init(u32 bound);				//����3��ʼ�� 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
void U3_Send_Date(u8* dbuf,u16 len);
void U3_Send_Byte(char byte);

void F407USART3_SendByte(u8 Data);
void F407USART3_SendBytes(u8 *Data,u16 leng);
void F407USART3_SendString(char *str);

#define F407USART3_RECEIVE_BUFF_ENABLE			1
#if(F407USART3_RECEIVE_BUFF_ENABLE==1)
	#define F407USART3_RECEIVE_BUFF_SIZE		2048

	u8 F407USART3_buffWrite(u8 data);
	u8 F407USART3_buffWrites(u8 *data,u16 length);
	u8 F407USART3_buffRead(u8 *data);
	u16 F407USART3_buffReads(u8 *data,u16 length);
	u16 F407USART3_buffLength(void);
#endif


#endif













