/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */


#include "adc.h"
#include "io.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"

ADC_HandleTypeDef hadc;

/* ADC init function */
void ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_DISABLE;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_384CYCLES;//�Ӵ����ʱ��
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

}


//*******************************************************
//�������ƣ�GetAdcValue
//�����������
//�������ã��õ�ADC����ֵ
//���ز�������
//*******************************************************
u16 GetAdcValue(void)
{
	u16 adc_temp = 0;
	LM_ON();//��Ӧ������·�ĵ�Դ
	vTaskDelay(100);//�ȴ�ms�ȶ�����
	__HAL_RCC_ADC1_CLK_ENABLE();
	HAL_ADC_Start_IT(&hadc);
	vTaskDelay(20);
	HAL_ADC_Stop_IT(&hadc);
	LM_OFF();//�ر�Ӧ���źŵ����·�ĵ�Դ
	
	//��������Ȳɼ������ٹر�ʱ�ӣ���Ȼ�����0
	adc_temp = HAL_ADC_GetValue(&hadc);
	__HAL_RCC_ADC1_CLK_DISABLE();
	return adc_temp;

}






