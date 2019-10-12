/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "algorithm.h"
#include "math.h"
#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C_OLED.h"
#include "timer.h"

/*******************************************************************************
* ������ : Num2Str
* ����   : ����ת��Ϊ�ַ���,�ı�����ĵ�1~6λ����6λ
		Ĭ��ͨ��λ��Ϊ�������ֺ͵�λ��+001232N
* ����   :
* ���   :
* ����   :
* ע��   :
*******************************************************************************/
void Num2Str(char *str, unsigned long num)
{
	unsigned char index[]="0123456789";
	char i = 6;
    do
    {
        str[i--] = index[num%10];
        num /= 10;
    }while(i);
	
}

/*******************************************************************************
* ������ : Num2Str
* ����   : ����ת��Ϊ�ַ���,��123����>"123"
* ����   :
* ���   :
* ����   :
* ע��   :
*******************************************************************************/
void Dnum2Str(unsigned char * str,unsigned int num,unsigned char digit)
{
	unsigned char index[]="0123456789";
	char i = digit;
    do
    {
        str[--i] = index[num%10];
        num /= 10;
    }while(i);
}
/*******************************************************************************
* ������ : Num2Str
* ����   : ����ת��Ϊhex�ַ���,��ʮ����24 = ʮ������18 ��ת���ַ���"18"
* ����   :
* ���   :
* ����   :
* ע��   :
*******************************************************************************/
void Num2HexStr(char *str, unsigned long num, unsigned char digit)
{
	unsigned char index[]="0123456789ABCDEF";
	char i = digit;
    do
    {
        str[--i] = index[num%16];
        num /= 16;
    }while(i);

}
/*******************************************************************************
* ������ : Hex2Str
* ����   : 16������ת�����ַ���������AT+NSOST����
* ����   : ����: pSrc - Դ����ָ��
* ���   : ���: pDst - Ŀ���ַ���ָ��
* ����   :
* ע��   : �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"
*******************************************************************************/
void Hex2Str(char* pSrc, char* pDst, unsigned int nSrcLength)
{
	int i = 0;
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�

	for (i = 0; i < nSrcLength; i++)
	{
		*pDst++ = tab[*pSrc >> 4];		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';
}

/*******************************************************************************
* ������ : AsciiToHex
* ����   : ASCALL��ת�����ַ�
* ����   : unsigned char cNum  ASC-II�ַ���
* ���   : unsigned char HEX��
* ����   :
* ע��   : �磺{'A'} --> 0xA
*******************************************************************************/
unsigned char AsciiToHex(unsigned char cNum)
{
	if(cNum>='0'&&cNum<='9')
	{
		cNum -= '0';
	}
	else if(cNum>='A'&&cNum<='F')
	{
		cNum -= 'A';
		cNum += 10;
	}
	else if(cNum>='a'&&cNum<='f')
	{
		cNum -= 'a';
		cNum += 10;
	}
	return cNum;
}

/*******************************************************************************
* ������ : StrToHex
* ����   : �ַ���ת��hex
* ����   : char *ptr�ַ��洢����unsigned short len���ݳ���
* ���   :
* ����   :
* ע��   : �磺"C8329BFD0E01" -->  {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
*******************************************************************************/
void StrToHex(char *ptr, unsigned short len)
{
	unsigned char n1  = 0;
	unsigned char n2  = 0;
	unsigned char num = 0;
	unsigned short i = 0;
	unsigned short index = 0;

	for(i=0; i<len; i++)
	{
		index = i << 1;				//index=i*2
		n1 = AsciiToHex(ptr[index]);
		n2 = AsciiToHex(ptr[index+1]);
		num = (n1<<4) + n2;
		ptr[i] = num;
	}
}

/*******************************************************************************
* ������ : Str2Hex
* ����   : �ַ���ת��hex
* ����   : �ַ���str��ת����洢������hexstr
* ���   :
* ����   :
* ע��   : �磺"A" -->  {43}
str = +002321N

hex = 2b3030323332314e
*******************************************************************************/
void Str2HexStr(char * Str,char * HexStr)
{
    int i =0;
    u8 dec;
    char * p = HexStr;
    u8 hex = 0;
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�
	
    for(i = 0;Str[i];i++)
    {
        dec = *(Str+i);//ʮ���Ʊ�ʾһ��ACCII
        hex = dec/16;
		*p = tab[hex];
		p++;
        hex = dec%16;
		*p = tab[hex];
		p++;
    }
    *p = 0;//���һλ��ֵ0
}
/*******************************************************************************
* ������ : HexStr2Str
* ����   : hexת��Ϊ�ַ���
* ����   : hexstr��ת����洢�ַ����Ŀռ�
* ���   :�����Ƿ���ϸ�ʽ0�������ϻ���λ����0��>0��λ��
* ����   :
* ע��   : �磺"A" -->  {43}

*******************************************************************************/
u16 HexStr2Str(char * HexStr,char * Str)
{
    u16 i =0;
	char * p = Str;
	
    for(i = 0;HexStr[i]&&HexStr[i+1];i+=2,p++)
    {
		//����λ
		if((HexStr[i]<='9')&&(HexStr[i]>='0'))
		{
			*p = (HexStr[i]-'0')*16;
		}
		else if((HexStr[i]<='F')&&(HexStr[i]>='A'))
		{
			*p = (HexStr[i]-'A'+10)*16;
		}
		else if((HexStr[i]<='f')&&(HexStr[i]>='a'))
		{
			*p = (HexStr[i]-'a'+10)*16;
		}
		else//�����ϸ�ʽ����Ϊ���ܳ����˳���0-9,a-f����A-F֮�������
			return 0;
		
		//����λ
		if((HexStr[i+1]<='9')&&(HexStr[i+1]>='0'))
		{
			*p += (HexStr[i+1]-'0');
		}
		else if((HexStr[i+1]<='F')&&(HexStr[i+1]>='A'))
		{
			*p += (HexStr[i+1]-'A'+10);
		}
		else if((HexStr[i+1]<='f')&&(HexStr[i+1]>='a'))
		{
			*p += (HexStr[i+1]-'a'+10);
		}
		else//�����ϸ�ʽ����Ϊ���ܳ����˳���0-9,a-f����A-F֮�������
			return 0;
    }
    *p = 0;//���һλ��ֵ0
	return i/2;//�������ݵ�λ��
}
//*******************************************************
//�������ƣ�u8 DataPrc(u8 *buff,u8 *ch)
//�����������������
//�������ã�������ת����ɺ�����ݣ���ָ��Աȣ��ó���Ӧ�Ĵ�ʩ
//���ز�����ָ��������
//*******************************************************
u8 StrCmp(u8 *buff,u8 *ch)
{
	u16 i,j;
	for(i = 0,j = 0;;i++,j++)
	{
		if(*(buff+i) != *(ch+j))//��������
		{
			return 0;//��ͬ
		}
		if(*(ch+j) == '\0'||*(buff+i)=='\0')//����������˲ο����ݵ�ĩβ
		{
			return 1;//��ͬ
		}
	}
}
//*******************************************************
//�������ƣ�u16 Lenstr2Lennum(u8 lenstr[])
//������������ַ���ʽ�洢������
//�������ã����ַ�����ת��Ϊu16���֣����5���
//���ز�����ת���������
//*******************************************************
u16 Lenstr2Lennum(u8 lenstr[])
{
    u8 i = 0;
    u16 num = 0;
    while((lenstr[i]<='9')&&(lenstr[i]>='0'))
    {
        num += (lenstr[i] - '0')*(mypow(10,i));
        i++;
    }
    return num;
}
//*******************************************************
//�������ƣ�u8 CMD_Anls(u8 cmd[])
//�����������������
//�������ã�������ת����ɺ�����ݣ���ָ��Աȣ��ó���Ӧ�Ĵ�ʩ
//���ز����������ݵ�״̬

//���õ�ʱ����Ҫ��u8���ص����ݸ�CDM_GOT.status��ֵ
//����
/*
CMD_GOT_NUM_0 1//û������
CMD_GOT_TOO_LONG 2//û������
CMD_GOT_FORMAT_ERROR 3 //���ݸ�ʽ����
CMD_GOT_OK 4 //���ݽ��ճɹ�
*/
//*******************************************************
u8 CMD_Anls(u8 cmd[])
{
    u16 i = CMD_GOT_START_INDEX;//�ӵڶ�����ʼ��������Ϊǰ������\r\n
    u16 j = 0;//�ṹ���е��±����
    u16 data_end_i = 0;//���ݽ������±�
    u8 len_str[CMD_MAXNUM_STR+1] = {0};//Ĭ���ǳ���0,�ֱ��ʾ��λʮλ��λ
    if((cmd[i]>'9')||(cmd[i]<='0'))//�����ͷ��������˵��������
        return CMD_GOT_NUM_0;//1����ʾ������Ϣ��û������
    while(cmd[i]!=',')//������Ϊ�ָ����Ⱥ�����
    {
        if(i>CMD_GOT_START_INDEX+CMD_MAXNUM_STR-1)//�±����4��˵������Ҫ���
            return CMD_GOT_TOO_LONG;//2����ʾ���ݹ���
        if(cmd[i]>'9'||cmd[i]<'0')//����ڶ���֮ǰ�����˳�������֮�������
            return CMD_GOT_FORMAT_ERROR;//���ݸ�ʽ����
        len_str[i-CMD_GOT_START_INDEX] = cmd[i];
        i++;
    }
    cmdgot.len = Lenstr2Lennum(len_str);//���ַ���ת��Ϊ����
    i++;//����','
    data_end_i = i + cmdgot.len*2-1;//ָ���������һ��byte
    while(cmd[i]!='\r')//��û������\rʱ
    {
        if(((cmd[i]>='0'&&cmd[i]<='9')||
           (cmd[i]>='A'&&cmd[i]<='F')||
           (cmd[i]>='a'&&cmd[i]<='f'))&&
           (i <= data_end_i))
            cmdgot.lastcmd[j++] = cmd[i++];//����
        else
            return CMD_GOT_FORMAT_ERROR;//���ݸ�ʽ����
    }
    cmdgot.lastcmd[j] = 0;//���һλ��ֵ0
    return CMD_GOT_OK;
}

//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//m^n����
float mypow_float(float m,u8 n)
{
	float result=1.0;	 
	while(n--)result*=m;    
	return result;
}

//u16��������
void u16sortup(u16 * ain,u16 len)
{
    u16 i,j;
    u16 t;
    for(i=0;i<len-1;i++)//n�����������ܹ�ɨ��len-1��
    {
        for(j=0;j<len-i-1;j++)//ÿһ��ɨ�赽a[n-i-2]��a[n-i-1]�Ƚ�Ϊֹ����
        {
            if(ain[j]>ain[j+1])//��һλ����ǰһλ��С�Ļ����ͽ�����������λ�ã�����
            {
               t=ain[j+1];
               ain[j+1]=ain[j];
               ain[j]=t;
            }
        }
    }
}

