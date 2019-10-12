/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "wwdg.h"
#include "led.h"

WWDG_HandleTypeDef hwwdg;


//WWDG�ĳ�ʼ��
void WWDG_Init(u8 win,u8 cnt , u32 psc)
{

	hwwdg.Instance = WWDG;
	hwwdg.Init.Prescaler = psc;
	hwwdg.Init.Window = win;
	hwwdg.Init.Counter = cnt;
	hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
	if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
	{
	Error_Handler(__FILE__, __LINE__);
	}

}
	
//wwdg�ĵݼ����������¸�ֵ
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
	HAL_WWDG_Refresh(hwwdg);	
}
