/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */


#include "IO.h"
#include "sys.h"

//�����źŵ����·��IO����
void IO_Init(void)
{
	GPIO_InitTypeDef  gpioinitstruct;


	/* Reset PIN to switch off the LED */
	HAL_GPIO_WritePin(LMCS_GPIO_PORT,LMCS_PIN , GPIO_PIN_SET);
	/* Enable the GPIO_LED Clock */
	LMCS_CLK_ENABLE;

	/* Configure the GPIO_LED pin */
	gpioinitstruct.Pin    = LMCS_PIN;
	gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull   = GPIO_NOPULL;
	gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(LMCS_GPIO_PORT , &gpioinitstruct);

	HAL_GPIO_WritePin(LMCS_GPIO_PORT,LMCS_PIN , GPIO_PIN_SET);	
	
}

//�����źŵ����·��IO����
void Beep_Init(void)
{
	GPIO_InitTypeDef  gpioinitstruct;


	/* Reset PIN to switch off the LED */
	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_PIN , GPIO_PIN_SET);
	/* Enable the GPIO_LED Clock */
	BEEP_CLK_ENABLE;

	/* Configure the GPIO_LED pin */
	gpioinitstruct.Pin    = BEEP_PIN;
	gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull   = GPIO_NOPULL;
	gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(BEEP_GPIO_PORT , &gpioinitstruct);

	BEEP_OFF();//�ر�BEEP	
	
}

