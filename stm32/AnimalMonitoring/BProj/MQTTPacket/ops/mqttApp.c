#include "mqttApp.h"
#include "mqttBase.h"
// fundamental
#include "usart.h"
#include "led.h"
#include "delay.h"
// communication
#include "usart3.h"
#include "ec25.h"
#include "UART_TCPbuff.h"
#include "MyFunction_C.h"
#include "queue.h"
// file system
#include "fattester.h"	
// mqtt ����״̬
static unsigned char mqtt_state = 0;
unsigned char mqtt_state_get()
{
	return mqtt_state;
}
void mqtt_state_set(unsigned char state)
{
	mqtt_state = state;
	printf("mqtt_state_set %d\r\n", state);
}
// ping״̬
static unsigned char mqtt_ping_state = 0;
unsigned char mqtt_ping_state_get()
{
	return mqtt_ping_state;
}
void mqtt_ping_state_set(unsigned char state)
{
	mqtt_ping_state = state;
}
// ��ʼ������
SqQueue Q_stage;   // ��������������
SqQueue Q_resent;  // �����ط�������
QElemType frame_p; // ����Ԫ��
/**
 * @description: serialize package 
 * @param buf ���ͻ�������ַ
 * @param pkg ���ݰ�
 * @return: ���л�������
 */
uint16_t mypicPack_serialize(char *buf, struct pick_package pkg)
{
	uint32_t uid = 0;
	// ��ʼ�ֽڣ��̶�Ϊ0xa596
	buf[0] = 0xa5;
	buf[1] = 0x96;

	uid = Frame_UID_get();
	buf[2] = uid >> 24;
	buf[3] = uid >> 16;
	buf[4] = uid >> 8;
	buf[5] = uid;

	// ͼƬ���У����´�����Ҫ�Զ���1
	buf[6] = pkg.picture_id >> 8;
	buf[7] = pkg.picture_id;

	//һ���ֳɶ��ٰ�
	buf[8] = pkg.packet_total_num >> 8;
	buf[9] = pkg.packet_total_num;
	// ����ͼƬ��ID
	buf[10] = pkg.packet_id >> 8;
	buf[11] = pkg.packet_id;
	// �������ݳ���
	buf[12] = pkg.data_length >> 8;
	buf[13] = pkg.data_length;
	// ͼƬ���ݳ���
	buf[14] = pkg.data_total_length >> 24;
	buf[15] = pkg.data_total_length >> 16;
	buf[16] = pkg.data_total_length >> 8;
	buf[17] = pkg.data_total_length;
	//crc
	buf[18] = 0;
	buf[19] = 0;
	// ����֡��Ϣ
	frame_p.uid = uid;
	frame_p.pic_id = pkg.picture_id;
	frame_p.pack_id = pkg.packet_id;
	return 20;
}
/**
 * @description: mqtt���ݷ��ͺ����������ط�����
 * @param pTopic ����
 * @param pMessage ������Ϣ
 * @param msglen ��Ϣ����
 * @param buf mqtt���ݻ�����
 * @param buflen mqtt����������
 * @return: 
 */
MY_MQTT_ERR mqtt_publish_data(char *pTopic, char *pMessage, uint16_t msglen)// ��Ч��Ϣ
{
	uint8_t i, j;
	// �ط�����
	uint8_t try_cnt = 0;	// ��������
	uint8_t perst_flag = 0; // reset flag
	uint16_t pack_crc = 0;  // CRC result

	try_cnt = 0;
	LED_GREEN_ON();
	// while ((mqtt_publish_s(pTopic, pMessage, msglen, 0, 1, 0, buf, buflen) == 0))
	while ((mqtt_publish(pTopic, pMessage, msglen) == 0))
	{
		if (try_cnt >= 3) // �����쳣���ر�����ͻ���
		{
			if (perst_flag == 0)
			{
				perst_flag = 1;
				try_cnt = 0;
				ec25_Restart();
				printf("ec25_reset ec25_reset ec25_reset!!!!");
			}
			else if (perst_flag == 1)
			{
				//f_close(&fl_in);
				// �����м����
				// �����м����
				printf("Network ERROR!!!!");
				F407USART1_SendString("mqtt_publish_s failed:MY_MQTT_ERR_SEND_FAIL\r\n");
				return MY_MQTT_ERR_SEND_FAIL;
			}
		}
		printf("mqtt_publish failed, try to reconnect...try_cnt=%d\r\n", try_cnt);
		// reconnect
		i = 0, j = 0;
		while (mqtt_connect() == 0) // ���½���MQTT����,�������̻�ر����磬Ȼ���������ӣ�
		{
			j++;
			printf("ccc%dccc", j);
			delay_ms(1000);
			if (j > 3)
			{
				i = 20;
				break;
			}
		}
		printf("\r\n");
		mqtt_state_set(0);
		while (mqtt_state_get() == 0) // ���ӳɹ����ȴ�10s
		{
			printf("...%d...", i);
			delay_ms(500);
			if (i++ >= 20) //�ȴ�10s
				break;
		}
		printf("\r\n");
		try_cnt++;
	}
	LED_GREEN_OFF();
}
/** 
 * @brief send pictures of assigned path(folder)
 * @param pic_buff ����ͼƬ�ְ���Ҫ��buff�ռ�
 * @param pic_bufflen ����ͼƬ�ְ���Ҫ��buff�ռ䳤��
 * @param file_path ͼƬ·��
 * @param picture_id ͼƬid
 * @param packagelen ͼƬ��Ƭ����
 * @param timeOut �ȴ�picdonw��Ӧ���������Ҫ����ע��
 * @return num of files
 * @note  max 20
 */
MY_MQTT_ERR send_picture(uint8_t *pic_buff, uint32_t pic_bufflen,
						 uint8_t *file_path,
						 uint32_t picture_id,
						 uint32_t packagelen,
						 uint16_t timeOut)
{
	OS_ERR err;
	uint8_t try_cnt = 0;	// ��������
	uint8_t perst_flag = 0; // reset flag
	uint16_t pack_crc = 0;  // CRC result
	// ���ṹ��
	struct pick_package pic_pack;
	// Queue status
	Status q_res;
	// MQTT status
	MY_MQTT_ERR mqtt_res;
	// ��ȡ�ļ�״̬
	FIL fl_in;
	FRESULT res;
	uint32_t getLen;			 // ��ȡ������
	uint32_t otherLen;			 // ʣ���ܳ���
	uint16_t picPack_offset = 0; // package����/ƫ����
	// ���ļ�
	res = f_open(&fl_in, (const char *)file_path, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		printf("MY_MQTT_ERR_GETDATA_FAIL <PATH:%s>\r\n", file_path);
		return MY_MQTT_ERR_GETDATA_FAIL;
	}
	// ÿ��ͼƬΨһ,���ڸ��ݶ�ȡ�����ݸı�
	pic_pack.picture_id = picture_id;
	printf("#now sending picture_id=%d#\r\n", picture_id);
	// ��ȡ�ļ���С
	pic_pack.data_total_length = f_size(&fl_in);
	// ���㴫�����
	pic_pack.packet_total_num = (pic_pack.data_total_length / packagelen) + 1;
	printf("f_open successfully <file_path:%s|file_size:%d|sub_num:%d|packagelen:%d>\r\n",
		   file_path, pic_pack.data_total_length, pic_pack.packet_total_num, packagelen);
	pic_pack.data_length = packagelen;
	// ����
	f_lseek(&fl_in, 0); // ��Ŀ¼�ƶ����ʼ���ʼλ��
	otherLen = pic_pack.data_total_length;
	pic_pack.packet_id = 0;

	while (1)
	{
		int i; //temp
		// ��ȡ����
		if (otherLen > packagelen) // ʣ�����ݴ���ͼƬ��Ƭ����
		{
			pic_pack.data_length = packagelen;
			// ���л���ͷ
			picPack_offset = mypicPack_serialize((char *)pic_buff, pic_pack);
			f_read(&fl_in, pic_buff + picPack_offset, packagelen, &getLen);
			if (getLen != packagelen)
			{
				f_close(&fl_in);
				printf("MY_MQTT_ERR_GETDATA_FAIL <rc|pack:%d|%d>\r\n", getLen, packagelen);
				return MY_MQTT_ERR_GETDATA_FAIL; // ���������Ҫ����
			}
			// add crc
			pack_crc = ModBusCRC(pic_buff + picPack_offset, getLen);
			// printf("%0X,picPack_offset=%d,packagelen=%d\r\n", pack_crc, picPack_offset, getLen);
			pic_buff[18] = pack_crc >> 8;
			pic_buff[19] = pack_crc;
		}
		else if (otherLen > 0) // С�ڷ�Ƭ����
		{
			pic_pack.data_length = otherLen;
			// ���л���ͷ
			picPack_offset = mypicPack_serialize((char *)pic_buff, pic_pack);
			f_read(&fl_in, pic_buff + picPack_offset, otherLen, &getLen);
			if (getLen != otherLen)
			{
				f_close(&fl_in);
				printf("MY_MQTT_ERR_GETDATA_FAIL <rc|pack:%d|%d>\r\n", getLen, packagelen);
				return MY_MQTT_ERR_GETDATA_FAIL; // ���������Ҫ����
			}
			// add crc
			pack_crc = ModBusCRC(pic_buff + picPack_offset, getLen);
			printf("%0X,picPack_offset=%d,packagelen=%d\r\n", pack_crc, picPack_offset, getLen);
			pic_buff[18] = pack_crc >> 8;
			pic_buff[19] = pack_crc;
		}
		else // ����������ϣ��ȴ�ȷ������
		{
			f_close(&fl_in);
			F407USART1_SendString("PICTURE MY_MQTT_ERR_NONE�����ͳɹ�\r\n");
			return MY_MQTT_ERR_NONE;
		}
		// ����ǰ����ջ�����
		UART_TCPbuff_Run(F407USART3_buffRead);
		// ��������
		mqtt_res = mqtt_publish_data(MY_TOPIC_PICUP, (char *)pic_buff, getLen + picPack_offset);
		if (mqtt_res == MY_MQTT_ERR_SEND_FAIL)
		{
			f_close(&fl_in);
			return MY_MQTT_ERR_SEND_FAIL;
		}
		// д�����
		if (otherLen - getLen > packagelen)
		{
			q_res = EnQueue(&Q_stage, frame_p);
			if (q_res) // ����гɹ�
			{
				printf("+ Q_stage:[%3d,%3d,%3d],Q+UID:%08X,pid:%04X\r\n", Q_stage.front, Q_stage.rear, QueueLength(Q_stage), frame_p.uid, frame_p.pack_id);
				printf("* Q_resnt:[%3d,%3d,%3d],Q*UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);
			}
			else // �������������������쳣����Ϊÿһ��ͼƬ�����һ�check���У�
			{
				printf("Server error\r\n");
				return MY_MQTT_ERR_ACK_TIMEOUT;
			}
		}
		otherLen -= getLen;
		printf("otherLen:%d,crc=%0X\r\n", otherLen, pack_crc);
		pic_pack.packet_id++;
	}
	return 0;
}
/**
 * @description: ��ȡ�ط�ͼƬ������
 * @param d_path �ļ���·��
 * @param file_path �ļ�·��
 * @param pic_id ͼƬid
 * @return: 
 */
void get_resent_path(uint8_t *d_path, uint8_t *file_path, uint16_t pic_id)
{
	uint8_t fn_t[30] = {0};

	file_path[0] = 0;
	strcat((char *)file_path, d_path); // ����ԭʼԴ�ļ�·��
	strcat((char *)file_path, "/");	// Դ·����б��
	sprintf((char *)fn_t, "IMAG%04d.JPG", pic_id);
	strcat((char *)file_path, fn_t); // Ŀ��·�����ļ���
	printf("get_resent_path file name: %s\r\n", file_path);
}
/**
 * @description: 
 * @param pic_buff ͼƬ������
 * @param pic_bufflen ͼƬ����������
 * @param packagelen mqtt������
 * @return: 0  ���ͳɹ���>0 �����±�
 */
MY_MQTT_ERR check_Queue(uint8_t *pic_buff, uint32_t pic_bufflen,	// ����ͼƬ�ְ���Ҫ��buff�ռ��Լ�buffʵ�ʳ���
						uint32_t packagelen)					  // ͼƬ��Ƭ����
{
	uint16_t i;
	QElemType elem;
	// �ط�����
	//	uint8_t try_cnt = 0;  // ��������
	//	uint8_t perst_flag = 0;  // reset flag
	uint16_t pack_crc = 0; // CRC result

	// ���ṹ��
	struct pick_package pic_pack;

	// Queue status
	Status q_res;
	// MQTT status
	MY_MQTT_ERR mqtt_res;
	// ��ȡ�ļ�״̬
	FIL fl_in;
	FRESULT res;
	uint32_t getLen;			 // ��ȡ������
	uint32_t otherLen;			 // ʣ���ܳ���
	uint16_t picPack_offset = 0; // package����/ƫ����
	uint8_t file_path[40] = {0};
	// ��ȡ�ط����е�һ��Ԫ��
	GetHead(Q_resent, &elem);
	i = Q_resent.front;
	while (i != Q_resent.rear)
	{
#if (EN_log_print >= 2)
		printf("\r\n#####check_Queue####\r\n");
#endif // EN_log_print \
	// �����ͼƬ��
		if (elem.pic_id != 0)
		{
			get_resent_path((uint8_t *)"0:ARCH", file_path, elem.pic_id); //��ȡ�ļ��ĵ�ַ
			file_path[19] = 0;
			// printf("resent picture path:%s \r\n", file_path);
			res = f_open(&fl_in, (const char *)file_path, FA_OPEN_EXISTING | FA_READ);
			if (res != FR_OK)
			{
				get_resent_path((uint8_t *)"0:INBOX", file_path, elem.pic_id); //��ȡ�ļ��ĵ�ַ
				file_path[20] = 0;
				// printf("resent picture path:%s \r\n", file_path);
				res = f_open(&fl_in, (const char *)file_path, FA_OPEN_EXISTING | FA_READ);
				if (res != FR_OK)
				{
					printf("MY_MQTT_ERR_GETDATA_FAIL <PATH:%s>\r\n", file_path);
					f_close(&fl_in);
					return MY_MQTT_ERR_GETDATA_FAIL;
				}
			}
#if (EN_log_print >= 2)
			printf("resent picture path:%s \r\n", file_path);
#endif // EN_log_print
		RESENT:
			// ÿ��ͼƬΨһ,���ڸ��ݶ�ȡ�����ݸı�
			pic_pack.picture_id = elem.pic_id;
			// ��ȡ�ļ���С
			pic_pack.data_total_length = f_size(&fl_in);
			// ���㴫�����
			// pic_pack.packet_total_num = (pic_pack.data_total_length / packagelen) + 1;
			pic_pack.packet_total_num = 0XFFFF; // �ط�����������
			//	printf("f_open successfully <file_path:%s|file_size:%d|sub_num:%d|packagelen:%d>\r\n",
			//		   file_path, pic_pack.data_total_length, pic_pack.packet_total_num, packagelen);
			pic_pack.packet_id = elem.pack_id;
			if ((elem.pack_id + 1) * packagelen < pic_pack.data_total_length)
				pic_pack.data_length = packagelen;
			else
				pic_pack.data_length = pic_pack.data_total_length % packagelen;
			otherLen = pic_pack.data_length;
			// ����
			picPack_offset = mypicPack_serialize((char *)pic_buff, pic_pack);
			// ���л���ͷ
			res = f_lseek(&fl_in, packagelen * (elem.pack_id)); // ��Ŀ¼�ƶ����ʼ���ʼλ��,pack_id�Ǵ��㿪ʼ�ġ�
			if (res != FR_OK)
			{
				printf("f_lseek ERROR f_lseek ERROR f_lseek ERROR\r\n");
			}
#if (EN_log_print >= 2)
			printf("f_lseek:%d|%d\r\n", packagelen * (elem.pack_id), pic_pack.data_total_length);
#endif // EN_log_print
			f_read(&fl_in, pic_buff + picPack_offset, otherLen, &getLen);
			if (getLen != otherLen)
			{
				f_close(&fl_in);
				//myfree(SRAMIN, file_path);
				printf("MY_MQTT_ERR_GETDATA_FAIL <rc|pack:%d|%d>\r\n", getLen, packagelen);
				return MY_MQTT_ERR_GETDATA_FAIL; // ���������Ҫ����
			}
			// add crc
			pack_crc = ModBusCRC(pic_buff + picPack_offset, getLen);
#if (EN_log_print >= 2)
			printf("%0X,picPack_offset=%d,packagelen=%d\r\n", pack_crc, picPack_offset, getLen);
#endif // EN_log_print
			pic_buff[18] = pack_crc >> 8;
			pic_buff[19] = pack_crc;
			// ����ǰ����ջ�����
			UART_TCPbuff_Run(F407USART3_buffRead);
			// ��������
			//mqtt_publish_data(MY_TOPIC_PICUP, mqttbuff, mqttbufflength, pic_buff, getLen + picPack_offset);
			mqtt_res = mqtt_publish_data(MY_TOPIC_PICUP, (char *)pic_buff, getLen + picPack_offset);
			if (mqtt_res == MY_MQTT_ERR_SEND_FAIL)
			{
				f_close(&fl_in);
				printf("%0X,picPack_offset=%d,packagelen=%d\r\n", pack_crc, picPack_offset, getLen);
				return MY_MQTT_ERR_SEND_FAIL;
			}
			// д�����
			DeQueue(&Q_resent, &elem);
			printf("*-Q_resnt:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);
			//printf("---Q_resent:[%d,%d],Qin len=%d\r\n",Q_resent.front,Q_resent.rear,QueueLength(Q_resent));
			q_res = EnQueue(&Q_stage, frame_p);
			if (q_res) // ����гɹ�
			{
				printf("*+Q_stage:[%3d,%3d,%3d],Q+UID:%08X,pid:%04X\r\n", Q_stage.front, Q_stage.rear, QueueLength(Q_stage), frame_p.uid, frame_p.pack_id);
			}
			else // �������������������쳣����Ϊÿһ��ͼƬ�����һ�check���У�
			{
				f_close(&fl_in);
				printf("Server error\r\n");
				return MY_MQTT_ERR_ACK_TIMEOUT;
			}
			q_res = GetHead(Q_resent, &elem);
			if (q_res == SUCCESS && (elem.pic_id == pic_pack.picture_id))
			{
				printf("send  data\r\n");
				goto RESENT;
			}
			else
			{
				printf("close picture\r\n");
				f_close(&fl_in);
				break;
			}
		}
		// �ط���������Ϣ
		else
		{
			DeQueue(&Q_resent, &elem);
			printf("*-Q_resnt:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);
			GetHead(Q_resent, &elem);
		}
		i = (i + 1) % MAXSIZE;
	}
	return 0;
}
MY_MQTT_ERR mycheck_Queue(void)
{
	uint32_t packagelen = PACKAGE_LEN; // ͼƬ��Ƭ����
	check_Queue(MQTT_DATA_buf, MQTT_DATA_BUFF_MAXLENGTH,
				packagelen);
}
MY_MQTT_ERR mysend_picture(uint8_t *file_path, uint32_t picture_id)
{
	MY_MQTT_ERR res;
	uint32_t packagelen = PACKAGE_LEN; // ͼƬ��Ƭ����
	uint16_t timeOut = 50000;
	res = send_picture(MQTT_DATA_buf, MQTT_DATA_BUFF_MAXLENGTH,
					   file_path, picture_id, packagelen, timeOut);
	mycheck_Queue();
	return res;
}

/**
 * @description: ���л����������ݰ�ͷ
 * @param buf ��������ַ
 * @param msg_len ��Ϣ����
 * @param crc crcУ��ֵ
 * @return: 
 */
int mymes_serialize(char *buf, uint16_t msg_len, uint16_t crc)
{

	uint32_t uid = 0;
	// ��ʼ�ֽڣ��̶�Ϊ0xa590
	buf[0] = 0xa5;
	buf[1] = 0x96;

	uid = Frame_UID_get();
	buf[2] = uid >> 24;
	buf[3] = uid >> 16;
	buf[4] = uid >> 8;
	buf[5] = uid;

	buf[6] = msg_len >> 8;
	buf[7] = msg_len;

	buf[8] = crc >> 8;
	buf[9] = crc;
	buf[10] = 0;

	frame_p.uid = uid;
	frame_p.pic_id = 0;
	frame_p.pack_id = 0;
	return 10;
}
/**
 * @description: ��������
 * @param msg ���ݵ�ַ
 * @return: 0 ���ͳɹ���>0 ����ʧ��
 */
MY_MQTT_ERR mysend_data(char *msg)
{
	uint16_t len = 0;
	uint16_t crc = 0;
	uint16_t offset = 0;
	// ɨ�軺�����Ƿ��������
	UART_TCPbuff_Run(F407USART3_buffRead);
	// ���
	len = strlen(msg);
	crc = ModBusCRC((uint8_t *)msg, len);
	offset = mymes_serialize(MQTT_DATA_buf, len, crc);
	strcat(MQTT_DATA_buf, msg);
	// printf("%d,%d####\r\n",len+offset,strlen(MQTT_DATA_buf));
	// ��������
	mqtt_publish_data(MY_TOPIC_MSGUP, MQTT_DATA_buf, len+offset);
	// д�����
	EnQueue(&Q_stage, frame_p);
	printf("+ Q_stage:[%3d,%3d,%3d],QOUID:%08X,pid:%04X\r\n", Q_stage.front, Q_stage.rear, QueueLength(Q_stage), frame_p.uid, frame_p.pack_id);
	printf("* Q_resnt:[%3d,%3d,%3d],QOUID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);
	return 0;
}
