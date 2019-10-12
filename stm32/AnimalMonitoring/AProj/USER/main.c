#include "includes.h"

// ��������Ӳ��
#include "led.h"
#include "key.h"  
#include "power.h"

// �ļ�ϵͳ
#include "malloc.h"  
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "usbh_usr.h" 
 
// ����
#include "draft.h"

// ����
#include "usart3.h" 
#include "ec25.h" 
#include "UART_TCPbuff.h"
#include "mqtt.h"

#include "mqtt.h"
#include "MQTTPacket.h"
#include "transport.h"
/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				256
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);			

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			5 
//���������ջ��С
#define MAIN_STK_SIZE  					3000
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//����ɨ������
//�����������ȼ�
#define WATCH_TASK_PRIO       			4 
//���������ջ��С
#define WATCH_STK_SIZE  					3000
//�����ջ	
OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//������
void watch_task(void *pdata);

//����ɨ������
//�����������ȼ�
#define SCAN_TASK_PRIO       			3
//���������ջ��С
#define SCAN_STK_SIZE  					3000
//�����ջ	
OS_STK SCAN_TASK_STK[SCAN_STK_SIZE];
//������
void scan_task(void *pdata);

// ������Ϣ
#define EC25_TASK_PRIO       			6
//���������ջ��С
#define EC25_STK_SIZE  					3000
//�����ջ	
OS_STK EC25_TASK_STK[EC25_STK_SIZE];
//������
void ec25_task(void *pdata);

// ������Ϣ
#define MQTTRECEIVE_TASK_PRIO       			7
//���������ջ��С
#define MQTTRECEIVE_STK_SIZE  					300
//�����ջ	
OS_STK MQTTRECEIVE_TASK_STK[MQTTRECEIVE_STK_SIZE];
//������
void MqttReveiveTask(void *pdata);
//////////////////////////////////////////////////////////////////////////////

u8 key1_down=0;
u8 key2_down=0;
u8 wkup_down=0;

int main(void)
{ 
	u8 state;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	printf("uart_init init\r\n");	
	usart3_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	KEY_Init(); 				//������ʼ��  
	ec25_Init();
	Power_Ctrl_Init();  // ��Դ��ʼ��
	Cam_Crtl_Init();  // ����������ų�ʼ��
	//state=ec25_soft_start();  // ����EC25
	//printf("ec25_soft_start state:%d\r\n", state);
#ifdef USB_ENABLE
	//��ʼ��USB����
	//USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);
#endif	
	
//	if(!SD_Init()) printf("SD_Init ok\r\n");//�ж�SD���Ƿ����
//	else printf("SD_Init Error\r\n");	

//	mymem_init(SRAMIN);		//��ʼ���ڲ��ڴ��	
// 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
//  	f_mount(fs[0],"0:",1); 	//����SD��  
//  	f_mount(fs[1],"1:",1); 	//����U�� 	
//	
  	OSInit();  	 				//��ʼ��UCOSII
  	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	    
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		   			  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   
 	OSTaskCreate(watch_task,(void *)0,(OS_STK*)&WATCH_TASK_STK[WATCH_STK_SIZE-1],WATCH_TASK_PRIO);	
	OSTaskCreate(scan_task,(void *)0,(OS_STK*)&SCAN_TASK_STK[SCAN_STK_SIZE-1],SCAN_TASK_PRIO);
	OSTaskCreate(MqttReveiveTask,(void *)0,(OS_STK*)&MQTTRECEIVE_TASK_STK[MQTTRECEIVE_STK_SIZE-1],MQTTRECEIVE_TASK_PRIO);	
	OSTaskCreate(ec25_task,(void *)0,(OS_STK*)&EC25_TASK_STK[EC25_STK_SIZE-1],EC25_TASK_PRIO);			
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}	  
//������
void main_task(void *pdata)
{	
	u8 flag=0;
	u8 state;
	printf("main_task run\r\n");	
	while(1)
	{
//		if(key1_down==1 && flag==0)
//		{
//			printf("key1_down..openUSB...\r\n");
//			usb_state_set(1);  // ���ñ�־����USB
//			usbapp_mode_set(); 
//			openUSB();
//			state=WaitDistinguishflag(30000);
//			if(state==1)
//			{
//				printf("WaitDistinguishflag...\r\n");
//				
//				mydump_file();
//			}else
//			{
//				printf("Fail WaitDistinguishflag...\r\n");
//			}
//			key1_down=0;
//			flag=1;
//			
//		}
//		else if(key1_down==1 && flag==1)
//		{
//			printf("key1_down..closeUSB...\r\n");
//						
//			closeUSB();
//			
//			state=WaitDisconnectflag(3000);
//			if(state==1)
//			{
//				printf("USB_CONNECT_OFF...\r\n");
//				USB_CONNECT_OFF();
//				usbapp_mode_stop();
//				
//				//mysend_allpic();
//			}
//			else
//			{
//				printf("Fail USB_CONNECT_OFF...\r\n");
//			}
//			
//			//mydump_file_auto();
//			key1_down=0;
//			flag=0;
//		}
//		
//		if(key2_down==1)
//		{
//			printf("key2_down..closeUSB...\r\n");
//			USB_Photograph();
//			key2_down = 0;
//		}
		delay_ms(10);
	}
} 
extern u8 memdevflag; // USB�Ƿ����ӳɹ�
//����ɨ������
void watch_task(void *pdata)
{	
	u32 t;
	//uint8 sss=0;
	
	while(1)
	{
		t++;
		if(t%1000 == 200) 
		{
			LED_GREEN_OFF();
		}
		if(t%1000 == 0)
		{
			t=0;
			LED_GREEN_ON();
		}
		if((t%600)==0)//600ms���Ҽ��1��
		{
			//U����λ���   
			if(usbx.hdevclass==1)
			{
				//printf("U disk is ON,memdevflag=%d\r\n",memdevflag);
				if(memdevflag==0)//U�̲���λ?
				{
					fs[1]->drv=2;  //��ʱ��ΪҲ���Բ���,����Ϊ������������
					f_mount(fs[1],"1:",1); 	//���¹���U��
					USB_CON();
					usb_cflag_set(1);  // ״̬������ӳɹ�
					memdevflag=1;	// ���U����λ	
				}  
			}else memdevflag=0;	//U�̱��γ��� 
		}		
		while((usbx.bDeviceState&0XC0)==0X40)//USB�豸������,���ǻ�û���ӳɹ�,�Ͳ�ѯ.
		{
			
			usbapp_pulling();	//��ѯ����USB����
			delay_us(1000);		//������HID��ô��...,U�̱Ƚ���
			UART_TCPbuff_Run(F407USART3_buffRead);
		}
		if(t%10==0) 
			usbapp_pulling();//����USB����,1/ 10ms
		
		UART_TCPbuff_Run(F407USART3_buffRead);
		delay_ms(1);
	}
}

// С����ɨ������
static void scan_task(void *pdata)
{
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan(0);   
		switch(key)
		{
			case KEY1_PRES:		//����KEY_UP��
				key1_down=1;
				printf("KEY1_PRES\r\n");
				break;
			case KEY2_PRES:		//����KEY_UP��
				key2_down=1;
				printf("KEY2_PRES\r\n");
				break;
			case WKUP_PRES:		//����KEY_UP��
				wkup_down=1;
				printf("WKUP_PRES\r\n");
				break;
			default:break;
		}
		//UART_TCPbuff_Run();  // ��ȡ���ڻ�����������
		//checkMQTTRECEIVE();
 		delay_ms(5);
	}
}

static void ec25_task(void *pdata)
{
	//OS_CPU_SR cpu_sr=0;
	
	//char* text="1234567890";
	u32 time=0;
	
	u8 sss=0;
	
	
	printf("ec25_task run\r\n");
	while(1)
	{
		time++;
		if((time%5)==0)// 1000ms���Ҽ��1��
		{
			if(mqtt_state_get()==0)  // ������Ҫ�ټ�һ���жϣ�����ec25�Ƿ�����������
			{
				//OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    	
				printf("try mqtt_connect\r\n");
				mqtt_connect(MQTT_SEND_buf,MQTT_SEND_BUFF_MAXLENGTH);
				sss=0;
				//OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
			}
				
		}
			
		if(mqtt_state_get()==1)
		{
			LED_YELLOW_ON();
			//int mqtt_publish_s(char *pTopic,char *pMessage,u16 msglen,u8 dup, u8 qos,u8 retained,u8 *buf,u16 buflen)
			//printf("test mqtt_publish_s\r\n");
			//mqtt_publish_s("AAA",text,10,0,1,0,MQTT_DATA_buf,MQTT_DATA_BUFF_MAXLENGTH);
			if(sss==0)
			{
				mqtt_subscribe(MY_TOPIC_PICDOWN);
				mqtt_subscribe(MY_TOPIC_MSGDOWN);
				sss=1;	
			}
		}
		else 
		{
			LED_YELLOW_OFF();
		}
		printf("time cnt=%d\r\n",time);
		delay_ms(1000);
	}
}

static void MqttReveiveTask(void *pdata)
{
		//===========================
	s32 type;
	// unsigned short submsgid;
	// int subcount;
	// int granted_qos;
	u8 dup;
	s32 qos;
	//������־
	u8 retained;
	//��id
	u16 packetid;
	MQTTString topicName;
	//char cstring[20];
	//����	
	u8 *payload;
	//���ݳ���
	s32 payloadlen;
	//==============================
	printf("MqttReveiveTask run\r\n");
	while(1)
	{
		if(UART_TCP_buffLength()!=0)
		{
			printf("UART_TCP_buffLength !=0\r\n");
			//F407USART1_SendString("UART_TCP received data\r\n");
			//������յ���MQTT��Ϣ�������ݲ�ͬ����Ϣ��������ͬ�Ĵ���
			type=MQTTPacket_read(MQTT_Receivebuf, MQTT_RECEIVEBUFF_MAXLENGTH, transport_getdata);
			switch(type)
			{
//				case CONNECT:break;
				case CONNACK://����������Ӧ
					mqtt_state_set(1);
					break;
				case PUBLISH://���ĵ���Ϣ,�ɱ��˷���
						if(MQTTDeserialize_publish(&dup,&qos,&retained,&packetid,&topicName,&payload,&payloadlen,
								MQTT_Receivebuf,MQTT_RECEIVEBUFF_MAXLENGTH)==1)
						{
							
							//if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_MSGDOWN,strlen(MY_TOPIC_MSGDOWN))!=-1)
							if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_PICDOWN,strlen(MY_TOPIC_PICDOWN))!=-1)
							{//??????
								printf("\r\nTOPIC1=%s,payloadlen=%d\r\n",MY_TOPIC_MSGDOWN,payloadlen);
								picdown_Flag_set(1);
							}
							else if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_MSGDOWN,strlen(MY_TOPIC_MSGDOWN))!=-1)
							{//JSON??
								printf("TOPIC2=%s,payloadlen=%d\r\n",MY_TOPIC_MSGDOWN,payloadlen);
							}	
						}
//					if(MQTTDeserialize_publish(&dup,&qos,&retained,&packetid,&topicName,&payload,&payloadlen,
//							MQTT_Receivebuf,MQTT_RECEIVEBUFF_MAXLENGTH)==1)
//					{
//						//��MQTT��Ϣ������Ϊ���ݣ����ж���Ϣ������
//						if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_PICDOWN,strlen(MY_TOPIC_PICDOWN)))
//						{//ͼƬ��Ϣ��Ӧ
//							printf("TOPIC{%s} down\r\n",MY_TOPIC_PICDOWN);
//							for(i=0;i<payloadlen;i++)
//								F407USART3_SendByte(payload[i]);
//							picdown_Flag_set(1);
//						}
//						else if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_MSGDOWN,strlen(MY_TOPIC_MSGDOWN))!=-1)
//						{//JSON��Ϣ
//							printf("TOPIC{%s} down\r\n",MY_TOPIC_MSGDOWN);
//							for(i=0;i<payloadlen;i++)
//								F407USART3_SendByte(payload[i]);
//						}	
//					}
					break;
				case PUBACK://������Ϣ��Ӧ����ʾ�����ɹ�
					break;
				case PUBREC:break;
				case PUBREL:break;
				case PUBCOMP:break;
				case SUBSCRIBE:break;
				case SUBACK://������Ϣack
					break;
				case UNSUBSCRIBE:break;
				case UNSUBACK:break;
				case PINGREQ:break;
				case PINGRESP://Pin����
					break;
				case DISCONNECT:break;//�ɿͻ��˷��͸�����������ʾ�����Ͽ�����
				default:break;
			}
		}
		delay_ms(5);
	}
}

