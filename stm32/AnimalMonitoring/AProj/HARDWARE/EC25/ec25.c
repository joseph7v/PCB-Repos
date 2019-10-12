#include "ec25.h"  // self
#include "delay.h"	
#include "led.h"     
#include "string.h" 
#include "usart.h" 
#include "usart3.h" 
#include "UART_TCPbuff.h" 
#include "malloc.h"

//////////////////////////////////////////////////////////////////////////////////	   
//********************************************************************************
//�޸�˵��
//1,����__ec25dev�ṹ��
//2,�޸�ec25_send_cmd����,������Ԥ�ڽ������
////////////////////////////////////////////////////////////////////////////////// 	
 
__ec25dev ec25dev;	//ec25������

void ec25_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

	//GPIOD13,GPIOD6,GPIOD5��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��

	GPIO_SetBits(GPIOD,GPIO_Pin_5 );  // GPIOD5(W_DISABLE#) �ߣ��������磻
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);  // GPIOD6(PEREST) �ߣ�������,����reset������һ�������ܵ�·
	GPIO_ResetBits(GPIOD,GPIO_Pin_13);  // GPIOD13(DTR)����Ϊ�ͣ�����ģ��
}

//#define DTR_4G   	PDout(13)	// �͵�ƽ����ģ��
//#define DISABLE_4G  PDout(5)	// �Ƿ�ʹ�÷���ģʽ,�͵�ƽ��Ч
//#define PRESET_4G   PDout(6)	// ����ģ�飬�͵�ƽʱ��>460ms

void ec25_wakeup(int sw)
{
	printf("wakeup_modle\r\n");
	if(sw)  DTR_4G = 0; // �ر�
	else 	DTR_4G = 1; 
}

void ec25_reset(int ms)
{
	printf("reset_modle\r\n");
	PRESET_4G = 1;
	delay_ms(ms);
	PRESET_4G = 0;
}

// ec25���������,�����յ���Ӧ��
// str:�ڴ���Ӧ����
// ����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
//u8* ec25_check_cmd(u8 *str)
//{
//	char *strx=0;
//	if(USART3_RX_STA&0X8000)		//���յ�һ��������
//	{ 
//		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
//		//usart debug
//		printf("*dubug-cmd-rev*%s\r\n",USART3_RX_BUF);
//		
//		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
//	} 
//	return (u8*)strx;
//}

////��ec25��������
////cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
////ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
////waittime:�ȴ�ʱ��(��λ:10ms)
////����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
////       1,�յ���Ԥ�ڽ��
////       2,û�յ��κλظ�
//u8 ec25_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
//{
//	u8 i=0;
//	u8 res=0;  
//	USART3_RX_STA=0;
//	ec25dev.cmdon=1;//����ָ��ȴ�״̬
//	if((u32)cmd<=0XFF)
//	{   
//		while((USART3->SR&0X40)==0);//�ȴ���һ�����ݷ������  
//		USART3->DR=(u32)cmd;
//	}else 
//	{
//		//u3_printf("%s\r\n",cmd);//��������
//		while(cmd[i])
//		{
//			U3_Send_Byte(cmd[i]);
//			i++;
//		}
//		U3_Send_Byte('\r');
//		U3_Send_Byte('\n');
//	}
//	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
//	{
//		while(--waittime)	//�ȴ�����ʱ
//		{
//			delay_ms(10);
//			if(USART3_RX_STA&0X8000)//�Ƿ���յ��ڴ���Ӧ����
//			{
//				if(ec25_check_cmd(ack))res=0;//�յ��ڴ��Ľ����
//				else res=1;//�����ڴ��Ľ��
//				break; 
//			} 
//		}
//		if(waittime==0)res=2; 
//	}
//	return res;
//}

//u8 ec25_send_cmdEX(u8 *cmd,u8 *ack,u16 waittime)
//{
//	u8 i=0;
//	u8 res=0;  
//	USART3_RX_STA=0;
//	ec25dev.cmdon=1;//����ָ��ȴ�״̬
//	if((u32)cmd<=0XFF)
//	{   
//		while((USART3->SR&0X40)==0);//�ȴ���һ�����ݷ������  
//		USART3->DR=(u32)cmd;
//	}else 
//	{
//		while(cmd[i])
//		{
//			U3_Send_Byte(cmd[i]);
//			i++;
//		}
//		U3_Send_Byte('\r');
//		U3_Send_Byte('\n');
//	}
//	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
//	{
//		while(--waittime)	//�ȴ�����ʱ
//		{
//			delay_ms(10);
//			if(USART3_RX_STA&0X8000)//�Ƿ���յ��ڴ���Ӧ����
//			{
//				if(ec25_check_cmd(ack))res=0;//�յ��ڴ��Ľ����
//				else res=1;//�����ڴ��Ľ��
//				break; 
//			} 
//		}
//		if(waittime==0)res=2; 
//	}
//	return res;
//}


////�������ʱ����,��ec25_send_cmd�ɶ�ʹ��/�����ec25_send_cmd�����.
//void ec25_cmd_over(void)
//{
//	USART3_RX_STA=0;
//	ec25dev.cmdon=0;//�˳�ָ��ȴ�״̬
//}

////��1���ַ�ת��Ϊ16��������
////chr:�ַ�,0~9/A~F/a~F
////����ֵ:chr��Ӧ��16������ֵ
//u8 ec25_chr2hex(u8 chr)
//{
//	if(chr>='0'&&chr<='9')return chr-'0';
//	if(chr>='A'&&chr<='F')return (chr-'A'+10);
//	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
//	return 0;
//}

////��1��16��������ת��Ϊ�ַ�
////hex:16��������,0~15;
////����ֵ:�ַ�
//u8 ec25_hex2chr(u8 hex)
//{
//	if(hex<=9)return hex+'0';
//	if(hex>=10&&hex<=15)return (hex-10+'A'); 
//	return '0';
//}


// 
/**
  * @brief  wait for OK flag, unit:ms
  * @param  timeout wait time
  *
  * @retval 
  * 	-<em>1</em> success
  * 	-<em>0</em> fail
 */
u8 WaitString_OKflag(u16 timeout)
{
	static u16 time=0;
	while(!waitStringOK_get())
	{
		delay_ms(1);
		time+=1;
		if(time>=timeout)
		{
			time=0;
			return 0;
		}
	}
	time=0;
	return 1;
}

u8 ec25_init(void)
{
	u8 *p1; 
	u8 cnt;
	u8 i;
	ec25_wakeup(1);   // DRT
	ec25_reset(460);  // reset
	
	
	cnt=0;
	//AT
	for(i=0;i<10;i++)
	{
		waitString_set("OK",2);
		F407USART3_SendString("AT\r\n");
		F407USART1_SendString("AT\r\n");
		if(WaitString_OKflag(2000))  // wait 2s, if success, then break.
			break;
	}  // wait 20s
	
	waitString_set("OK",2);
	F407USART3_SendString("AT+CPIN?\r\n");
	F407USART1_SendString("AT+CPIN?\r\n");
	WaitString_OKflag(3000);
	
	// set APN
	waitString_set("OK",2);
	F407USART3_SendString("AT+QICSGP=1,1,\"HVTV\",\"\",\"\",3\r\n");
	F407USART1_SendString("AT+QICSGP=1,1,\"HCTV\",\"\",\"\",3\r\n");
	WaitString_OKflag(3000);
	
	delay_ms(1000);
	
	// reset
	waitString_set("OK",2);
	F407USART3_SendString("AT+CFUN=0\r\n");
	F407USART1_SendString("AT+CFUN=0\r\n");
	WaitString_OKflag(3000);	
	delay_ms(500);
	waitString_set("OK",2);
	F407USART3_SendString("AT+CFUN=1\r\n");
	F407USART1_SendString("AT+CFUN=1\r\n");
	WaitString_OKflag(3000);
	
	delay_ms(1000);
	
	for(i=0;i<5;i++)
	{
		waitString_set("+CREG: 0,1",10);
		F407USART3_SendString("AT+CREG?\r\n");
		F407USART1_SendString("AT+CREG?\r\n");
		if(WaitString_OKflag(3000))  //
			break;
	}
		
	waitString_set("+CSQ",2);
	F407USART3_SendString("AT+CSQ\r\n");
	F407USART1_SendString("AT+CSQ\r\n");
	WaitString_OKflag(3000);
}

u8 ec25_TCPConnect(u8 *ip,u16 port)
{
	char *ptr = mymalloc(SRAMIN,50);  // ����50���ֽڵ��ڴ�
	
	waitString_set("OK",2);
	//�ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	WaitString_OKflag(3000);
	
	delay_ms(500);
	
	waitString_set("OK:",2);
	F407USART3_SendString("AT+QIDEACT=1\r\n");
	F407USART1_SendString("AT+QIDEACT=1\r\n");
	WaitString_OKflag(3000);
	
	delay_ms(1000);
	
	waitString_set("OK:",2);
	//������
	F407USART3_SendString("AT+QIACT=1\r\n");
	F407USART1_SendString("AT+QIACT=1\r\n");
	WaitString_OKflag(50000);

	delay_ms(1000);
	
	waitString_set("+QIOPEN: 0,0",12);//����ģʽ�ȴ�����Ӧ
	//��tcp����
	sprintf(ptr,"AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,1\r\n",ip,port);
	F407USART3_SendString(ptr);
	F407USART1_SendString(ptr);
	//AT+QIOPEN=1,0,"TCP","47.106.99.188",10020,0,1
	//AT+QIOPEN=1,0,"TCP","112.74.59.250",10002,0,2
	//sprintf(ptr,"%s","AT+QIOPEN=1,0,\"TCP\",\"47.106.99.188\",10020,0,1");
	//sprintf(ptr,"%s",);
	//sprintf(ptr,"AT+QIOPEN=1,0,\"TCP\",\"%s\",%d,0,0",ip,port);
	//F407USART3_SendString("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,1\r\n");
	//F407USART1_SendString("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,1\r\n");
	WaitString_OKflag(10000);
	
	F407USART1_SendString("ec25_TCPConnect\r\n");


	myfree(SRAMIN,ptr);  // �ͷ��ڴ�
	
	return 1;
}

void ec25_TCPDisConnect(void)
{
	waitString_set("OK",2);
	//�ر����磬������ܻ��ʧ��
	F407USART3_SendString("AT+QICLOSE=0\r\n");
	F407USART1_SendString("AT+QICLOSE=0\r\n");
	WaitString_OKflag(3000);
	
	delay_ms(500);
	
	waitString_set("OK:",2);
	F407USART3_SendString("AT+QIDEACT=1\r\n");
	F407USART1_SendString("AT+QIDEACT=1\r\n");
	WaitString_OKflag(3000);
}

u8 ec25_TCPSendData(u8 *data,u16 length)
{
	u8 state=0;
	u16 len=length,index=0;
	#define UART_SEND_PACK_LENGTH_MAX 1500
	while(len>UART_SEND_PACK_LENGTH_MAX)
	{
		state=ec25_TCPSendData1024(data+index,UART_SEND_PACK_LENGTH_MAX);
		if(state==0)
		{
			return 0;
		}
		len-=UART_SEND_PACK_LENGTH_MAX;
		index+=UART_SEND_PACK_LENGTH_MAX;
	}
	if(len>0)
	{
		state=ec25_TCPSendData1024(data+index,len);
		if(state==0)
		{
			return 0;
		}		
	}
	return state;
}

u8 ec25_TCPSendData1024(u8 *data,u16 length)
{
	u8 state;
	char *ptr = mymalloc(SRAMIN,50);  // ����50���ֽڵ��ڴ�
	
	waitString_set("\r\n>",3);
	sprintf(ptr,"AT+QISEND=0,%d\r\n",length);
	F407USART3_SendString(ptr);
	//F407USART1_SendString(ptr);
	myfree(SRAMIN,ptr);  // �ͷ��ڴ�
	//printf("AT+CIPSEND=0,%d\r\n",length);
	state=WaitString_OKflag(5000);
	if(state==0)
	{
		F407USART1_SendString("SIM7100_TCPSendData1024 ERROR-1");
		return 0;
	}		
	//
	waitString_set("SEND OK",7);
	F407USART3_SendBytes(data,length);
	state=WaitString_OKflag(10000);	
	if(state==0)
	{
		F407USART1_SendString("SIM7100_TCPSendData1024 ERROR-2");
		return 0;
	}	
	return 1;
}

void ec25_TCPReadData(u8 *data,u16 length)
{
	// =UART_TCPbuff_Run
}

//u8 ec25_soft_start(void)
//{
//	u8 *p1; 
//	u8 cnt;
//	//DRT
//	ec25_wakeup(1);
//	ec25_reset(460);
//	//reset
//	//AT
//	cnt=0;
//	while(ec25_send_cmd("AT","OK",100))  // 0����Ƿ�Ӧ��ATָ�� 
//	{
//		delay_ms(600);
//		printf("(1/9)wait ec25 response,retrt times:%d\r\n",cnt);
//		delay_ms(400);  
//		if(cnt++>15) return EC25_ERR_AT;
//	} 
//	printf("(1/9)AT? +OK\r\n");	
//	cnt=0;
//	while(ec25_send_cmd("AT+CPIN?","+CPIN: READY",100))  // 1���SIM��
//	{
//		delay_ms(500);
//		printf("(2/9)AT+CPIN? %d\r\n",cnt);
//		delay_ms(500);  
//		if(cnt++>20) 
//			return EC25_ERR_CPIN; 
//	}
//	printf("(2/9)AT+CPIN? +CPIN: READY\r\n");	
//	cnt=0;
//	while(ec25_send_cmd("AT+CREG?","+CREG: 0,1",100))  // 2 CS Service
//	{
//		delay_ms(500);
//		printf("(3/9)AT+CREG? %d\r\n",cnt);
//		delay_ms(500);  
//		if(cnt++>5) return EC25_ERR_CREG; 
//	}

//	if(ec25_send_cmd("AT+QICSGP=1,1,\"hctv\",\"\",\"\",3","OK",500)) {
//		printf("(7/9)!fail connet to hctv\r\n");
//		return EC25_ERR_QICSGP;	
//	}
//	else
//		printf("(7/9)connet to hctv\r\n");
//	delay_ms(500);
//	
//	printf("(3/9)AT+CREG? +CREG: 0,1\r\n");	
//	cnt=0;
//	while(ec25_send_cmd("AT+CGREG?","+CGREG: 0,1",100))  // 3 PS Service
//	{
//		delay_ms(500);
//		printf("(4/9)AT+CGREG? %d\r\n",cnt);
//		delay_ms(500);  
//		if(cnt++>5) return EC25_ERR_CGREG; 
//	}
//	printf("(4/9)AT+CGREG? +CGREG: 0,1\r\n");		
//	cnt=0;
//	while(ec25_send_cmd("AT+CEREG?","+CEREG: 0,1",100))  // 4 PS Service
//	{
//		delay_ms(500);
//		printf("(5/9)AT+CEREG? %d\r\n",cnt);
//		delay_ms(500);  
//		if(cnt++>5) return EC25_ERR_CEREG; 
//	} 
//	printf("(5/9)AT+CEREG? +CEREG: 0,1\r\n");	
//	
//	delay_ms(5000);
//	if(ec25_send_cmd("AT+CSQ","OK",200)==0)//��ѯ�ź�����,˳����GSMģ��״̬
//	{
//		p1=(u8*)strstr((const char*)(USART3_RX_BUF),":"); 
//		p1+=2;
//		ec25dev.csq=(p1[0]-'0')*10+p1[1]-'0';//�ź�����
//		printf("(6/9)AT+CSQ: %d\r\n",ec25dev.csq);
//		//printf("p1:%c,p2:%c,ps:5s\r\n",p1[0],p1[1],USART3_RX_BUF);
//		if(ec25dev.csq>30) 
//			ec25dev.csq=30;		
//		ec25dev.status|=1<<7;	//��ѯGSMģ���Ƿ���λ?
//	}else 
//	{ 
//		ec25dev.csq=0;	
//		ec25dev.status=0;	//���²���
//		printf("(6/9)fail AT+CSQ: %d\r\n",ec25dev.csq);
//		return EC25_ERR_CSQ;
//	}


//	// Activate a PDP Context
//	while(ec25_send_cmd("AT+CFUN=0","OK",100))  // 4 PS Service
//	{
//		delay_ms(500);
//		printf("AT+CFUN=0");
//		delay_ms(500);  
//		if(cnt++>5) return EC25_ERR_CEREG; 
//	} 
//	while(ec25_send_cmd("AT+CFUN=1","OK",100))  // 4 PS Service
//	{
//		delay_ms(500);
//		printf("AT+CFUN=1");
//		delay_ms(500);  
//		if(cnt++>5) return EC25_ERR_CEREG; 
//	} 
//	if(ec25_send_cmd("AT+QIACT=1","OK",500)) {
//		printf("(8/9)!fail Activate a PDP Context\r\n");
//		return EC25_ERR_QICSGP;	
//	}
//	else
//		printf("(8/9)Activate a PDP Context\r\n");
//	delay_ms(500);
//	
//	if(ec25_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"47.106.99.188\",10808,0,2","CONNECT",1000)) return EC25_ERR_QIOPEN;
//	//if(ec25_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"210.77.68.221\",10020,0,2","CONNECT",1000)) return EC25_ERR_QIOPEN;
//	
//	// tongxunmao TCP:112.74.59.250:10002
//	//if(ec25_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"112.74.59.250\",10002,0,2","CONNECT",1000)){  //beijing 

//	
////	delay_ms(2000);
////	U3_Send_Date("123456789\r\n",12);
////	delay_ms(2000);
////	U3_Send_Date("123456789\r\n",12);
////	delay_ms(2000);
////	U3_Send_Date("123456789\r\n",12);
////	delay_ms(2000);
////	U3_Send_Date("123456789\r\n",12);
////	delay_ms(2000);
////	U3_Send_Date("123456789\r\n",12);
//	
////	printf("t\r\n");
//	return 0;
//}

//u8 ec25_tcp_transparent(void)
//{
//	if(ec25_send_cmd("AT","OK",100))  // if can't reveive response, it may in transparent mode.
//		ec25_send_cmdEX("+++","OK",200);  
//	
//	if(ec25_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"47.106.99.188\",10808,0,2","CONNECT",1000)) return EC25_ERR_QIOPEN;
//}

//u8 ec25_dec(void)
//{
//	printf("ec25_dec\r\n");
//	delay_ms(10);
//	//if(ec25_send_cmdEX("+++","OK",1000)) return EC25_ERR_DEACT;	
//	if(ec25_send_cmd("AT","OK",100))  // if can't reveive response, it may in transparent mode.
//		ec25_send_cmdEX("+++","OK",200);  
//	if(ec25_send_cmd("AT+QICLOSE=0","OK",500)) return EC25_ERR_DEACT;
//	if(ec25_send_cmd("AT+QIDEACT=1","OK",500)) return EC25_ERR_DEACT;
//	return 0;
//}

//ec25״̬���
//void ec25_status_check(void)
//{
//	u8 *p1; 
//	if(ec25dev.cmdon==0&&ec25dev.mode==0&&USART3_RX_STA==0)//��ָ��ȴ�״̬.����/����ģʽ/��û���յ��κ�����,�������ѯ
//	{
//		if(ec25_send_cmd("AT+CSQ","OK",25)==0)//��ѯ�ź�����,˳����GSMģ��״̬
//		{
//			p1=(u8*)strstr((const char*)(USART3_RX_BUF),":"); 
//			p1+=2;
//			ec25dev.csq=(p1[0]-'0')*10+p1[1]-'0';//�ź�����
//			if(ec25dev.csq>30)ec25dev.csq=30;		
//			ec25dev.status|=1<<7;	//��ѯGSMģ���Ƿ���λ?
//		}else 
//		{ 
//			ec25dev.csq=0;	
//			ec25dev.status=0;	//���²���
//		} 
//		if((ec25dev.status&0XC0)==0X80)//CPIN״̬,δ��ȡ?
//		{ 
//			ec25_send_cmd("ATE0","OK",100);//������(����ر�,����������ݿ����쳣)
//			if(ec25_send_cmd("AT+CPIN?","OK",25)==0)ec25dev.status|=1<<6;//SIM����λ
//			else ec25dev.status&=~(1<<6);//SIM������ 
//		} 
//		if((ec25dev.status&0XE0)==0XC0)//��Ӫ������,δ��ȡ?
//		{ 
//			if(ec25_send_cmd("AT+COPS?","OK",25)==0)//��ѯ��Ӫ������
//			{ 
//				p1=(u8*)strstr((const char*)(USART3_RX_BUF),"MOBILE");//����MOBILE,�����ǲ����й��ƶ�?
//				if(p1)ec25dev.status&=~(1<<4); //�й��ƶ� 
//				else 
//				{
//					p1=(u8*)strstr((const char*)(USART3_RX_BUF),"UNICOM");//����UNICOM,�����ǲ����й���ͨ?
//					if(p1)ec25dev.status|=1<<4;	//�й���ͨ 
//				}
//				if(p1)
//				{
//					ec25dev.status|=1<<5;	//�õ���Ӫ�������� 
//					//phone����ͨ������
//					ec25_send_cmd("AT+CLIP=1","OK",100);	//����������ʾ 
//					ec25_send_cmd("AT+COLP=1","OK",100);	//���ñ��к�����ʾ
//					//sms����ͨ������
//					ec25_send_cmd("AT+CMGF=1","OK",100);			//�����ı�ģʽ 
//					ec25_send_cmd("AT+CSCS=\"UCS2\"","OK",100);	//����TE�ַ���ΪUCS2 
//					ec25_send_cmd("AT+CSMP=17,0,2,25","OK",100);	//���ö���Ϣ�ı�ģʽ���� 
//				}
//			}else ec25dev.status&=~(1<<5);	//δ�����Ӫ������
//		}
//		ec25_cmd_over();//�������
//	}
//}

