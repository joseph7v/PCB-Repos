#ifndef __MQTT_H
#define __MQTT_H
/* Includes ------------------------------------------------------------------*/

#include "sys.h"


/* Exported Functions --------------------------------------------------------*/

#define CLIENTID            	"005a003f3436510b32323534"
#define KEEP_ALIVE_INTERVAL 	60
#define CLEANSESSION        	1
#define C_USERNAME        		"admin"

////*
#define C_PASSWORD      			"qiaojiaoshou123"
#define MQTT_SEERVER_IP				"210.77.68.221"
#define MQTT_SEERVER_PORT			10020
////*/


#define MY_TOPIC_PICDOWN					"A129FB112111/picdown"
#define MY_TOPIC_PICUP						"A129FB112111/picup"
#define MY_TOPIC_MSGDOWN					"A129FB112111/msgdown"
#define MY_TOPIC_MSGUP						"A129FB112111/msgup"


/*
����1����ȡ����ƫ��λ��
����2����ȡ���ݳ���
����3��buffָ�룬����ȡ��������д�뵽��buff��
���أ����ض�ȡ����ʵ�����ݳ���
*/
#define MY_GET_PICDATA_CALLBACK			u32 (*getdata)(u32,u32,char *)





typedef enum 
{
	MY_MQTT_ERR_NONE=0,
	//�ڴ����
	MY_MQTT_ERR_MEMOVER,
	//���ݴ���
	MY_MQTT_ERR_DATA_ERR,
	//��ȡ���ݴ���
	MY_MQTT_ERR_GETDATA_FAIL,
	//MQTT���ʹ���
	MY_MQTT_ERR_SEND_FAIL,
	//��Ӧ��ʱ
	MY_MQTT_ERR_ACK_TIMEOUT
}MY_MQTT_ERR;


// ���ջ�����
#define MQTT_RECEIVEBUFF_MAXLENGTH		2048
extern u8 MQTT_Receivebuf[MQTT_RECEIVEBUFF_MAXLENGTH];
// ���ͻ�����
#define MQTT_SEND_BUFF_MAXLENGTH		12000
extern u8 MQTT_SEND_buf[MQTT_SEND_BUFF_MAXLENGTH];
// MQTT������
#define MQTT_DATA_BUFF_MAXLENGTH		12000
extern u8 MQTT_DATA_buf[MQTT_DATA_BUFF_MAXLENGTH];


unsigned char mqtt_state_get(void);
void mqtt_state_set(unsigned char state);


void picdown_Flag_set(u8 value);
u8 picdown_Flag_get(void);





//����
int mqtt_subscrib(char *pTopic,char *pMessage);

//��������
int mqtt_connect(u8 *buf,u16 buflen);

//�Ͽ�����
int mqtt_disconnect(void);
//����,ǰ����Ҫ�Ƚ�����������

int mqtt_publish_s(char *pTopic,char *pMessage,u16 msglen,u8 dup, u8 qos,u8 retained,u8 *buf,u16 buflen);

int mqtt_publish_Receive(u8 *data,u16 datalen);

//����
int mqtt_subscribe(char *pTopic);
//��黺����
//int mqtt_subscribe_check(char *pTopic,char *pMessage);
//��鶩������
void checkMQTTRECEIVE(void);


#define FILE_PATH1 "0:/IMAG0012.jpg"
#define FILE_PATH10 "0:/IMAG0010.jpg"
#define FILE_PATH11 "0:/IMAG0011.jpg"
#define FILE_PATH12 "0:/IMAG0012.jpg"
#define FILE_PATH2 "0:/PIC.JPG"

//unsigned char send_picture(char* file_path, u16  packagelen, int send_picture_cnt, u16 timeOut);
MY_MQTT_ERR send_picture(u8 *mqttbuff,u32 mqttbufflength,	// MQTT��Ϣ����Ҫ��buff�ռ��Լ�����
						 u8 *pic_buff,u32 pic_bufflen,	// ����ͼƬ�ְ���Ҫ��buff�ռ��Լ�buffʵ�ʳ���
						 char* file_path,						// ͼƬ·��
						 char* file_path_d,						// ͼƬ·��(������ɺ�)
						 int picture_id,						// ͼƬid
						 u32 packagelen,						// ͼƬ��Ƭ����
						 u16 timeOut);						// �ȴ�picdonw��Ӧ���������Ҫ����ע��

						 MY_MQTT_ERR mysend_picture(char* file_path,						// ͼƬ·��
						 char* file_path_d,						// ͼƬ·��(������ɺ�)
						 int picture_id);						// ͼƬid	
													
MY_MQTT_ERR send_data(u8 *mqttbuff,u32 mqttbufflength, char* msg)	;
#endif

