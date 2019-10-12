/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */


#include "lowpower.h"
#include "led.h"
#include "i2c_oled.h"
#include "io.h"
#include "usart.h"

RTC_HandleTypeDef hrtc;


//*******************************************************
//�������ƣ�RTC_Init
//��������������жϵ�ʱ��
//�������ã�RTC��ʼ��
//���ز�������
//��ע��RTC���ѣ�������wkup����
//*******************************************************
void RTC_Init(u16 WKUP_Seconds)
{	
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	/**Initialize RTC Only 
	*/
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}

	/**Initialize RTC and set the Time and Date 
	*/
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
	sTime.Hours = 0x0;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}

	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = RTC_MONTH_JANUARY;
	sDate.Date = 0x1;
	sDate.Year = 0x0;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}

	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
	}
	/**Enable the WakeUp 
	*/
	//ע�⣡�����������Ѿ��޸���HAL_RTCEx_SetWakeUpTimer_IT�ڲ���䣬��RTC��WKUP��ʱ���ص���
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WKUP_Seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
	
}
//*******************************************************
//�������ƣ�Sys_Entry_Standby
//���������
//�������ã����ʵ�ֽ������ģʽ
//���ز�������
//��ע��RTC���ѣ�������wkup����
//*******************************************************
void Sys_Entry_Standby(void)
{
	OLED_OFF();//OLED����
	//IO���߻�������
	LED2_OFF();LED1_OFF();
	BEEP_OFF();USART3_TX_RESET();USART3_RX_RESET();
	
	HAL_MspDeInit();
	
	HAL_PWR_EnableBkUpAccess(); //���������ʹ��,CDP��
	
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);//���SBF��־λ�����ڴ���ģʽ�ı�־λ

	__HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);//RTCд�����ر�
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//���wkup��ʱ���жϱ�־λ
	__HAL_RTC_WAKEUPTIMER_ENABLE(&hrtc);//RTC��WKUP��ʱ����	
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//���WTUF��WKUP��ʱ����0ʱ���жϱ�־λ	

	__HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);//RTCд����ʹ��

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);//���PWR_CSR��WUF��WUKEUP��־
	
	SET_BIT(PWR->CR, PWR_CR_CWUF);//���ѱ�־λ����	
	
	
	HAL_PWR_EnterSTANDBYMode();//�������ģʽ	
}

void PreSleepProcessing(uint32_t ulExpectedTime)
{
	//PB��UART�ж���Ҫ��Ӧ��
	LED2_CLK_DISABLE;//PDʱ��
	LED1_CLK_DISABLE;//PCʱ��
	LMCS_CLK_DISABLE;//PCʱ��
	//ADC1��ʹ�õ�ʱ��Ŵ��������ﲻ��Ҫ�ر�
}

void PostSleepProcessing(uint32_t ulExpectedTime)
{
	LED2_CLK_ENABLE;//PD
	LED1_CLK_ENABLE;//PC
	LMCS_CLK_ENABLE;//PCʱ��


}

void RealWaitingTimeProcessing(uint32_t ulCompleteTickPeriods)
{
	if(usart.WAIT_START)	
		usart.waittime += ulCompleteTickPeriods;	
}
//RTC��WKUP�����ж�
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance == RTC)
	{
	}
}

