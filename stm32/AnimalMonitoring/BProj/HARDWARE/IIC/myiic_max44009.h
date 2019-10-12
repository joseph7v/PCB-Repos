#ifndef __MYIIC_MAX44009_H
#define __MYIIC_MAX44009_H
#include "sys.h"

   	   		   
//IO��������
#define MAX44009_SDA_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}	//PB11����ģʽ
#define MAX44009_SDA_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;} //PB11���ģʽ
//IO��������	 
#define MAX44009_IIC_SCL    PBout(10) //SCL
#define MAX44009_IIC_SDA    PBout(11) //SDA	 
#define MAX44009_READ_SDA   PBin(11)  //����SDA 

//IIC���в�������
void IIC_MAX44009_Init(void);                //��ʼ��IIC��IO��				 
void IIC_MAX44009_Start(void);				//����IIC��ʼ�ź�
void IIC_MAX44009_Stop(void);	  			//����IICֹͣ�ź�
void IIC_MAX44009_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_MAX44009_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_MAX44009_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_MAX44009_Ack(void);					//IIC����ACK�ź�
void IIC_MAX44009_NAck(void);				//IIC������ACK�ź�

void IIC_MAX44009_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_MAX44009_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















