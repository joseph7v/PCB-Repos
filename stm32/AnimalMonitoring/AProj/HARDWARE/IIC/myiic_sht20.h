#ifndef __MYIIC_SHT20_H
#define __MYIIC_SHT20_H
#include "sys.h"

   	   		   
//IO��������
#define SHT20_SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7����ģʽ
#define SHT20_SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7���ģʽ
//IO��������	 
#define SHT20_IIC_SCL    PBout(6) //SCL
#define SHT20_IIC_SDA    PBout(7) //SDA	 
#define SHT20_READ_SDA   PBin(7)  //����SDA 

//IIC���в�������
void IIC_SHT20_Init(void);                //��ʼ��IIC��IO��				 
void IIC_SHT20_Start(void);				//����IIC��ʼ�ź�
void IIC_SHT20_Stop(void);	  			//����IICֹͣ�ź�
void IIC_SHT20_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_SHT20_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_SHT20_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_SHT20_Ack(void);					//IIC����ACK�ź�
void IIC_SHT20_NAck(void);				//IIC������ACK�ź�

void IIC_SHT20_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_SHT20_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















