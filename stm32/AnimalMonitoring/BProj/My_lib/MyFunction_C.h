#ifndef _MYFUNCTION_C_H_
#define _MYFUNCTION_C_H_
#include "sys.h"
/*
���ܣ������ַ�������Ч�ַ�����
�������ַ����׵�ַ
����ֵ�������ַ�ʵ���ַ�����
*/
uint16_t stringLength(const uint8_t *string);

int16_t MystringSeek(uint8_t *data,uint16_t datalen,uint8_t *str,uint16_t strlen);

/*
���ܣ��ֽ����鸴��
������*obj��Ŀ�����ݵ�ַ
			*src��Դ���ݵ�ַ
			length��Դ���ݳ���
����ֵ����
*/
void bytesCopy(uint8_t *obj,uint8_t *src,uint16_t length);


void bytesCleare(uint8_t *obj,uint16_t length);


uint8_t BytesSum(uint8_t *data,uint16_t length);
uint8_t BytesSumNot(uint8_t *data,uint16_t length);


uint8_t BytesDeleteOnenumber(uint8_t *Bytes,uint8_t BytesLength,uint8_t number);
uint8_t BytesAddOnenumber(uint8_t *Bytes,uint8_t BytesLength,uint8_t MaxLength,uint8_t number,uint8_t value);


uint32_t stringtoNum(char* str);
uint8_t locate_character(char* str, char ch);

u32 Frame_UID_get(void);
unsigned short ModBusCRC(unsigned char *buf, unsigned int lenth);

#endif
