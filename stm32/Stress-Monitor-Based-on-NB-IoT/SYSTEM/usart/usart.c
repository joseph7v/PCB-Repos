#include "usart.h"


#if SYSTEM_SUPPORT_OS 
#include "FreeRTOS.h"//FreeRTOS
#endif



#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
	USART3->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART3_RX   //���ʹ���˽���
   	

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

#endif	


UART_HandleTypeDef husart3; //UART���

USART usart = {0,0,0,0,0};

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	//UART ��ʼ������
	husart3.Instance=USART3;					    //USART1
	husart3.Init.BaudRate=bound;				    //������
	husart3.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	husart3.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	husart3.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	husart3.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	husart3.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	husart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_Init(&husart3) != HAL_OK)//HAL_UART_Init()��ʹ��UART1
	{
		Error_Handler(__FILE__, __LINE__);
	}	
	
#if EN_USART3_RX   //���ʹ���˽���
	HAL_UART_Receive_IT(&husart3, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
#endif
	
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART3)//����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;			//PB10��A11
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLDOWN;			//ԭ��������
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB10��A10

		
#if EN_USART3_RX       //���ʹ���˽�����ô��Ҫ�����ж�
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,0,0);			//��ռ���ȼ�0�������ȼ�1
#endif	
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)//����Ǵ���1
	{
		if(usart.num >= USART_REC_LEN)//������Χ�������⡣���½��ա�
		{
			usart.num = 0;
			return;
		}
		usart.USART_RX_BUF[usart.num] = aRxBuffer[0];//(USART3->DR);	//��ȡ���յ�������	
		usart.num++;
		usart.USART_RX_BUF[usart.num] = 0;//ȷ������һ���ǽ�β����0

	}
}
//����1�жϷ������
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
	
	HAL_UART_IRQHandler(&husart3);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&husart3) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&husart3, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
} 






