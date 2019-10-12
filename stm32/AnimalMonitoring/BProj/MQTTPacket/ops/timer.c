/**
  ******************************************************************************
  * @file    timer.c
  * $Author: �ɺ�̤ѩ $
  * $Revision: 17 $
  * $Date:: 2014-10-25 11:16:48 +0800 #$
  * @brief   ��ʱ����غ���.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, EmbedNet</center>
  *<center><a href="http:\\www.embed-net.com">http://www.embed-net.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"

/**
  * @brief  ��ʼ����ʱ���ж�
  * @param  None
  * @retval None
  */
void Timer_Interrupts_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ��ʼ����ʱ�� 
  * @param  None
  * @retval None
  */
void Timer_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_DeInit(TIM3);                              					//��λTIM3��ʱ��
			
	/* TIM3 configuration */
	TIM_TimeBaseStructure.TIM_Period = 200;        					// 100ms    
	TIM_TimeBaseStructure.TIM_Prescaler = 36000;    				// ��Ƶ36000      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		// ʱ�ӷ�Ƶ 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//�����������ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Clear TIM3 update pending flag[���TIM3����жϱ�־] */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);

	/* Enable TIM3 Update interrupt [TIM3����ж�����]*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	/* TIM3����������*/
	TIM3->CNT=0;
	/* TIM3 enable counter [����TIM3����]*/
	TIM_Cmd(TIM3, DISABLE);  
	/*Config interrupts*/
	Timer_Interrupts_Config();
}



/**
  * @brief  �����ʱ���������Ĵ�������ֵ��������ʱ��
  * @param  None
  * @retval None
  */
void Timer_Start(void)
{
	TIM3->CNT=0;//����������Ĵ�����ֵ�����Լ�С��֡�����
  /* Enable the TIM Counter */
	TIM_Cmd(TIM3, ENABLE); 
}

/**
  * @brief  ֹͣ��ʱ���������ʱ���ļ���ֵ
  * @param  None
  * @retval None
  */
void Timer_Stop(void)
{ 
  /* Disable the TIM Counter */
	TIM_Cmd(TIM3, DISABLE);
}

