#ifndef _F407USART1_H_
#define _F407USART1_H_

#include "usertypedef.h"

//===============================================
//=================Ӳ����弶����
//===============================================
void F407USART1_Init(void);

void F407USART1_SendByte(uint8 Data);
void F407USART1_SendInt_DEC(uint16 value);
void F407USART1_SendString(char *str);
void F407USART1_SendDouble_DEC(double value,uint8 point);

#define USART1_DEBUG 0

#endif
