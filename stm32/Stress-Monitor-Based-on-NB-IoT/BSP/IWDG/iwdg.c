/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */


#include "iwdg.h"


IWDG_HandleTypeDef IWDG_Handler;


void IWDG_Init(u8 prer,u16 rld)
{
  IWDG_Handler.Instance = IWDG;
  IWDG_Handler.Init.Prescaler = prer;
  IWDG_Handler.Init.Reload = rld;
  if (HAL_IWDG_Init(&IWDG_Handler) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}

