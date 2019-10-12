#include "timer.h"
#include "usbh_usr.h" 
#include "delay.h"
#include "led.h"
extern USBH_HOST  USB_Host;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;
//ͨ�ö�ʱ��5�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��5!
void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��5�����ж�
	//TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

////��ʱ��5�жϷ�����
//void TIM5_IRQHandler(void)
//{
//	static unsigned char t;
//	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //����ж�
//	{
//		USBH_Process(&USB_OTG_Core, &USB_Host);
//		t++;
//		if(t==200){ HELLO_LED2_NOT(); t=0;}
//		
//	}
//	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
//}

//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		
	OSIntEnter();   
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)//����ж� 
	{
		if(OSRunning!=TRUE)//OS��û����,��TIM3���ж�,10msһ��,��ɨ��USB
		{
			usbapp_pulling();
		}
	}				   
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ  
	OSIntExit(); 	    		  			    
}
