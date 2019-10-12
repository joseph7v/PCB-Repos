#include "includes.h"

#include "MQTTPacket.h"
#include "transport.h"
#include "UART_TCPbuff.h"
#include "mqtt.h"

#include "fattester.h"	 
#include "sdio_sdcard.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"

// ���ջ�����
u8 MQTT_Receivebuf[MQTT_RECEIVEBUFF_MAXLENGTH];
// ���ͻ�����
u8 MQTT_SEND_buf[MQTT_SEND_BUFF_MAXLENGTH];
// MQTT������
u8 MQTT_DATA_buf[MQTT_DATA_BUFF_MAXLENGTH];


unsigned char  mqtt_state=0;

unsigned char  mqtt_state_get()
{
	return mqtt_state;
}
void mqtt_state_set(unsigned char state)
{
	mqtt_state=state;
}

u8 picdown_Flag=0;
void picdown_Flag_set(u8 value)
{
	picdown_Flag=value;
}

u8 picdown_Flag_get()
{
	return picdown_Flag;
}


/**
  * @brief  �����������һ����������
  * @param  pTopic ��Ϣ���⣬����
  * @param  pMessage ��Ϣ���ݣ�����
  * @retval С��0��ʾ������Ϣʧ��
  */
int mqtt_connect(u8 *buf,u16 buflen)
{
	s32 len,rc;
	//u16 time;
	//u16 receivelength=0;

	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	//
	data.clientID.cstring = CLIENTID;
	//���ӱ���ʱ��,��
	data.keepAliveInterval = 255;
	//����Ự��־��1��ÿ�����ߺ�����¶��ģ�0��ÿ�����ߺ����½���
	/*����Ự��־��ʾ������ǰ�����Ӽ�¼���ֹ�ϵ�������ǰ�ж�����Ϣ����Ҫ���¶���*/
	data.cleansession = 1;
	//��¼�û���
	data.username.cstring = C_USERNAME;
	//��¼����
	data.password.cstring = C_PASSWORD;
	transport_open();
	delay_ms(200);

	len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
	rc = transport_sendPacketBuffer(buf,len);

	return rc;
}

int mqtt_disconnect(void)
{
	s32 len,rc;
	unsigned char buf[500];
	int buflen = 500;
	len = MQTTSerialize_disconnect(buf, buflen); /* 3 */
	rc = transport_sendPacketBuffer(buf,len);

	transport_close();//�رն˿�

	return rc;
}


//����һ��MATT��Ϣ
//ʧ�ܷ���0 ���ɹ�����1
int mqtt_publish_s(char *pTopic,char *pMessage,u16 msglen,u8 dup, u8 qos,u8 retained,u8 *buf,u16 buflen)
{
	s32 len;
	u8 rc;
	MQTTString topicString = MQTTString_initializer;
	//����
	topicString.cstring = pTopic;
	len = MQTTSerialize_publish(buf, buflen,dup,qos,retained, 0, topicString, (unsigned char*)pMessage, msglen);
	rc=transport_sendPacketBuffer(buf,len);
	return rc;
	}


/**
  * @brief  �����������һ����Ϣ���ú�������ΪTCP�������ݺ���������
  * @param  pTopic ��Ϣ���⣬����
  * @retval С��0��ʾ������Ϣʧ��
  */
int mqtt_subscribe(char *pTopic)
{
	int rc = 0;
	unsigned char buf[200];
	int buflen = 200;
	int msgid = 1;//��ȷ�Ϸ���ID
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	int len = 0;
	//int type=0;

	topicString.cstring = pTopic;
	//����Ҫ���͵Ķ��������������л���buf����׼������
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);
	//�����л������ݷ��ͳ�ȥ
	rc = transport_sendPacketBuffer(buf, len);

	return rc;
}

int mqtt_Ping()
{
	int rc = 0;
	int len;
	unsigned char buf[200];
	len = MQTTSerialize_pingreq(buf, 200);//????
	rc = transport_sendPacketBuffer(buf, len);//??
	return rc;
}


void checkMQTTRECEIVE(void)
{
	//===========================
//	s32 type;
//	// unsigned short submsgid;
//	// int subcount;
//	// int granted_qos;
//	u8 dup;
//	s32 qos;
//	//������־
//	u8 retained;
//	//��id
//	u16 packetid;
//	MQTTString topicName;
//	//char cstring[20];
//	//����	
//	u8 *payload;
//	//���ݳ���
//	s32 payloadlen;
//	//==============================
//	if(UART_TCP_buffLength()!=0)
//	{
//		//F407USART1_SendString("UART_TCP received data\r\n");
//		//������յ���MQTT��Ϣ�������ݲ�ͬ����Ϣ��������ͬ�Ĵ���
//		type=MQTTPacket_read(MQTT_Receivebuf, MQTT_RECEIVEBUFF_MAXLENGTH, transport_getdata);
//		switch(type)
//		{
//			case CONNECT:break;
//			case CONNACK://����������Ӧ
//				mqtt_state_set(1);
//				break;
//			case PUBLISH://���ĵ���Ϣ,�ɱ��˷���
//					if(MQTTDeserialize_publish(&dup,&qos,&retained,&packetid,&topicName,&payload,&payloadlen,MQTT_Receivebuf,MQTT_RECEIVEBUFF_MAXLENGTH)==1)
//					{							
//						if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_PICDOWN,strlen(MY_TOPIC_PICDOWN))!=-1)
//						{
//							printf("\r\nTOPIC1=%s,payloadlen=%d\r\n",MY_TOPIC_MSGDOWN,payloadlen);
//							// for(i=0;i<payloadlen;i++)
//							// {
//							// 	F407USART1_SendByte('*');
//							// 	F407USART1_SendByte(payload[i]);
//							// 	F407USART1_SendByte('*');
//							// }
//							picdown_Flag_set(1);
//						}
//						else if(MystringSeek(topicName.lenstring.data,topicName.lenstring.len,MY_TOPIC_MSGDOWN,strlen(MY_TOPIC_MSGDOWN))!=-1)
//						{
//							printf("TOPIC2=%s,payloadlen=%d\r\n",MY_TOPIC_MSGDOWN,payloadlen);
//						}	
//					}
//				break;
//			case PUBACK://������Ϣ��Ӧ����ʾ�����ɹ�
//				break;
//			case PUBREC:break;
//			case PUBREL:break;
//			case PUBCOMP:break;
//			case SUBSCRIBE:break;
//			case SUBACK:break;//������Ϣack
//			case UNSUBSCRIBE:break;
//			case UNSUBACK:break;
//			case PINGREQ:break;
//			case PINGRESP://Pin����
//				break;
//			case DISCONNECT:break;//�ɿͻ��˷��͸�����������ʾ�����Ͽ�����
//			default:break;
//		}
//	}
}


#define SDCARD_BUFFER 8192
struct pick_package{
	u16  start_byte;            //��ʼ�ֽڣ��̶�Ϊ0xa596
	u16  picture_id;            //ͼƬ���У����´�����Ҫ�Զ���1
	u16  packet_total_num;      //һ���ֳɶ��ٰ�
	u16  data_length;           //�������ݳ��� 
	u16  packet_id;             //����ͼƬ��ID
	u32  data_total_length;     //ͼƬ�����ܳ���   
	//unsigned char 	data[data_len];		   //ͼƬ����
};
u16 mypicPack_serialize(char *buf,  struct pick_package pkg)
{
	// ��ʼ�ֽڣ��̶�Ϊ0xa596
	buf[0]=0xa5;
	buf[1]=0x96;
	// ͼƬ���У����´�����Ҫ�Զ���1
	buf[2]=pkg.picture_id>>8;
	buf[3]=pkg.picture_id;
	
	//һ���ֳɶ��ٰ�
	buf[4]=pkg.packet_total_num>>8;
	buf[5]=pkg.packet_total_num;
	// ����ͼƬ��ID
	buf[6]=pkg.packet_id>>8;
	buf[7]=pkg.packet_id;
	// �������ݳ��� 
	buf[8]=pkg.data_length>>8;
	buf[9]=pkg.data_length;
	// ͼƬ���ݳ���
	buf[10]=pkg.data_total_length>>24;
	buf[11]=pkg.data_total_length>>16;	
	buf[12]=pkg.data_total_length>>8;
	buf[13]=pkg.data_total_length;
	return 14;
}
	
MY_MQTT_ERR send_picture(u8 *mqttbuff,u32 mqttbufflength,	// MQTT��Ϣ����Ҫ��buff�ռ��Լ�����
						 u8 *pic_buff,u32 pic_bufflen,	// ����ͼƬ�ְ���Ҫ��buff�ռ��Լ�buffʵ�ʳ���
						 char* file_path,						// ͼƬ·��
						 char* file_path_d,						// ͼƬ·��(������ɺ�)
						 int picture_id,						// ͼƬid
						 u32 packagelen,						// ͼƬ��Ƭ����
						 u16 timeOut)						// �ȴ�picdonw��Ӧ���������Ҫ����ע��
{
	// ���ṹ��
	struct pick_package pic_pack;

	// �ļ���ȡ����
	FIL fl_in; 
	FRESULT res;
	u32 getLen;  // ÿ��ʵ�ʷ������ݳ��ȣ���packagelen�Ա�
		
	u16 picPack_offset=0;  // ���ͻ�����ƫ����
	u8 reSendCount=0;  // �ط�����ͳ��
	
	
	u32 otherLen;  // ����û�з��͵ĳ���
	u16 time=0;  // ��ʱʱ��ͳ��
	

	// ������
	res = f_open(&fl_in,file_path,FA_OPEN_EXISTING|FA_READ);	//#to-do ���ĺ궨�� ok
	if(res != FR_OK)
	{
		printf("MY_MQTT_ERR_GETDATA_FAIL <PATH:%s>\r\n",file_path);
		return MY_MQTT_ERR_GETDATA_FAIL;  // ���������Ҫ����
	}
	
	// �̶���ʼ�ֽ� 
	pic_pack.start_byte=0x96a5;	

	// ÿ��ͼƬΨһ,���ڸ��ݶ�ȡ�����ݸı�
	pic_pack.picture_id = picture_id;	
	printf("#test picture_id=%d\r\n",picture_id);
	// ��ȡ�ļ���С
	pic_pack.data_total_length = f_size(&fl_in);  
	// ���㴫�����
	pic_pack.packet_total_num = (pic_pack.data_total_length/packagelen) + 1; 
	printf("\r\nf_open successfully <file_path:%s|file_size:%d|sub_num:%d|packagelen:%d>\r\n",\
	file_path,pic_pack.data_total_length,pic_pack.packet_total_num,packagelen);
	
	MQTT_RESEND:
	pic_pack.data_length = packagelen;
	// ����
	f_lseek(&fl_in,0);  //��Ŀ¼�ƶ����ʼ���ʼλ��
	otherLen = pic_pack.data_total_length;
	pic_pack.packet_id = 0;
	time = 0;  
	
	while(1)
	{
		// ��ȡ����
		if(otherLen > packagelen)  // ʣ�����ݴ���ͼƬ��Ƭ����
		{
			pic_pack.data_length = packagelen;
			// ���л���ͷ
			picPack_offset=mypicPack_serialize(pic_buff, pic_pack);
			f_read(&fl_in,pic_buff+picPack_offset,packagelen,&getLen);
			if(getLen!=packagelen)
			{
				f_close(&fl_in); 
				printf("MY_MQTT_ERR_GETDATA_FAIL <rc|pack:%d|%d>\r\n",getLen,packagelen);
				return MY_MQTT_ERR_GETDATA_FAIL;  // ���������Ҫ����
			}
		}
		else if(otherLen>0)  // С�ڷ�Ƭ����
		{
			pic_pack.data_length = otherLen;
			// ���л���ͷ
			picPack_offset=mypicPack_serialize(pic_buff, pic_pack);
			f_read(&fl_in,pic_buff+picPack_offset,otherLen,&getLen);
			if(getLen!=otherLen)
			{
				f_close(&fl_in); 
				printf("MY_MQTT_ERR_GETDATA_FAIL <rc|pack:%d|%d>\r\n",getLen,packagelen);
				return MY_MQTT_ERR_GETDATA_FAIL;  // ���������Ҫ����
			}
		}
		else //����������ϣ��ȴ�ȷ������
		{	
			printf("�ȴ���Ӧ...\r\n");
			while(!picdown_Flag_get())
			{
				time++;
				delay_ms(1);
				if(time>=timeOut)
				{
					reSendCount++;
					if(reSendCount>3)
					{
						printf("MY_MQTT_ERR_SEND_FAIL\r\n");
						return MY_MQTT_ERR_SEND_FAIL;
					}
					else
					{
						printf("MY_MQTT_ERR_ACK_TIMEOUT,retry:%d times\r\n",reSendCount);
						goto MQTT_RESEND;
					}
					//return MY_MQTT_ERR_ACK_TIMEOUT;
				}
			}

			// �ɹ�����һ��ͼƬ
			f_close(&fl_in); 

			// �ƶ��ļ�
			res = f_rename(file_path,file_path_d);
			if(res != FR_OK)
			{
				printf("Fail to rename file:%s\r\n%s\r\nERROR:%d\r\n", file_path, file_path_d, res);
				return MY_MQTT_ERR_DATA_ERR;
			}else
			{
				printf("rename file:%s\r\n%s\r\nERROR:%d\r\n", file_path, file_path_d, res);
			}

			printf("MY_MQTT_ERR_NONE�����ͳɹ�\r\n");
			return MY_MQTT_ERR_NONE;
		}	

		// ��������
		if(mqtt_publish_s(MY_TOPIC_PICUP,pic_buff,getLen+picPack_offset,0,1,0,mqttbuff,mqttbufflength)==0)
		{
			f_close(&fl_in); 
			printf("MY_MQTT_ERR_SEND_FAIL\r\n");
			return MY_MQTT_ERR_SEND_FAIL;
		}

		otherLen-=getLen;
		printf("otherLen:%d\r\n",otherLen);
		pic_pack.packet_id++;
	}
}

MY_MQTT_ERR mysend_picture(char* file_path,						// ͼƬ·��
						 char* file_path_d,						// ͼƬ·��(������ɺ�)
						 int picture_id)						// ͼƬid						 					
{
	MY_MQTT_ERR res;
	u32 packagelen=10000;					// ͼƬ��Ƭ����
	u16 timeOut=50000;

	res = send_picture( MQTT_SEND_buf,MQTT_SEND_BUFF_MAXLENGTH,
						MQTT_DATA_buf,MQTT_DATA_BUFF_MAXLENGTH,
						file_path, file_path_d,
						picture_id, packagelen, timeOut);
	return res;		
}

MY_MQTT_ERR send_data(u8 *mqttbuff,u32 mqttbufflength, char* msg)						
{
	
	if(mqtt_publish_s(MY_TOPIC_MSGUP,msg,strlen(msg),0,1,0,mqttbuff,mqttbufflength)==0)
	{
		printf("MY_MQTT_ERR_SEND_FAIL\r\n");
		return MY_MQTT_ERR_SEND_FAIL;
	}
	return MY_MQTT_ERR_NONE;
}



