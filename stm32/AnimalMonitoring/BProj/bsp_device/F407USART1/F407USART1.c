#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
//#include "misc.h"
//#include "stdio.h"
//#include "stdarg.h"
#include "F407USART1.h"


#define F407USART1_SENDBYTE(x)		F407USART1_SendByte(x)

void F407USART1_Init(void)
{
}

/*
���ܣ��򴮿�2����һ���ֽ�����
��������Ҫ���͵��ֽ�����
����ֵ����
*/
void F407USART1_SendByte(uint8 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE)); 
   USART_SendData(USART1, Data); 
}

/*
���ܣ���һ��ʮ��λ������ת��Ϊ�ַ���ʽ���������
������value��ʮ��λ��������
����ֵ����
*/
void F407USART1_SendInt_DEC(uint16 value)
{
	uint16 dt=10000;
	uint8 ch=0;
	while(value<dt)
	{
		dt/=10;
		if(dt==0)F407USART1_SENDBYTE('0');
	}
	while(dt)
	{
		ch=value/dt;
		F407USART1_SENDBYTE('0'+ch);
		value-=(ch*dt);
		dt/=10;
	}
}
/*
���ܣ��򴮿ڷ���һ���ַ�������
������*str���ַ��� �׵�ַ
����ֵ����
*/
void F407USART1_SendString(char *str)
{
	u16 i=0;
	while(*(str+i)!=0)
	{
		F407USART1_SENDBYTE(*(str+i));
		i++;
	}
}
/*
���ܣ���һ������������ת��Ϊ�ַ���ʽ�����ָ�����С����λ��
������value������������
			point��ָ�����С����λ��
����ֵ����
*/
void F407USART1_SendDouble_DEC(double value,uint8 point)
{
	u8 i=0;
	double data=0;
	uint64 dat;
	if(value<1)
	{
		F407USART1_SENDBYTE('0');
	}
	for(data=1,i=0;data<value;data*=10,i++);
	//
	for(i+=point,data/=10;i>0;i--)
	{
		if(i==point)F407USART1_SENDBYTE('.');
		dat=value/data;
		data/=10;
		dat%=10;
		F407USART1_SENDBYTE('0'+dat);
	}
}





























