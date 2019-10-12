#ifndef __EC25_H__
#define __EC25_H__	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	   
//********************************************************************************
//�޸�˵��
//V1.1 20140810
//1,����__ec25dev�ṹ��
//2,�޸�ec25_send_cmd����,������Ԥ�ڽ������
////////////////////////////////////////////////////////////////////////////////// 	
 
#define EC25_MAX_NEWMSG	10		//���10������Ϣ

// 4G CTRL
#define DTR_4G   	PDout(13)	// �͵�ƽ����ģ��
#define DISABLE_4G  PDout(5)	// �Ƿ�ʹ�÷���ģʽ
#define PRESET_4G   PDout(6)	// ����ģ�飬�͵�ƽʱ��>460ms

typedef enum 
{
	EC25_ERR_NONE=0,  //û�д���
	
	EC25_ERR_AT,  	//AT1
	
	EC25_ERR_CSQ,  //CSQ2
	
	EC25_ERR_CPIN,  //ѯ��SIM��3
	
	EC25_ERR_CREG,  //CS Service4
	
	EC25_ERR_CGREG,  //PS Service5
	
	EC25_ERR_CEREG,  //PS Service6
	
	EC25_ERR_QICSGP,  //����PDP����7
	
	EC25_ERR_QIOPEN,  //������8
	EC25_ERR_DEACT,  //������
}MY_EC25_ERR;

typedef struct 
{							  
 	u8 status;		//EC25״̬
					//bit7:0,û���ҵ�ģ��;1,�ҵ�ģ����,csq
					//bit6:0,SIM��������;1,SIM������,cpin
					//bit5:0,δ�����Ӫ������;1,�ѻ����Ӫ������
					//bit4:0,�й��ƶ�;1,�й���ͨ
					//bit3:0:����
					//bit2:0:����
					//bit1:0:����
					//bit0:0:����
	
	u8 mode;		//��ǰ����ģʽ
					//0,��������ģʽ/����ģʽ/����ģʽ
					//1,������
					//2,ͨ����
					//3,������Ӧ��
	
	vu8 cmdon;		//����Ƿ���ָ���ڷ��͵ȴ�״̬
					//0,û��ָ���ڵȴ���Ӧ
					//1,��ָ���ڵȴ���Ӧ
	
	u8 csq;			//�ź�����
	
	vu8 newmsg;		//����Ϣ����,0,û������Ϣ;����,����Ϣ����
	u8 newmsgindex[EC25_MAX_NEWMSG];//�¶�����SIM���ڵ�����,���¼EC25_MAX_NEWMSG���¶���
	u8 incallnum[20];//������뻺����,�20λ
}__ec25dev; 

extern __ec25dev ec25dev;	//ec25������
  
#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8		//�ߵ��ֽڽ����궨��

void ec25_Init(void);
void ec25_wakeup(int sw);
void ec25_reset(int ms);

u8* ec25_check_cmd(u8 *str);
u8 ec25_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 ec25_send_cmdEX(u8 *cmd,u8 *ack,u16 waittime);
void ec25_cmd_over(void);

u8 ec25_chr2hex(u8 chr);
u8 ec25_hex2chr(u8 hex);
u8 ec25_soft_start(void);
u8 ec25_dec(void);
void ec25_status_check(void);


u8 ec25_TCPConnect(u8 *ip,u16 port);
void ec25_TCPDisConnect(void);

u8 ec25_TCPSendData(u8 *data,u16 length);
u8 ec25_TCPSendData1024(u8 *data,u16 length);
	
void ec25_TCPReadData(u8 *data,u16 length);


#endif





