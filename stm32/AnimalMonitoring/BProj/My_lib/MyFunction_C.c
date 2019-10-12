#include "MyFunction_C.h"
#include "rng.h"
/*
���ܣ������ַ�������Ч�ַ�����
�������ַ����׵�ַ
����ֵ�������ַ�ʵ���ַ�����
*/
uint16_t stringLength(const uint8_t *string)
{
	uint16_t i=0;
	while(*(string+i))i++;
	return i;
}


/*
���ܣ���ָ��������������ָ�����ַ�
������
����ֵ������-1��ʾ����ʧ�� ���򷵻����������ַ�����ʼλ��
*/
int16_t MystringSeek(uint8_t *data,uint16_t datalen,uint8_t *str,uint16_t strlen)
{
	uint8_t flag=0;
	int16_t i=0,j=0;
	for(i=0;i<=datalen-strlen;i++)
	{
		if(data[i]!=str[0])continue;
		//
		for(j=0,flag=0;j<strlen;j++)
		{
			if(data[i+j]!=str[j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)return i;
	}
	return -1;
}



/*
���ܣ��ֽ����鸴��
������*obj��Ŀ�����ݵ�ַ
			*src��Դ���ݵ�ַ
			length��Դ���ݳ���
����ֵ����
*/
void bytesCopy(uint8_t *obj,uint8_t *src,uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++)
	{
		obj[i]=src[i];
	}
}
/*
���ܣ�����ֽ�����
������*obj��Ŀ�����ݵ�ַ
			length��Դ���ݳ���
����ֵ����
*/
void bytesCleare(uint8_t *obj,uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++)
	{
		obj[i]=0;
	}
}

uint32_t stringtoNum(char* str)
{
	uint32_t num = 0;
	uint8_t  cnt = 0;
	while (str[cnt]>='0' && str[cnt]<='9')
	{
		num *= 10;
		num += (str[cnt] - '0');
		cnt++;
	}
	return num;
}

uint8_t locate_character(char* str, char ch)
{
	uint8_t i=0;
	#define MAX_LOCATE_LEN 20
	while(i <MAX_LOCATE_LEN)
	{
		if(str[i] == ch)
			return i;
		i++;
	}
	return 0;
}

uint8_t BytesSum(uint8_t *data,uint16_t length)
{
	uint16_t i,sum=0;
	for(i=0;i<length;i++)
	{
		sum+=data[i];
	}
	return sum;
}
uint8_t BytesSumNot(uint8_t *data,uint16_t length)
{
	uint16_t i,sum=0;
	for(i=0;i<length;i++)
	{
		sum+=data[i];
	}
	sum=~sum;
	return sum;
}
//��һ��ָ�����ȵ������У�ɾ��һ������ɾ��������������ǰ�ƶ�
uint8_t BytesDeleteOnenumber(uint8_t *Bytes,uint8_t BytesLength,uint8_t number)
{
	uint8_t i=number;
	if(number>=BytesLength||BytesLength==0)return 0;
	for(;i<BytesLength-1;i++)
	{
		Bytes[i]=Bytes[i+1];
	}
	return 1;
}
//��һ��������,��֪��BytesLength������,������numer�����һ������,������������ƶ�
uint8_t BytesAddOnenumber(uint8_t *Bytes,uint8_t BytesLength,uint8_t MaxLength,uint8_t number,uint8_t value)
{
	uint8_t i=BytesLength;
	if(number>BytesLength||BytesLength>=MaxLength)return 0;
	if(number==BytesLength)
	{
		Bytes[number]=value;
	}
	else 
	{
		for(i=BytesLength;i>number;i--)
		{
			Bytes[i]=Bytes[i-1];
		}	
		Bytes[number]=value;
	}
	return 1;
}

//////////////////////CRC
#define POLY 0xa001
unsigned short ModBusCRC(unsigned char *buf, unsigned int lenth)
{
	int i, j;
	unsigned short crc;
	for (i = 0, crc = 0xffff; i < lenth; i++)
	{
		crc ^= buf[i];
		//printf("%0X ",buf[i]);
		for (j = 0; j < 8; j++)
		{
			if (crc & 0x01)
				crc = (crc >> 1) ^ POLY;
			else
				crc >>= 1;
		}
	}
	return crc;
}
//////////////////////UID
u32 Frame_UID_get(void)
{
	u32 uid = 0;
	uid += (*(volatile uint32_t *)0x1fff7a10);
	uid += (*(volatile uint32_t *)0x1fff7a14);
	uid += (*(volatile uint32_t *)0x1fff7a18);

	uid += RTC->TR;
	uid += RTC_GetSubSecond();

	uid += rngGetRandomNum();
	return uid;
}
//////////////////////
