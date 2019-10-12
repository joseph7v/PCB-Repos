/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "myiic.h"
#include "main.h"
	

//IIC��ʼ��
void IIC_Init(void)
{
    GPIO_InitTypeDef gpioinitstruct;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PB6��B7��ʼ������
    gpioinitstruct.Pin=GPIO_PIN_6|GPIO_PIN_7;
    gpioinitstruct.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    gpioinitstruct.Pull=GPIO_PULLUP;          //����
    gpioinitstruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&gpioinitstruct);
    
    IIC_SDA=1;
    IIC_SCL=1;  
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
//	n_nops(1);
	__NOP;
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	n_nops(1);//��Ҫ��ʱ4us
	__NOP;
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	n_nops(1);//��Ҫ��ʱ4us
	__NOP;
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
//	n_nops(1);//��Ҫ��ʱ4us		
	__NOP;
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;__NOP;//n_nops(1);//��Ҫ��ʱ1us	   
	IIC_SCL=1;__NOP;//n_nops(1);//��Ҫ��ʱ1us	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
//	n_nops(1);//��Ҫ��ʱ2us
	__NOP;
	IIC_SCL=1;
//	n_nops(1);//��Ҫ��ʱ2us
	__NOP;
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
//	n_nops(1);//��Ҫ��ʱ2us
	__NOP;
	IIC_SCL=1;
//	n_nops(1);//��Ҫ��ʱ2us
	__NOP;
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
//		n_nops(1);//��Ҫ��ʱ2us  
		__NOP;
		IIC_SCL=1;
//		n_nops(1);//��Ҫ��ʱ2us
		__NOP;
		IIC_SCL=0;	
//		n_nops(1);//��Ҫ��ʱ2us
		__NOP;
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
//        n_nops(1);//��Ҫ��ʱ2us
		__NOP;
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
//		n_nops(1);//��Ҫ��ʱ1us
		__NOP;
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

void n_nops(u8 n)
{
	u8 num = n;
	while(num--)
	{};
}



