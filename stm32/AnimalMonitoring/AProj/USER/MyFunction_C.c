#include "MyFunction_C.h"


/*
���ܣ������ַ�������Ч�ַ�����
�������ַ����׵�ַ
����ֵ�������ַ�ʵ���ַ�����
*/
uint16 stringLength(const uint8 *string)
{
	uint16 i=0;
	while(*(string+i))i++;
	return i;
}


/*
���ܣ���ָ��������������ָ�����ַ�
������
����ֵ������-1��ʾ����ʧ�� ���򷵻����������ַ�����ʼλ��
*/
int16 MystringSeek(uint8 *data,uint16 datalen,uint8 *str,uint16 strlen)
{
	uint8 flag=0;
	int16 i=0,j=0;
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
void bytesCopy(uint8 *obj,uint8 *src,uint16 length)
{
	uint16 i;
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
void bytesCleare(uint8 *obj,uint16 length)
{
	uint16 i;
	for(i=0;i<length;i++)
	{
		obj[i]=0;
	}
}



uint8 BytesSum(uint8 *data,uint16 length)
{
	uint16 i,sum=0;
	for(i=0;i<length;i++)
	{
		sum+=data[i];
	}
	return sum;
}
uint8 BytesSumNot(uint8 *data,uint16 length)
{
	uint16 i,sum=0;
	for(i=0;i<length;i++)
	{
		sum+=data[i];
	}
	sum=~sum;
	return sum;
}
//��һ��ָ�����ȵ������У�ɾ��һ������ɾ��������������ǰ�ƶ�
uint8 BytesDeleteOnenumber(uint8 *Bytes,uint8 BytesLength,uint8 number)
{
	uint8 i=number;
	if(number>=BytesLength||BytesLength==0)return 0;
	for(;i<BytesLength-1;i++)
	{
		Bytes[i]=Bytes[i+1];
	}
	return 1;
}
//��һ��������,��֪��BytesLength������,������numer�����һ������,������������ƶ�
uint8 BytesAddOnenumber(uint8 *Bytes,uint8 BytesLength,uint8 MaxLength,uint8 number,uint8 value)
{
	uint8 i=BytesLength;
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
