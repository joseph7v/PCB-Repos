#ifndef _MYFUNCTION_C_H_
#define _MYFUNCTION_C_H_


#ifndef uint64
#define uint64 unsigned long long
#endif

#ifndef int64
#define int64 signed long long
#endif

#ifndef uint32
#define uint32 unsigned int 
#endif

#ifndef int32
#define int32 signed int 
#endif


#ifndef uint16
#define uint16 unsigned short
#endif

#ifndef uint8
#define uint8 unsigned char
#endif

#ifndef int16
#define int16 short
#endif

#ifndef int8
#define int8 char
#endif




/*
���ܣ������ַ�������Ч�ַ�����
�������ַ����׵�ַ
����ֵ�������ַ�ʵ���ַ�����
*/
uint16 stringLength(const uint8 *string);

int16 MystringSeek(uint8 *data,uint16 datalen,uint8 *str,uint16 strlen);

/*
���ܣ��ֽ����鸴��
������*obj��Ŀ�����ݵ�ַ
			*src��Դ���ݵ�ַ
			length��Դ���ݳ���
����ֵ����
*/
void bytesCopy(uint8 *obj,uint8 *src,uint16 length);


void bytesCleare(uint8 *obj,uint16 length);


uint8 BytesSum(uint8 *data,uint16 length);
uint8 BytesSumNot(uint8 *data,uint16 length);


uint8 BytesDeleteOnenumber(uint8 *Bytes,uint8 BytesLength,uint8 number);
uint8 BytesAddOnenumber(uint8 *Bytes,uint8 BytesLength,uint8 MaxLength,uint8 number,uint8 value);







#endif
