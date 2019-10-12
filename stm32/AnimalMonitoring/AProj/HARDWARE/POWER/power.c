#include "power.h"
#include "delay.h"
#include "usart.h"
/*
#define VCC_DVC   PBout(13)	
#define VCC_GPS   PBout(14)	
#define VCC_USB   PBout(15)	

#define VCC_WIFI  PCout(4)	
#define VCC_4G    PCout(5)	
 */

/**
 * ģ���Դ����
 */
void Power_Ctrl_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��

	//GPIOB13,GPIOB14,GPIOB15��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);            //��ʼ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //GPIOB13,B14,B15���õ�
	
	//GPIOC4,GPIOC5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);            //��ʼ��
	GPIO_ResetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);    //GPIOC0,C1,C2���õ�
}


void Cam_Crtl_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOAʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1);//GPIOC8,C9���øߣ�����
}

//��������֪ͨ������ջ�¼�񣨸�����������ã�������ʱ��0.5~1s��Ȼ������
void USB_Photograph(void)
{	

	USB_PHO = 1;
	printf("����...\r\n");
	delay_ms(1000);
	USB_PHO = 0;	
	printf("ing .....\r\n");

}

//��������Ƿ�������״̬������ƽ����ʱ����֪����ⲿ�豸Ҫ��USB�ˣ��ⲿ�豸���USB���Ӻ���԰���������3���ͣ����ɻָ����
void USB_Connecting(u8 ctrl)
{	
	if(ctrl==1)
	{
		USB_CNT = 1;
		printf("�������...\r\n");
	}else
	{
		USB_CNT = 0;
		printf("�ָ����...\r\n");
	}
	
}

void USB_CONNECT_ON()
{
	VCC_USB = 1;
}
void USB_CONNECT_OFF()
{
	VCC_USB = 0;
}

u8 openUSB(void)
{
	USB_CONNECT_ON();
	delay_ms(100);
	USB_Connecting(1);
	delay_ms(10);
}

u8 closeUSB(void)
{
	delay_ms(10);
	USB_Connecting(0);
}


