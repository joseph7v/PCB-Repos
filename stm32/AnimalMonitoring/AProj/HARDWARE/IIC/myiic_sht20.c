#include "myiic_sht20.h"
#include "delay.h"
  

//初始化IIC
void IIC_SHT20_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	SHT20_IIC_SCL=1;
	SHT20_IIC_SDA=1;

}
//产生IIC起始信号
void IIC_SHT20_Start(void)
{
	SHT20_SDA_OUT();     //sda线输出
	SHT20_IIC_SDA=1;	  	  
	SHT20_IIC_SCL=1;
	delay_us(4);
 	SHT20_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SHT20_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(4);
}	  
//产生IIC停止信号
void IIC_SHT20_Stop(void)
{
	SHT20_SDA_OUT();//sda线输出
	SHT20_IIC_SCL=0;
	SHT20_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SHT20_IIC_SCL=1; 
	delay_us(1);
	SHT20_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_SHT20_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SHT20_SDA_IN();      //SDA设置为输入  
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
	SHT20_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_SHT20_Send_Byte(u8 txd)
{                        
    u8 t;   
		SHT20_SDA_OUT(); 	    
    SHT20_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        SHT20_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(5);   //对TEA5767这三个延时都是必须的
		SHT20_IIC_SCL=1;
		delay_us(5); 
		SHT20_IIC_SCL=0;	
		delay_us(5);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_SHT20_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SHT20_SDA_IN();//SDA设置为输入
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
        IIC_SHT20_NAck();//发送nACK
    else
        IIC_SHT20_Ack(); //发送ACK   
    return receive;
}



























