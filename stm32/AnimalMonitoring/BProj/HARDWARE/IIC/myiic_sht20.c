#include "myiic_sht20.h"
#include "delay.h"
  

//��ʼ��IIC
void IIC_SHT20_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	SHT20_IIC_SCL=1;
	SHT20_IIC_SDA=1;

}
//����IIC��ʼ�ź�
void IIC_SHT20_Start(void)
{
	SHT20_SDA_OUT();     //sda�����
	SHT20_IIC_SDA=1;	  	  
	SHT20_IIC_SCL=1;
	delay_us(4);
 	SHT20_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SHT20_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}	  
//����IICֹͣ�ź�
void IIC_SHT20_Stop(void)
{
	SHT20_SDA_OUT();//sda�����
	SHT20_IIC_SCL=0;
	SHT20_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SHT20_IIC_SCL=1; 
	delay_us(1);
	SHT20_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_SHT20_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SHT20_SDA_IN();      //SDA����Ϊ����  
	SHT20_IIC_SDA=1;delay_us(1);	   
	SHT20_IIC_SCL=1;delay_us(1);	 
	while(SHT20_READ_SDA)
	{
		ucErrTime++;
	//	delay_us(1);
		if(ucErrTime>250)
		{
			IIC_SHT20_Stop();
			return 1;
		}
	}
	SHT20_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_SHT20_Ack(void)
{
	SHT20_IIC_SCL=0;
	SHT20_SDA_OUT();
	SHT20_IIC_SDA=0;
	delay_us(20);
	SHT20_IIC_SCL=1;
	delay_us(2);
	SHT20_IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_SHT20_NAck(void)
{
	SHT20_IIC_SCL=0;
	SHT20_SDA_OUT();
	SHT20_IIC_SDA=1;
	delay_us(5);
	SHT20_IIC_SCL=1;
	delay_us(5);
	SHT20_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_SHT20_Send_Byte(u8 txd)
{                        
    u8 t;   
		SHT20_SDA_OUT(); 	    
    SHT20_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        SHT20_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(5);   //��TEA5767��������ʱ���Ǳ����
		SHT20_IIC_SCL=1;
		delay_us(5); 
		SHT20_IIC_SCL=0;	
		delay_us(5);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_SHT20_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SHT20_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
    SHT20_IIC_SCL=0; 
    delay_us(5);
		SHT20_IIC_SCL=1;
    receive<<=1;
    if(SHT20_READ_SDA)receive++;   
		delay_us(5); 
  }					 
    if (!ack)
        IIC_SHT20_NAck();//����nACK
    else
        IIC_SHT20_Ack(); //����ACK   
    return receive;
}



























