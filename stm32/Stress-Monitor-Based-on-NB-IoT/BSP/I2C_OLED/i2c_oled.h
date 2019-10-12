/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */
#ifndef __I2C_OLED_H
#define __I2C_OLED_H

#include "sys.h"
////OLED��Ϣ����
//typedef struct{	
//	u8 olfl_line1[3];//����Ƿ�����OL����FL
//	u8 cmd_title[5];//"CMD:"
//	u8 msg_title[5];//"MSG:"
//	u8 msg_line1[17];//��1�е���Ϣ���Ƿ�overtime
//	u8 cmd_line2[22];//���͵�AT����
//	u8 msg_line4[22];//���յ�����Ļ��4����Ϣ,ʵ�ʵ�1����Ϣ
//	u8 msg_line5[22];
//	u8 msg_line6[22];
//	u8 msg_line7[22];
//	u8 msg_line8[22];//���ѹ�����ݺ�AT�յ���msg
//	u16 OLED_REFRESHING_STAT;//ÿһλ��ʾһ��ģ���Ƿ�Ҫ���£�1����Ҫ���£�0������Ҫ����
//}OLED_TEMP;
//extern OLED_TEMP oled_temp;

//#define OLFL_LINE1_BIT_CS	0x0001
//#define CMD_TITLE_BIT_CS	0x0001<<1
//#define MSG_TITILE_BIT_CS	0x0001<<2
//#define MSG_LINE1_BIT_CS	0x0001<<3
//#define CMD_LINE2_BIT_CS	0x0001<<4
//#define MSG_LINE4_BIT_CS	0x0001<<5
//#define MSG_LINE5_BIT_CS	0x0001<<6
//#define MSG_LINE6_BIT_CS	0x0001<<7
//#define MSG_LINE7_BIT_CS	0x0001<<8
//#define MSG_LINE8_BIT_CS	0x0001<<9
//#define CLEAR_SCREEN_BIT_CS	0x0001<<10

//#define ClearScreenFlagSet()		{oled_temp.OLED_REFRESHING_STAT |=  CLEAR_SCREEN_BIT_CS}  //��������flag�������Զ�����



#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
#define CMD_START_X       30    //OLED��������ʾ��λ��
#define CMD_START_Y       1
#define MSG_START_X		  30	//OLED��AT��Ϣ��ʼ��x
#define RCV_START_X       12     //��Ϣ��OLED�Ͽ�ʼ��λ��
#define RCV_START_X_NoNewLine       30     //��Ϣ��OLED�Ͽ�ʼ��λ��
#define LINE_FEED         1//��Ϣ���п�ʼ��ʾ
#define NO_LINE_FEED      0//��Ϣ��������ʾ
#define RCV_START_Y       3 
#define OLED_X_MAX        127
#define OLED_Y_MAX        7
#define SGN_START_X       18
#define SGN_START_Y       0

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
//#define OLED_MAX_TRIALS 300   //��������ô���HAL_I2C_IsDeviceReady���豸׼�����
//#define I2C_XFER_TIMEOUT_MAX    300//IIC���ȴ���ʱ��

void I2C_OLED_Init(void);//��ʼ��IIC

void I2C_WriteByte(u8 addr,u8 data);
void WriteCmd(u8 I2C_Command);//д����
void WriteDat(u8 I2C_Data);//д����
void OLED_Init(void);


void OLED_SetPos(u8 x, u8 y); //������ʼ������
void OLED_Fill(u8 fill_Data);//ȫ�����
void OLED_CLS(void);//����
void OLED_ON(void);//��OLED
void OLED_OFF(void);//�ر�OLED
void OLED_ShowStr(u8 x,u8 y,u8 ch[],u8 TextSize);
void OLED_ShowCN(u8 x,u8 y,u8 N);//��ʾ����
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 TextSize);//��ʾ����


//�������ʾ����
void MsgDisp(u8 linefeed);//��ʾ��Ϣ
void InitDisp(void);//��ʾ��ʼ��
void cmdDisp(u8 * cmd);//��ʾcmd
void sgnDisp(u8 * sgn);//��ʾ��Ϣ������overtime���ź�ǿ�ȵ�
void MsgSentDisp(u8 * msg);//��ʾ�Ѿ����͵�����
void intMsgSentDisp(u16 intmsg);//����int�����ݵ���ʾ
void CMD_GOT_STA_Disp(void);//��ʾ���յ�����Ϣ״̬
void CMD_GOT_Disp(void);//��ʾ���յ�������
void BrdStaDisp(void);//���°��ӵ�״̬OL����FL
void Error_Handler_Disp(char * file,int line);//��ʾ������ļ�����������Error_Handler��ʹ��
#endif

