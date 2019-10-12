/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "BC95.h"
#include "I2C_OLED.h"
#include "usart.h"
#include "string.h"
#include "algorithm.h"
#include "timer.h"
#include "common.h"
#include "FreeRTOS.h"
#include "task.h"

////////////////////////////////////////////////////////////
//�������ݺ���״̬
//������ݵ�status�Ǵ���ģ���ô�洢����Ϣ�п������ϴεģ���Ϊû������
//���������״̬
//CMD_GOT_NUM_0 1//û������
//CMD_GOT_TOO_LONG 2//û������
//CMD_GOT_FORMAT_ERROR 3 //���ݸ�ʽ����
//CMD_GOT_OK 4 //���ݽ��ճɹ�
CMD_GOT cmdgot = {0,0,0,0};



///////////////////////////////////////////////////////////
//**************************************************************************************
//�������ƣ�void AttachNetwork(void)
//�����������
//�������ã������������ã������������
//���ز�������
//****************************************************************************************
u8 AttachNetwork(void)
{
	u8 try_num = API_OPEN_REPEAT_NUMBER;
	flag.BRD_STA = BRD_ATTACKNETWORKING;//�������ڳ�������
	while(try_num) 
	{
		if(AT_NRB()==0)//����������ɹ�
			goto ATTACKERROR;
		if(AT_Test()==0)//���ʣ������Ѿ�����0,
			goto ATTACKERROR;
		if(CDP_Get_Cfg(CDP_SERVER_IP_PORT)==0)
			goto ATTACKERROR;
		if(CFUN_Get_Set()==0)
			goto ATTACKERROR;
		if(NBAND_Get_Set(TELECHINA_NBAND)==0)
			goto ATTACKERROR;
		if(CGATT_Get_Set()==0)
			goto ATTACKERROR;
		flag.BRD_STA = BRD_ATTACKED;//�ɹ���������
		BrdStaDisp();//���°��ӵ�״̬
		break;
		ATTACKERROR:
		try_num--;
	}
	return try_num;
}
//*********************************************************************************
//�������ƣ�StartWaitMsg
//���������
//�������ã���ʼ�ȴ�ͨ���ź�.�����ȴ���ʼ��־λ�򿪣��ȴ�ʱ��������㣬����λ������
//���ز�����
//*********************************************************************************
void StartWaitMsg(void)
{
	taskENTER_CRITICAL();
	usart.WAIT_START = 1;//��ʼ�ȴ�
	usart.waittime = 0;
	usart.num = 0;
	taskEXIT_CRITICAL();
}
//*******************************************************
//�������ƣ�StopWaitMsg
//���������
//�������ã�ֹͣ�ȴ�ͨ���ź�.�����ȴ���ʼ��־λ�򿪣��ȴ�ʱ��������㣬����λ������
//���ز�����
//*******************************************************
void StopWaitMsg(void)
{
	taskENTER_CRITICAL();
	usart.WAIT_START = 0;//ֹͣ�ȴ�
	usart.waittime = 0;
	
	taskEXIT_CRITICAL();
}
//*******************************************************
//�������ƣ�AT_NRB
//���������
//�������ã�BC95����reboot
//���ز�����1
//*******************************************************
u8 AT_NRB(void)
{
	u8 try_num = AT_NRB_REPEAT_NUMBER;//��������
	char cmd[10] = "AT+NRB\r\n";
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("REBOOT...");	
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_NRB_WAIT_TIME)//����5��
		{
			if(usart.num>50)
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//��λ�źűȽ϶࣬�ȴ�ʱ�䳤һ��
				//���Ҫ��ǰ�棬��Ϊ��֪��NRB�յ�ʲô����
				//��Ҫ�ȴ�ȫ��������ϣ����ܹرյȴ�
				//��ʵ����Ҳ����Ҫ����Ϊÿ�ν���ǰ��Ҫ��ʼ��numΪ0
				vTaskDelay(2*NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�AT_TEST
//���������
//�������ã�����ATͨ���Ƿ�����
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 AT_Test(void)
{
	u8 try_num = AT_AT_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	char cmd[6] = "AT\r\n";
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("AT TEST...");
		
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
//		printf("AT\r\n");
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�CFUN_Get_Cfg
//���������
//�������ã�����CDP
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CDP_Get_Cfg(char * ip_port)
{
	u8 try_num = AT_NCDP_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	//������ַ�����ֵʱ��һ��Ҫ������Ĵ�С
	char cmd[15] = "AT+NCDP?\r\n";
	
	char rcv[40] = "\r\n+NCDP:";
	strcat(rcv,ip_port);
	strcat(rcv,"\r\n\r\nOK\r\n");

	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("CFUN GetSet...");
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
//		printf("AT+NCDP=180.101.147.115,5683\r\n");
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp(rcv,usart.USART_RX_BUF))
//			if(usart.num!=0&&StrCmp("\r\n+NCDP:180.101.147.115,5683\r\n\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//�¸�ATָ��ǰ�ȴ�һ��ʱ��
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			//��ʾ�ȴ�ʱ��
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		CDP_Cfg(ip_port);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}
//*******************************************************
//�������ƣ�AT_TEST
//���������CDP��ַ
//�������ã�CDP��ַ����
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CDP_Cfg(char * ip_port)
{
	u8 try_num = AT_NCDP_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	//������ַ�����ֵʱ��һ��Ҫ������Ĵ�С
	char cmd[64] = "AT+NCDP=";
	strcat(cmd,ip_port);
	strcat(cmd,"\r\n");
	
	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("CDP Cfg...");
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
//		printf("AT+NCDP=180.101.147.115,5683\r\n");
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//�¸�ATָ��ǰ�ȴ�һ��ʱ��
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
//				return 1;
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}
//*******************************************************
//�������ƣ�CFUN_Get_Set
//���������
//�������ã�����cfun����
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CFUN_Get_Set(void)
{
	u8 try_num = AT_CFUN_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
//	u16 time_temp=0;
	//������ַ�����ֵʱ��һ��Ҫ������Ĵ�С
	char cmd[15] = "AT+CFUN?\r\n";
	
	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("CFUN GetSet...");
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp("\r\n+CFUN:1\r\n\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//�¸�ATָ��ǰ�ȴ�һ��ʱ��
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		CFUN_Set();
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�CFUN_Set
//���������
//�������ã�CFUN������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CFUN_Set(void)
{
	u8 try_num = AT_CFUN_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	char cmd[15] = "AT+CFUN=1\r\n";
	
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("CFUN SET...");		
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//CGATTĿǰ���Ǻ��ȶ����ȴ�ʱ�䳤һ�㣬����Ĭ�ϵ�AT�ȴ�����
				vTaskDelay(2*NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}
//*******************************************************
//�������ƣ�NBAND_Get_Set
//������������ŵ�5,8,20
//�������ã�����NBAND����
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 NBAND_Get_Set(char * nband)
{
	u8 try_num = AT_NBAND_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
//	u16 time_temp=0;
	//������ַ�����ֵʱ��һ��Ҫ������Ĵ�С
	char cmd[20] = "AT+NBAND?\r\n";
	
	char rcv[25] = "\r\n+NBAND:";
	strcat(rcv,nband);
	strcat(rcv,"\r\n\r\nOK\r\n");
	
	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("NBAND GetSet...");
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		vTaskDelay(NEXT_AT_WAIT_TIME);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			//�̶�Ϊ����ʱ��һ���������ġ�
//			if(usart.num!=0&&StrCmp("\r\n+NBAND:5\r\n\r\nOK\r\n",usart.USART_RX_BUF))
			//���̶�ƽ̨����ͨ���ú�������5��8��20
			if(usart.num!=0&&StrCmp(rcv,usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//�¸�ATָ��ǰ�ȴ�һ��ʱ��
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		NBAND_Set(nband);//����NBAND=5
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�NBAND_Set
//���������nband���ã�����Ϊ5����ͨΪ8
//�������ã�nband������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 NBAND_Set(char * nband)
{
	u8 try_num = AT_NBAND_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	char cmd[15] = "AT+NBAND=";
	strcat(cmd,nband);
	strcat(cmd,"\r\n");
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("NBAND Set...   ");
		
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_CGATT_WAIT_TIME)//����3��
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�CGATT_Get_Set
//���������
//�������ã���������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CGATT_Get_Set(void)
{
	u8 try_num = AT_CGATT_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	//������ַ�����ֵʱ��һ��Ҫ������Ĵ�С
	char cmd[15] = "AT+CGATT?\r\n";
	CGATT_Set();//�����磬CGATT = 1

	
	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("CGATT GetSet...");
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_CGATT_WAIT_TIME)//����3��
		{
			if(usart.num!=0&&StrCmp("\r\n+CGATT:1\r\n\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				//�¸�ATָ��ǰ�ȴ�һ��ʱ��
				vTaskDelay(NEXT_AT_WAIT_TIME);

				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			

		}
		sgnDisp("- OVERTIME -");		
		vTaskDelay(NEXT_AT_WAIT_TIME);
		CGATT_Set();
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�CGATT_Set
//���������
//�������ã�CFUN������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CGATT_Set(void)
{
	u8 try_num = AT_CGATT_REPEAT_NUMBER;//AT���ͺ󣬷���ERROR�����ԵĴ���
	char cmd[15] = "AT+CGATT=1\r\n";
	
	while(try_num)
	{	
		//��ʼ������
		InitDisp();
		cmdDisp("CGATT SET...");	
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_DEFAULT_WAIT_TIME)//��������
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				vTaskDelay(NEXT_AT_WAIT_TIME);
				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}
//*******************************************************
//�������ƣ�MSG_Send
//������������ݵ�hexstr��ʽ�������ݳ�������hexΪһ������
//��AΪ1�����ݣ�hex��ʽΪ43
//�������ã���������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 intMSG_Send(u16 num,u8 len)
{
	u8 try_num = AT_NMGS_REPEAT_NUMBER;//��������
	char hexstr[DATA_MAXNUM] = {0,0,0};
	char numlenstr[3] = {0,0,0};//���ݵĳ��ȣ���ʮ�����ַ�����ʾ��8λʮ����������ת��Ϊ"8"
	Dnum2Str(numlenstr,len,1);//�������ó���1λʮ����������Ҫ�Ż�
	//����ATָ��
	char cmd[USART_SEND_LEN] = "AT+NMGS=";
	Num2HexStr(hexstr,num,2*len);
	strcat(cmd,numlenstr);
	strcat(cmd,",");
	strcat(cmd,hexstr);
	strcat(cmd,"\r\n");
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("MSG Send...");	
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_NMGS_WAIT_TIME)//����5��
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				intMsgSentDisp(num);
				vTaskDelay(NEXT_AT_WAIT_TIME);				
				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}

//*******************************************************
//�������ƣ�MSG_Send
//������������ݵ�hexstr��ʽ�������ݳ�������hexΪһ������
//��AΪ1�����ݣ�hex��ʽΪ43
//�������ã���������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 MSG_Send(char * msg,char * len)
{
	u8 try_num = AT_NMGS_REPEAT_NUMBER;//��������
	char hexstr[DATA_MAXNUM] = {0,0,0};
	//����ATָ��
	char cmd[USART_SEND_LEN] = "AT+NMGS=";
	Str2HexStr(msg,hexstr);
	strcat(cmd,len);
	strcat(cmd,",");
	strcat(cmd,hexstr);
	strcat(cmd,"\r\n");
	while(try_num)
	{
		//��ʼ������
		InitDisp();
		cmdDisp("MSG Send...");	
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_NMGS_WAIT_TIME)//����5��
		{
			if(usart.num!=0&&StrCmp("\r\nOK\r\n",usart.USART_RX_BUF))
			{
				//����ȴ�ʱ��
				OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
				//�رյȴ�
				StopWaitMsg();//ֹͣ�ȴ���Ϣ
				MsgDisp(LINE_FEED);
				MsgSentDisp(msg);
				vTaskDelay(NEXT_AT_WAIT_TIME);				
				return try_num;//����
			}
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}
//*******************************************************
//�������ƣ�CMD_Get
//�����������
//�������ã���������
//���ز������ɹ���>=1,TRUE������ʧ�ܣ�0,FALSE��
//*******************************************************
u8 CMD_Get(void)
{
	u8 try_num = AT_NMGR_REPEAT_NUMBER;//������ݴ�����������
	s16 i = 0;//���ڱ���buff
	//���յ�����
	char cmd[15] = "AT+NMGR\r\n";
	
	while(try_num)
	{
		//�������ݣ����ȴ��������ݵ���
		if(MSG_Send("get  msg","8") == 0)//������ݷ���ʧ�ܣ���ô���¿�ʼ
			return SEND_MSG_ERROR;
		vTaskDelay(AT_BEFORE_NMGR_WAIT_TIME);
		//��ʼ��CDP������������������
		i = 0;
		//��ʼ������
		InitDisp();
		cmdDisp("MSG Rcv... ");	
		//��ʼʱ��ȴ�
		StartWaitMsg();
		//��������		
		printf(cmd);
		//�ȴ�����3s��˵����������
		while(usart.waittime <= AT_NMGR_WAIT_TIME)//����5��
		{
			while(usart.USART_RX_BUF[i]==0)//�������һλ
			{
				if(usart.num>=5&&
					i-4>=0&&
					usart.USART_RX_BUF[i-4]=='O'&&
					usart.USART_RX_BUF[i-3]=='K')
				{
					//����ȴ�ʱ��
					OLED_ShowStr(WAITINGTIME_X,WAITINGTIME_Y,"    ",1);
					cmdgot.status = CMD_Anls(usart.USART_RX_BUF);
					if(cmdgot.len != HexStr2Str(cmdgot.lastcmd,cmdgot.str))//���λ�������
						cmdgot.status = CMD_GOT_FORMAT_ERROR;//���ݸ�ʽ����
					//�رյȴ�
					StopWaitMsg();//ֹͣ�ȴ���Ϣ
					MsgDisp(LINE_FEED);
					vTaskDelay(NEXT_AT_WAIT_TIME);

					return try_num;//����
				}
				if(usart.waittime > AT_NMGR_WAIT_TIME)
					break;
			}
			i++;
			OLED_ShowNum(WAITINGTIME_X, WAITINGTIME_Y, usart.waittime, WAITINGTIME_DISP_DIGITS,1);
			#ifdef AT_TICKLESS_ON 
				vTaskDelay(AT_CHECK_EVERYTIME);//ÿ200ms����һ�β鿴�Ƿ�������
			#endif
			
		}
		sgnDisp("- OVERTIME -");
		vTaskDelay(NEXT_AT_WAIT_TIME);
		try_num--;
	}
	//�رյȴ�
	StopWaitMsg();
	return try_num;//ʣ�ೢ�Դ��������Ϊ0.���ʾATָ�����ʧ��
}


