#include "ec25.h"
#include "usart3.h"
#include "UART_TCPbuff.h"
#include "MyFunction_C.h"
// foundmental
#include "stdio.h"
#include "power.h"
#include "usart.h"
#include "delay.h"

/**
 * @description: ec25_Init
 * @param {type} 
 * @return: 
 */
EC25_ERR ec25_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // ʹ��GPIODʱ��

	// GPIOD13,GPIOD6,GPIOD5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  // ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);			   // ��ʼ��

	GPIO_SetBits(GPIOD, GPIO_Pin_7);	// GPIOD7(W_DISABLE#) �ߣ��������磻
	GPIO_ResetBits(GPIOD, GPIO_Pin_14); // GPIOD6(PEREST) �ߣ�������,����reset������һ�������ܵ�·
	GPIO_ResetBits(GPIOD, GPIO_Pin_13); // GPIOD13(DTR) ����Ϊ�ͣ�����ģ��

	F407USART3_Init(921600); // config & fixed
	POWER_4G = 1;			 // ��4G��Դ
#if (EN_log_print >= 3)
	F407USART1_SendString("POWER_4G,ec25_Init\r\n");
#endif
	delay_ms(1000);
	return ec25_tcp_Conditon_Normal();
}
/**
 * @description: Ӳ������ģ��
 * @param ms a low level voltage with the frame of150~460ms, 300ms recommand
 * @return: 
 */
void ec25_reset(int ms)
{
#if (EN_log_print >= 3)
	F407USART1_SendString("reset_modle\r\n");
#endif
	EC25_PERST = 1;
	delay_ms(ms);
	EC25_PERST = 0;
}
/**
 * @description: ec25����
 * @param {type} 
 * @return: 
 */
void ec25_DTR_Sleep(void)
{
	waitString_set("OK", 2);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	WaitString_OKflag(5000);

	// Deactivate a PDP Context
	waitString_set("OK:", 2);
	F407USART3_SendString("AT+QIDEACT=1\r\n");
	F407USART1_SendString("AT+QIDEACT=1\r\n");
	WaitString_OKflag(5000);

	EC24_DTR = 1;
}
/**
 * @description: Ӳ������EC25
 * wake up the module
 * @param {type} 
 * @return: 
 */
void ec25_DTR_Wakeup(void)
{
	EC24_DTR = 0; // active wake up
}
/**
 * @description: ģ��ػ�
 * turn off the module
 * @param {type} 
 * @return: 
 */
void ec25_Turn_off(void)
{
	waitString_set("POWERED DOWN", 12);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QPOWD\r\n");
#if (EN_log_print >= 1)
	F407USART1_SendString("AT+QPOWD\r\n");
#endif
	WaitString_OKflag(5000);
	delay_ms(1000);
	POWER_4G = 0; // turn off the power
	delay_ms(1000);
}
/**
 * @description: ec25����
 * @param {type} none
 * @return: none
 */
void ec25_Restart(void)
{
	ec25_Turn_off();
	POWER_4G = 0; // ��4G��Դ
	delay_ms(1000);
	ec25_Init();
}
/**
 * @description: EC25������ʽ��������
 * @param {type} 
 * @return: 
 */
EC25_ERR ec25_tcp_Conditon_Normal(void)
//void ec25_Init(void)
{
	uint8_t i;
	// Power on the module, already done
	ec25_reset(460); // Reboot the module
	// �ȴ���Դ�ȶ������Խ��յ�ATָ��

	for (i = 0; i < 30; i++)
	{
		waitString_set("OK", 2);
		F407USART3_SendString("AT\r\n");
		F407USART1_SendString("AT\r\n");
		if (WaitString_OKflag(2000))
			break;
	} // maxium wait 20s
	// config uart
	waitString_set("OK", 2);
	F407USART3_SendString("AT+IFC=2,2\r\n");
	F407USART1_SendString("AT+IFC=2,2\r\n");
	WaitString_OKflag(3000);

	return ec25_Act_PDP_Condition();
}
/**
 * @description: act PDP Condition
 * @param {type} none
 * @return: EC25_ERR
 */
EC25_ERR ec25_Act_PDP_Condition(void)
{
	uint8_t i, j, res;
	/* Use AT+CPIN?/AT+CREG?/AT+CGREG? to query the SIM status and network registration status */
	// Query the SIM status
	i = 1;
	while (i++)
	{
		waitString_set("+CPIN: READY", 12);
		F407USART3_SendString("AT+CPIN?\r\n");
		F407USART1_SendString("AT+CPIN?\r\n");
		res = WaitString_OKflag(2000);
		if (res)
		{
			printf("Find (U)SIM card\r\n");
			break;
		}
		else if (i > 10)
		{
			printf("failed to identify (U)SIM card\r\n");
			return EC25_ERR_CPIN;
		} // maxium wait 20s
	}
	// Query CS service
	i = 1;
	while (i++)
	{
		waitString_set("+CREG: 0,1", 10);
		F407USART3_SendString("AT+CREG?\r\n");
		F407USART1_SendString("AT+CREG?\r\n");
		res = WaitString_OKflag(2000);
		if (res)
		{
			printf("Register on CS domain service\r\n");
			break;
		}
		else if (i > 20)
		{
			printf("Failed to register on CS domain service\r\n");
			return EC25_ERR_CREG;
		} // maxium wait 40s
	}
	// Query PS service
	i = 1;
	while (i++)
	{
		waitString_set("+CGREG: 0,1", 10);
		F407USART3_SendString("AT+CGREG?\r\n");
		F407USART1_SendString("AT+CGREG?\r\n");
		res = WaitString_OKflag(2000);
		if (res)
		{
			printf("Register on PS domain service\r\n");
			break;
		}
		else if (i > 20)
		{
			printf("Failed to register on PS domain service\r\n");
			return EC25_ERR_CGREG;
		} // maxium wait 40s
	}

#if In_Condition_HCTV == 0
	/* Use AT+QICSGP=1,1,"HCTV","","",3 to set APN as "HCTV",user name as "",password as ""*/
	//set APN
	waitString_set("OK", 2);
	F407USART3_SendString("AT+QICSGP=1,1,\"HCTV\",\"\",\"\",3\r\n");
	F407USART1_SendString("AT+QICSGP=1,1,\"HCTV\",\"\",\"\",3\r\n");
	// F407USART3_SendString("AT+QICSGP=1,1,\"HCTV\",\"\",\"\",0\r\n");
	// F407USART1_SendString("AT+QICSGP=1,1,\"HCTV\",\"\",\"\",0\r\n");
	WaitString_OKflag(3000);
	delay_ms(1000);
	// soft reset network
	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=0\r\n");
	F407USART1_SendString("AT+CFUN=0\r\n");
	WaitString_OKflag(3000);
	delay_ms(500);
	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=1\r\n");
	F407USART1_SendString("AT+CFUN=1\r\n");
	WaitString_OKflag(3000);
	delay_ms(1000);
#endif //In_Condition_HCTV 

	// act PDP����
	j = 3;
	while(j--)	
	{
		waitString_set("OK", 2);
		F407USART3_SendString("AT+QIACT=1\r\n");
		F407USART1_SendString("AT+QIACT=1\r\n");
		res = WaitString_OKflag(5000);
		if (!res)
		{
			printf("Fail:AT+QIACT\r\n");
			//waitString_set("OK:", 2);
			//F407USART3_SendString("AT+QIDEACT=1\r\n");
			//F407USART1_SendString("AT+QIDEACT=1\r\n");
			//res = WaitString_OKflag(5000);
			//break;
		}			
		// Query PDP condition
		i = 5; // maxium wait 20s
		while (i--)
		{
			waitString_set("+QIACT", 6);
			F407USART3_SendString("AT+QIACT?\r\n");
			F407USART1_SendString("AT+QIACT?\r\n");
			res = WaitString_OKflag(2000);
			if (res)
			{
				printf("Success:AT+QIACT\r\n");
				return EC25_ERR_NONE;
			}
		}
		printf("Failed to register on PS domain service\r\n");
	}
	return EC25_ERR_QIACT;
}
/**
 * @description: ec25 Open Connection
 * @param uint8_t *ip_port ����IP��PORT��ָ�
 * @return: 0 �ɹ�, ���� EC25_ERR ʧ��
 * @note: ǰ����PDP�����Ѿ���
 */
EC25_ERR ec25_TCPConnect(char *ip, uint16_t port)
{
	uint8_t i, res;
	char ptr[50];
	// close Network
	i = 1;
	sprintf(ptr, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n", ip, port);
	while (i++)
	{
		waitString_set("OK", 2);
		// �ر����磬������ܻ��ʧ��
		F407USART3_SendString("AT+QICLOSE=0\r\n");
		F407USART1_SendString("AT+QICLOSE=0\r\n");
		res = WaitString_OKflag(2000);
		if (res)
		{
			printf("OK\r\n");
			break;
		}
		else if (i > 20)
		{
			printf("Fail:AT+QICLOSE\r\n");
			return EC25_ERR_QICLOSE;
		} // maxium wait 40s
	}
	// Establish Network
	i = 1;
	while (i++)
	{
		waitString_set("+QIOPEN: 0,0", 12);
		F407USART3_SendString(ptr);
		F407USART1_SendString(ptr);
		WaitString_OKflag(5000);
		delay_ms(500);
		// Querry QISTATE
		waitString_set("+QISTATE", 8);
		F407USART3_SendString("AT+QISTATE?\r\n");
		F407USART1_SendString("AT+QISTATE?\r\n");
		res = WaitString_OKflag(2000);
		if (res)
		{
			F407USART1_SendString("ec25_TCP_Connected\r\n");
			break;
		}
		else if (i > 5)
		{
			F407USART1_SendString("Fail:ec25_TCP_Connect\r\n");
			return EC25_ERR_QIOPEN;
		} // maxium wait 40s
	}
	return EC25_ERR_NONE;
}
/**
 * @description: �Ͽ�TCP����
 * @param {type} none
 * @return: 
 */
uint8_t ec25_TCPDisConnect(void)
{
	waitString_set("OK", 2);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	return WaitString_OKflag(5000);
}
#if 0
/**
 * @description: TCP Connect
 * @param {type} 
 * @return: 
 * @note: refer "Quectel EC2x TCP(IP) AT Commands" page 8
 */
	uint8_t SIM7100_TCPConnect(uint8_t *ip, uint16_t port)
{
	OS_ERR err;
	uint8_t res;
	uint8_t i;
	char ptr[50] = {0};

	waitString_set("OK", 2);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QICLOSE\r\n");
	delay_ms(1000);

	waitString_set("OK:", 2);
	F407USART3_SendString("AT+QIDEACT=1\r\n");
	F407USART1_SendString("AT+QIDEACT=1\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QIDEACT\r\n");
	delay_ms(1000);

	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=0\r\n");
	F407USART1_SendString("AT+CFUN=0\r\n");
	res = WaitString_OKflag(3000);
	if (!res)
		printf("Fail:AT+CFUN=0\r\n");
	delay_ms(500);
	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=1\r\n");
	F407USART1_SendString("AT+CFUN=1\r\n");
	res = WaitString_OKflag(3000);
	if (!res)
		printf("Fail:AT+CFUN=0\r\n");

	for (i = 0; i < 10; i++)
	{
		waitString_set("+CREG: 0,1", 10);
		F407USART3_SendString("AT+CREG?\r\n");
		F407USART1_SendString("AT+CREG?\r\n");
		if (WaitString_OKflag(3000))
			break;
		else
		{
			printf("Fail:AT+CREG,i=%d\r\n", i);
		}
	}

	waitString_set("+CSQ", 2);
	F407USART3_SendString("AT+CSQ\r\n");
	F407USART1_SendString("AT+CSQ\r\n");
	WaitString_OKflag(3000);

	waitString_set("OK:", 2);
	// ������
	F407USART3_SendString("AT+QIACT=1\r\n");
	F407USART1_SendString("AT+QIACT=1\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QIACT\r\n");

	delay_ms(1000);

	waitString_set("+QIOPEN: 0,0", 12); //����ģʽ�ȴ�����Ӧ
	//��tcp����
	sprintf(ptr, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n", ip, port);
	//F407USART1_SendString("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,1\r\n");
	F407USART3_SendString(ptr);
#if (EN_log_print >= 1)
	F407USART1_SendString(ptr);
#endif // EN_log_print
	res = WaitString_OKflag(10000);
	if (res)
	{
		F407USART1_SendString("ec25_TCP_Connected\r\n");
	}
	else
	{
		F407USART1_SendString("Fail:ec25_TCP_Connect\r\n");
	}
	return res;
}

uint8_t SIM7100_TCPConnect(uint8_t *ip, uint16_t port)
{
	OS_ERR err;
	uint8_t res;
	uint8_t i;
	char ptr[50];

	waitString_set("OK", 2);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QICLOSE\r\n");
	delay_ms(1000);

	waitString_set("OK:", 2);
	F407USART3_SendString("AT+QIDEACT=1\r\n");
	F407USART1_SendString("AT+QIDEACT=1\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QIDEACT\r\n");
	delay_ms(1000);

	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=0\r\n");
	F407USART1_SendString("AT+CFUN=0\r\n");
	res = WaitString_OKflag(3000);
	if (!res)
		printf("Fail:AT+CFUN=0\r\n");
	delay_ms(500);
	waitString_set("OK", 2);
	F407USART3_SendString("AT+CFUN=1\r\n");
	F407USART1_SendString("AT+CFUN=1\r\n");
	res = WaitString_OKflag(3000);
	if (!res)
		printf("Fail:AT+CFUN=0\r\n");

	for (i = 0; i < 10; i++)
	{
		waitString_set("+CREG: 0,1", 10);
		F407USART3_SendString("AT+CREG?\r\n");
		F407USART1_SendString("AT+CREG?\r\n");
		if (WaitString_OKflag(3000)) 
			break;
		else
		{
			printf("Fail:AT+CREG,i=%d\r\n",i);
		}
		
	}

	waitString_set("+CSQ", 2);
	F407USART3_SendString("AT+CSQ\r\n");
	F407USART1_SendString("AT+CSQ\r\n");
	WaitString_OKflag(3000);

	waitString_set("OK:", 2);
	// ������
	F407USART3_SendString("AT+QIACT=1\r\n");
	F407USART1_SendString("AT+QIACT=1\r\n");
	res = WaitString_OKflag(5000);
	if (!res)
		printf("Fail:AT+QIACT\r\n");

	delay_ms(1000);

	waitString_set("+QIOPEN: 0,0", 12); //����ģʽ�ȴ�����Ӧ
	//��tcp����
	sprintf(ptr, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n", ip, port);
	//F407USART1_SendString("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,1\r\n");
	F407USART3_SendString(ptr);
#if (EN_log_print >= 1)
	F407USART1_SendString(ptr);
#endif // EN_log_print
	res = WaitString_OKflag(10000);
	if (res)
	{
		F407USART1_SendString("ec25_TCP_Connected\r\n");
	}
	else
	{
		F407USART1_SendString("Fail:ec25_TCP_Connect\r\n");
	}
	return res;
}

/**
 * @description: ����ʧ�ܺ����½�������
 * @param {type} 
 * @return: 
 */
uint8_t ec25_TCPReconnect(uint8_t *ip, uint16_t port)
{
	OS_ERR err;
	uint8_t res;
	uint8_t i;
	char ptr[50] = {0};

	waitString_set("OK", 2);
	// �ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	res = WaitString_OKflag(10000);
	if (!res)
	{
		printf("Fail:AT+QICLOSE\r\n");
		return EC25_ERR_QICLOSE;
	}

	delay_ms(1000);

	// F407USART1_SendString("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,1\r\n");
	sprintf(ptr, "AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n", ip, port);
	// ��������5��
	for (i = 0; i < 5; i++)
	{
		waitString_set("+QIOPEN: 0,0", 12); // ����ģʽ�ȴ�����Ӧ
		// ��tcp����
		F407USART3_SendString(ptr);
#if (EN_log_print >= 1)
		F407USART1_SendString(ptr);
#endif // EN_log_print
		res = WaitString_OKflag(10000);
		if (res)
		{
			F407USART1_SendString("ec25_TCP_Connected\r\n");
			return EC25_ERR_NONE;
		}
	}
	F407USART1_SendString("Fail:ec25_TCP_Connect,EC25_ERR_QIOPEN\r\n");
	return EC25_ERR_QIOPEN;
}
#endif // test

/**
 * @description: ����һ������
 * @param uint8_t *data ������ͼƬ��ַ, uint16_t length ������ͼƬ����
 * @return: 
 */
uint32_t ec25_TCPSendData(char *data, uint16_t length)
{
	uint8_t res = 0;
	uint16_t len = length, index = 0;

	while (len > UART_SEND_PACK_LENGTH_MAX)
	{
		res = ec25_TCPSendDataPack(data + index, UART_SEND_PACK_LENGTH_MAX);
		if (res == 0)
		{
			printf("Fail:ec25_TCPSendDataPack-3\r\n");
			return 0;
		}
		len -= UART_SEND_PACK_LENGTH_MAX;
		index += UART_SEND_PACK_LENGTH_MAX;
	}
	if (len > 0)
	{
		res = ec25_TCPSendDataPack(data + index, len);
		if (res == 0)
		{
			printf("Fail:ec25_TCPSendDataPack-4\r\n");
			return 0;
		}
		index += len;
	}
	return index;
}
/**
 * @description: ����һ֡����<=1460
 * @param uint8_t *data ������ͼƬ��ַ, uint16_t length ������ͼƬ����
 * @return: �ɹ����ͳ���
 */
uint32_t stringtoNum(char* str);
uint16_t ec25_TCPSendDataPack(char *data, uint16_t length)
{
	uint8_t *buff;
	uint8_t res,offset,i;
	uint32_t unackedbytes;
	static char myBUF50[30];
	// check cache
	i=1;
	while(i++)
	{
		if(i>30)
		{
			F407USART1_SendString("\r\n\r\nplease check send status!\r\n\r\n");
			delay_ms(1000);
			return 0;
		}
		
		waitString_set("+QISEND: ", 9);
		buff = waitAppend_set(30);
		F407USART3_SendString("AT+QISEND=0,0\r\n");
		res = WaitAppend_OKflag(10000);
		if (res == 0)
		{
			F407USART1_SendString("Fail:QISEND Queey Data, ec25_TCPSendDataPack-2��");
			delay_ms(100);
			continue;
		}
		offset = locate_character((char *)buff, ',');
		offset += locate_character((char *)(buff+offset+1), ',');  // Ѱ�ҵڶ�������
		unackedbytes = stringtoNum((char *)(buff+offset+2));
		
		printf("unackedbytes=%d\r\n",unackedbytes);
		if(unackedbytes<5120)	
		{
			break;
		}
		else
		{
			printf(".wait.\r\n");
		}
	
		delay_ms(100);		
	}
	// send header
	waitString_set(">", 1);
	sprintf(myBUF50, "AT+QISEND=0,%d\r\n", length);
	F407USART3_SendString(myBUF50);
#if (EN_log_print >= 3)
	F407USART1_SendString(myBUF50);
#endif // EN_log_print
	res = WaitString_OKflag(5000);
	if (res == 0)
	{
		F407USART1_SendString("Fail:AT+QISEND=0, ec25_TCPSendDataPack-1");
		return 0;
	}
	// send data
	// reveive SEND OK|SEND FAIL|ERROR	
	waitString_set("SEND ", 5);
	buff = waitAppend_set(2);
	F407USART3_SendBytes(data, length);
	res = WaitAppend_OKflag(10000);
	if (res == 0)
	{
		F407USART1_SendString("Fail:QISEND Data, ec25_TCPSendDataPack-2��");
		return 0;
	}
	if (buff[0]=='F' && buff[1]=='A') // FAIL
	{
		F407USART1_SendString("\r\n\r\nplease check send status!\r\n\r\n");
		delay_ms(1000);
		return 0;
	}	
	return length;
}
