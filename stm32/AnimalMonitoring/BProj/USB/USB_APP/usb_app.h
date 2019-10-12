#ifndef __USB_APP_H
#define __USB_APP_H	 
#include "sys.h"
#include "usbh_msc_core.h"
//USB APP���ƽṹ��
typedef struct 
{
	u8 bDeviceState; 	//USB״̬���
						//bit0:��ʾ����������SD��д������
						//bit1:��ʾ��������SD����������
						//bit2:SD��д���ݴ����־λ
						//bit3:SD�������ݴ����־λ
						//bit4:1,��ʾ��������ѯ����(�������ӻ�������)
						//bit5:����.
						//bit6:1,��ʾUSB���豸����;0,��ʾû���豸����
						//bit7:1,��ʾUSB�Ѿ�����;0,��ʾUSBû������.
	u8 hdevclass; 		//USB HOST�豸����
						//1,U��
}_usb_app;

extern _usb_app usbx;
extern USBH_HOST  USB_Host; 
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;
 
void usbapp_mode_stop(void);
void usbapp_mode_set(void);

void usbapp_init(void);			//��ʼ��USB
void usbapp_pulling(void);
void usbapp_mode_stop(void);
void usbapp_mode_set(void);
u8 usbapp_user_app(void);
// USBʶ��״̬
u8 usbConnectSwitchGet(void);
void usbConnectSwitchSet(u8 state);
u8 usbConnectStateGet(void);
void usbConnectStateSet(u8 state);
u8 waitUsbConnectFlag(u16 timeout);
u8 waitUsbDisonnectFlag(u16 timeout);


#endif

















