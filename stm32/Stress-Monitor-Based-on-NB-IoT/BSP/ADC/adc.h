/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#ifndef _ADC_H
#define _ADC_H
#include "sys.h"

#define ZJU_ADCx                                 ADC1
#define ZJU_ADCx_CLK_ENABLE()                    __HAL_RCC_ADC1_CLK_ENABLE()
#define ZJU_ADCx_CLK_DISABLE()                   __HAL_RCC_ADC1_CLK_DISABLE()
    
#define ZJU_ADCx_GPIO_PORT                       GPIOC
#define ZJU_ADCx_GPIO_PIN                        GPIO_PIN_0
#define ZJU_ADCx_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define ZJU_ADCx_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOC_CLK_DISABLE()

extern ADC_HandleTypeDef hadc;

void ADC_Init(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
u16 GetAdcValue(void);

#endif


