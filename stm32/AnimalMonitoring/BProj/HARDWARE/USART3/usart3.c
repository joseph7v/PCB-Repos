#include "usart3.h"
//===============================================
//=================Ӳ����弶����
//===============================================
/**
 * @description: ��ʼ������2IO
 * @param none
 * @return: none
 */
void F407USART3_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/**
 * @description: ��ʼ������3�ж�
 * @param {type} none
 * @return: none
 */
void F407USART3_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @description: ��ʼ������3�����а�����IO��ʼ�������ʼ�����жϳ�ʼ��
 * @param uint32_t btl set bound
 * @return: none
 */
void F407USART3_Init(uint32_t btl)
{
	USART_InitTypeDef USART_InitStructure;
	// ��UART3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	// ����IO
	F407USART3_IO_Init();
	USART_DeInit(USART3);
	// ���ô��ڲ���
	USART_InitStructure.USART_BaudRate = btl;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	// �����ж����ȼ�
	F407USART3_NVIC_Init();
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}
/**
 * @description: �򴮿�3����һ���ֽ�����
 * @param uint8_t Data
 * @return: none
 */
void F407USART3_SendByte(uint8_t Data)
{
	while (!(USART3->SR & USART_FLAG_TXE))
		;
	USART_SendData(USART3, Data);
}
/**
 * @description: �򴮿�3����ָ�����ȵ��ֽ�
 * @param uint8_t * Data ���ݵ�ַ; uint16_t leng ���ݾֳ���  
 * @return: 
 */
void F407USART3_SendBytes(char *Data, uint16_t leng)
{
	uint16_t i = 0;
	for (i = 0; i < leng; i++)
	{
		F407USART3_SendByte(*(Data + i));
	}
}
/**
 * @description: �����ַ���
 * @param char *str string address
 * @return: 
 */
void F407USART3_SendString(char *str)
{
	u16 i = 0;
	while (*(str + i) != 0)
	{
		F407USART3_SendByte(*(str + i));
		i++;
	}
}

#if (F407USART3_RECEIVE_BUFF_ENABLE == 1)
uint8_t F407USART3_buff[F407USART3_RECEIVE_BUFF_SIZE];  //UART3������
volatile uint16_t F407USART3_buffHead = 0; // ͷָ��
volatile uint16_t F407USART3_buffEnd = 0;  // βָ��
volatile uint8_t F407USART3_buffOverFlag = 0;  // ���Ǳ�־
///////////////////////////д��
/**
 * @description: ��F407USART3_buffд��һ���ֽ�
 * @param uint8_t data ��д���ֽ�
 * @return: 0 д��ʧ�ܣ�1 д��ɹ�
 */
uint8_t F407USART3_buffWrite(uint8_t data)
{
	if (F407USART3_buffHead != F407USART3_buffEnd || F407USART3_buffOverFlag == 0)
	{
		F407USART3_buff[F407USART3_buffEnd] = data;
		F407USART3_buffEnd++;
		if (F407USART3_buffEnd >= F407USART3_RECEIVE_BUFF_SIZE)
		{
			F407USART3_buffEnd = 0;
			F407USART3_buffOverFlag = 1;
		}
		return 1;
	}
	return 0;
}
/**
 * @description: ��F407USART3_buffд��ָ�������ַ�
 * @param uint8_t *data ��д���ַ���ַ, uint16_t length ��д���ַ�����
 * @return:  �ɹ���F407USART3_buffд���ַ�����
 */
uint8_t F407USART3_buffWrites(uint8_t *data, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		if (F407USART3_buffWrite(*(data + i)) == 0)
		{
			return 0;
		}
	}
	return i;
}

///////////////////////////��ȡ
/**
 * @description: ��F407USART3_buff�ж�ȡһ���ֽڵ�����
 * @param uint8_t *data ���ݵ�ַ
 * @return: 1 �ɹ���0 ʧ��
 * @note ѭ������
 */
uint8_t F407USART3_buffRead(uint8_t *data)
{
	if (F407USART3_buffHead != F407USART3_buffEnd || F407USART3_buffOverFlag == 1)
	{
		*data = F407USART3_buff[F407USART3_buffHead];
		F407USART3_buffHead++;
		if (F407USART3_buffHead >= F407USART3_RECEIVE_BUFF_SIZE)
		{
			F407USART3_buffHead = 0;
			F407USART3_buffOverFlag = 0;
		}
		return 1;
	}
	return 0;
}
/**
 * @description: ��F407USART3_buff�ж�ȡָ�����ȵ����ݵ�ָ����ַ
 * @param uint8_t *data ָ����ַ, uint16_t length ָ������
 * @return: uint16_t ��ȡ�ĳ���
 */
uint16_t F407USART3_buffReads(uint8_t *data, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		if (F407USART3_buffRead(data + i) == 0)
			return 0;
	}
	return length;
}
/**
 * @description: �񻺴�������
 * @param {type} none
 * @return: uint16_t ����������
 */
uint16_t F407USART3_buffLength(void)
{
	return F407USART3_RECEIVE_BUFF_SIZE * F407USART3_buffOverFlag + F407USART3_buffEnd - F407USART3_buffHead;
}
#endif // F407USART3_RECEIVE_BUFF_ENABLE

//===============================================
//=================�жϺ���
//===============================================
void USART3_IRQHandler(void)
{
	static uint8_t data = 0;
	static uint8_t step = 0;

	//������󣬱��봦��,����ᵼ������
	if (0x0008 & USART3->SR)
	{
		data = USART3->DR;
		step = 0;
	}
	else if (0x0020 & USART3->SR)
	{
		//�����ݻ��Զ�����жϱ�־λ
		data = USART3->DR;
		F407USART3_buffWrite(data);
	}
}
