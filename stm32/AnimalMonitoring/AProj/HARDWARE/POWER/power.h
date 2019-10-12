#ifndef __POWER_H
#define __POWER_H
#include "sys.h"
/*
- DEVC_CRL  -PC0
- GPS_CRL   -PC1
- USB_CRL   -PC2
 */

//���ƶ˿ڶ���
#define VCC_DVC   PBout(13)	
#define VCC_GPS   PBout(14)	
#define VCC_USB   PBout(15)	

#define VCC_WIFI  PCout(4)	
#define VCC_4G    PCout(5)	

void Power_Ctrl_Init(void);//��ʼ��	

void USB_CONNECT_ON(void);
void USB_CONNECT_OFF(void);


// ����˿ڳ�ʼ��
#define USB_CNT		PDout(0)	// 8TX-���ڲ�Ϊ3������USB
#define USB_PHO		PDout(1)	// 7GND���ڲ�Ϊ5����������

void Cam_Crtl_Init(void);
void USB_Connecting(u8 ctrl);
void USB_Photograph(void);

u8 openUSB(void);  // ��USB
u8 closeUSB(void);  // �ر�USB
#endif
