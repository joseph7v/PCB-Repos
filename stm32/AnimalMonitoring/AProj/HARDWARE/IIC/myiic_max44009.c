#include "myiic_max44009.h"
#include "delay.h"
	  
//��ʼ��IIC
void IIC_MAX44009_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
 
	MAX44009_IIC_SCL=1;
	MAX44009_IIC_SDA=1;

}
//����IIC��ʼ�ź�
void IIC_MAX44009_Start(void)
{
	MAX44009_SDA_OUT();     //sda�����
	MAX44009_IIC_SDA=1;	  	  
	MAX44009_IIC_SCL=1;
	delay_us(4);
 	MAX44009_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MAX44009_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}	  
//����IICֹͣ�ź�
void IIC_MAX44009_Stop(void)
{
	MAX44009_SDA_OUT();//sda�����
	MAX44009_IIC_SCL=0;
	MAX44009_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MAX44009_IIC_SCL=1; 
	delay_us(1);
	MAX44009_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_MAX44009_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MAX44009_SDA_IN();      //SDA����Ϊ����  
	MAX44009_IIC_SDA=1;delay_us(1);	   
	MAX44009_IIC_SCL=1;delay_us(1);	 
	while(MAX44009_READ_SDA)
	{
		ucErrTime++;
	//	delay_us(1);
		if(ucErrTime>250)
		{
			IIC_MAX44009_Stop();
			return 1;
		}
	}
	MAX44009_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_MAX44009_Ack(void)
{
	MAX44009_IIC_SCL=0;
	MAX44009_SDA_OUT();
	MAX44009_IIC_SDA=0;
	delay_us(20);
	MAX44009_IIC_SCL=1;
	delay_us(2);
	MAX44009_IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_MAX44009_NAck(void)
{
	MAX44009_IIC_SCL=0;
	MAX44009_SDA_OUT();
	MAX44009_IIC_SDA=1;
	delay_us(5);
	MAX44009_IIC_SCL=1;
	delay_us(5);
	MAX44009_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_MAX44009_Send_Byte(u8 txd)
{                        
    u8 t;   
		MAX44009_SDA_OUT(); 	    
    MAX44009_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MAX44009_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(5);   //��TEA5767��������ʱ���Ǳ����
		MAX44009_IIC_SCL=1;
		delay_us(5); 
		MAX44009_IIC_SCL=0;	
		delay_us(5);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_MAX44009_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MAX44009_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    MAX44009_IIC_SCL=0; 
    delay_us(5);
		MAX44009_IIC_SCL=1;
    receive<<=1;
    if(MAX44009_READ_SDA)receive++;   
		delay_us(5); 
  }					 
    if (!ack)
        IIC_MAX44009_NAck();//����nACK
    else
        IIC_MAX44009_Ack(); //����ACK   
    return receive;
}



























