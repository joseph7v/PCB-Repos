#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
#include "common.h"


#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART3_TX_SET()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET) 
#define USART3_TX_RESET()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET) 
#define USART3_RX_SET()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET) 
#define USART3_RX_RESET()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET) 

typedef struct{
	volatile u8 status;
	volatile u16 num;
	volatile u8  USART_RX_BUF[USART_REC_LEN];//���ڸ��������ݵ�ʱ���ж�
	volatile u8  WAIT_START;
	volatile u16 waittime;
}USART;
extern USART usart;

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef husart3; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);


#endif	/*_USART_H_*/
