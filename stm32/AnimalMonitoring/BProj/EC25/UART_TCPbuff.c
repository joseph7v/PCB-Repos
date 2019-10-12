#include "UART_TCPbuff.h"
#include "usart.h"
#include "usart.h"
///////////////////Verify received characters
#define WAITSTRINGBUFF_LENGTH 50
uint8_t waitStringBuff[WAITSTRINGBUFF_LENGTH];
static uint8_t waitStringBuffLen = 0;  // ���ؼ���
static uint8_t waitStringBuffStep = 0;  // ���ؼ���2
static uint8_t waitStringOK = 0;  // status flag

/**
 * @description: ���õȴ�ָ���ַ�������UART_TCPbuff_Run�����н����ж�
 * @param int8_t *str ָ���ַ�����ַ, uint8_t strlen ָ���ַ�������
 * @return: ʶ���ַ�����
 */
uint8_t waitString_set(char *str, uint8_t strlen)
{
	uint8_t i = 0;
	for (i = 0; i < strlen && i < WAITSTRINGBUFF_LENGTH; i++)
	{
		waitStringBuff[i] = str[i];
	}
	waitStringBuffLen = i;
	waitStringOK = 0;
	return i;
}
/**
 * @description: waitStringOK set function
 * @param uint8_t value set value
 * @return: none
 */
void waitStringOK_set(uint8_t value)
{
	waitStringOK = value;
}
/**
 * @description: ��ȡwaitStringOK��ֵ
 * @param {type}  none
 * @return: none
 */
uint8_t waitStringOK_get(void)
{
	return waitStringOK;
}
/**
 * @description: �ȴ�һ�����ù����������
 * @param {type} 
 * @return: 1 �ɹ���0 ʧ��
 */
uint8_t WaitString_OKflag(uint16_t timeout)
{
	OS_ERR err;
	uint16_t time = 0;
	while (!waitStringOK_get())
	{
		OSTimeDly(1, OS_OPT_TIME_DLY, &err);
		time += 1;
		if (time >= timeout)
		{
			return 0;
		}
	}
	return 1;
}

////////////////////���û�����2
#define WAITAPPENDBUFF_LENGTH 30
uint8_t waitAppendBuff[WAITAPPENDBUFF_LENGTH];
static uint8_t waitAppendBuffLen = 0;  // ���ؼ���
static uint8_t waitAppendBuffStep = 0; // ���ؼ���2
static uint8_t waitAppendOK = 0;	   // status flag
/**
 * @description: ���ô����ջ�������С
 * @param uint8_t strlen ���ճ���
 * @return: none
 */
uint8_t *waitAppend_set(uint8_t strlen)
{
	if (strlen <= WAITAPPENDBUFF_LENGTH)
	{
		waitAppendBuffLen = strlen;
	}
	else
	{
		waitAppendBuffLen = WAITAPPENDBUFF_LENGTH;
	}
	waitAppendOK = 0;
	return waitAppendBuff;
}
/**
 * @description: waitAppendOK set function
 * @param uint8_t value set value
 * @return: none
 */
void waitAppendOK_set(uint8_t value)
{
	waitAppendOK = value;
}
/**
 * @description: ��ȡwaitAppendOK��ֵ
 * @param {type}  none
 * @return: none
 */
uint8_t waitAppendOK_get(void)
{
	return waitAppendOK;
}
/**
 * @description: �ȴ�һ�����ù����������
 * @param {type} 
 * @return: 1 �ɹ���0 ʧ��
 */
uint8_t WaitAppend_OKflag(uint16_t timeout)
{
	OS_ERR err;
	uint16_t time = 0;
	while (!waitAppendOK_get())
	{
		OSTimeDly(1, OS_OPT_TIME_DLY, &err);
		time += 1;
		if (time >= timeout)
		{
			time = 0;
			return 0;
		}
	}
	return 1;
}

/////////////////// UART_TCP_buff �������������
uint8_t UART_TCP_buff[UART_TCP_BUFF_MAXSIZE];  //
volatile uint16_t UART_TCP_buffHead = 0;
volatile uint16_t UART_TCP_buffEnd = 0;
volatile uint8_t UART_TCP_buffOverFlag = 0;
/**
 * @description: ��UART_TCP_buff������д��һ���ַ�
 * @param uint8_t data ��д����ַ�
 * @return: 0 д��ʧ�ܣ�1 д��ɹ�
 */
uint8_t UART_TCP_buffWrite(char data)
{
	if (UART_TCP_buffHead != UART_TCP_buffEnd || UART_TCP_buffOverFlag == 0)
	{
		UART_TCP_buff[UART_TCP_buffEnd] = data;
		UART_TCP_buffEnd++;
		if (UART_TCP_buffEnd >= UART_TCP_BUFF_MAXSIZE)
		{
			UART_TCP_buffEnd = 0;
			UART_TCP_buffOverFlag = 1;
		}
		return 1;
	}
	return 0;
}
/**
 * @description: ��UART_TCP_buff������д��ָ�����ȵ��ַ�
 * @param uint8_t *data �ַ���ַ, uint16_t length �ַ�����
 * @return: n д���ַ����ȣ� 0 ��ʾд��ʧ��
 */
uint8_t UART_TCP_buffWrites(char *data, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		if (UART_TCP_buffWrite(*(data + i)) == 0)
		{
			return 0;
		}
	}
	return i;
}
/**
 * @description: ��UART_TCP_buff����������һ���ַ�
 * @param uint8_t *data ��ȡ���ַ���ŵ�ַ
 * @return: 1 ��ȡ�ɹ���0 ��ȡʧ��
 */
uint8_t UART_TCP_buffRead(char *data)
{
	if (UART_TCP_buffHead != UART_TCP_buffEnd || UART_TCP_buffOverFlag == 1)
	{
		*data = UART_TCP_buff[UART_TCP_buffHead];
		// F407USART1_SendByte(*data);
		UART_TCP_buffHead++;
		if (UART_TCP_buffHead >= UART_TCP_BUFF_MAXSIZE)
		{
			UART_TCP_buffHead = 0;
			UART_TCP_buffOverFlag = 0;
		}
		return 1;
	}
	return 0;
}
/**
 * @description: ��UART_TCP_buff����������ָ�������ַ�
 * @param uint8_t *data ��ȡ�ַ����λ��, uint16_t length ��ȡ�ַ�����
 * @return: n �ɹ���ȡ�ĳ��ȣ� 0 ��ʾ��ȡʧ��
 */
uint16_t UART_TCP_buffReads(char *data, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		if (UART_TCP_buffRead(data + i) == 0)
			return 0;
	}
	return length;
}
/**
 * @description: ��ȡ��UART_TCP_buff����������
 * @param {type} none
 * @return: ����������
 */
uint16_t UART_TCP_buffLength(void)
{
	return UART_TCP_BUFF_MAXSIZE * UART_TCP_buffOverFlag + UART_TCP_buffEnd - UART_TCP_buffHead;
}

/////////////////// UART_TCP_buff �������������
char UART_TCP_buff2[UART_TCP_BUFF_MAXSIZE]; // ��ec25����ȡ����Ч�����ݴ�λ��
/**
 * @description: ѭ����ȡUART3��������ݣ���������д�뵽UART_TCP_buff������
 * @param uint8_t (*getbyte)(uint8_t *) �ص�����ͨ����������ȡָ��λ�õ�����
 * @return: 1 ��ȡ���
 */
uint8_t UART_TCPbuff_Run(uint8_t (*getbyte)(char *))
{
	char data, temp;
	uint8_t test_test[20];
	static uint8_t datalengArry[6];
	static uint8_t datalengArrycount = 0;
	static uint16_t dataleng = 0;
	static uint8_t step = 0;
	static uint16_t countError = 0;
	static uint16_t dataReceiveIndex = 0;
	// extern void F407USART1_SendByte(uint8_t Data);
	while (getbyte(&data) == 1) // �˴����õ���F407USART3_buffRead
	{
		// checkָ���ַ�����ӵ�����
		if (waitAppendOK == 0 && waitStringOK == 1)
		{
			#if (EN_log_print >= 2)
			F407USART1_SendByte(data);
			#endif // EN_log_print			
			waitAppendBuff[waitAppendBuffStep++] = data; 
			if ((waitAppendBuffStep >= waitAppendBuffLen) || (temp == '\r' && data == '\n'))
			{
				waitAppendOK_set(1);
			}
			temp = data;
		}
		else
		{
			waitAppendBuffStep = 0;
		}

		// check ָ���ַ���
		if (waitStringOK == 0 && waitStringBuff[waitStringBuffStep] == data)
		{
			waitStringBuffStep++;
			if (waitStringBuffStep >= waitStringBuffLen)
			{
				waitStringOK_set(1);
			}
		}
		else
		{
			if (waitStringBuff[0] == data)
			{
				waitStringBuffStep = 1;
			}
			else
			{
				waitStringBuffStep = 0;
			}
		}

		// extract valid information
		switch (step)
		{
		// check recv
		case 0:
			if (data == 'r')
				step = 2;
			break;
		case 2:
			if (data == 'e')
				step = 4;
			else if (data != 'r')
				step = 0;
			break;
		case 4:
			if (data == 'c')
				step = 6;
			else if (data == 'r')
				step = 2;
			else
				step = 0;
			break;
		case 6:
			if (data == 'v')
				step = 8;
			else if (data == 'r')
				step = 2;
			else
				step = 0;
			break;
		case 8:
			if (data == '"')
				step = 9;
			else if (data == 'r')
				step = 2;
			else
				step = 0;
			break;
		case 9:
			if (data == ',')
				step = 10;
			else if (data == 'r')
				step = 2;
			else
				step = 0;
			countError = 0;
			break; 
		// it is a very effective way to extract the information.
		// ignore useless characters
		case 10:
			countError++;
			if (countError >= 10)
			{
				step = 0;
				break;
			}
			if (data == ',') // the second comma
			{
				step = 23;
				datalengArrycount = 0;
			}
			break;

		case 23:
			if (data == '\r')
				step = 24;
			else // get the length of the input
			{
				datalengArry[datalengArrycount] = data;
				datalengArrycount++;
				if (datalengArrycount >= 6)
					step = 0;
			}
			break;
		case 24:
			if (data == '\n')
			{
				dataleng = 0;
				dataReceiveIndex = 0;
				for (; datalengArrycount > 0; datalengArrycount--)
				{
					dataleng *= 10;
					dataleng += (datalengArry[dataReceiveIndex] - '0');
					dataReceiveIndex++;
				}
				dataReceiveIndex = 0;
				if (dataleng <= UART_TCP_BUFF_MAXSIZE)
					step = 30;
				else
					step = 0;
			}
			else
				step = 0;
			break;

		case 30:
			UART_TCP_buff2[dataReceiveIndex] = data;
			dataReceiveIndex++;
			if (dataReceiveIndex >= dataleng)
			{
				// printf("ok %d,%d\r\n",dataReceiveIndex,dataleng);
				UART_TCP_buffWrites(UART_TCP_buff2, dataleng);
				step = 0;
			}
			break;
		default:
			break;
		}
	}
	return 1;
}
