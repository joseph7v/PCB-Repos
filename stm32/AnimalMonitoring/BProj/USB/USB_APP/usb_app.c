#include "usb_app.h"
#include "usbh_usr.h"
#include "delay.h"
#include "power.h"
#include "string.h"
//USB HOST
USBH_HOST USB_Host;
USB_OTG_CORE_HANDLE USB_OTG_Core;
_usb_app usbx; //USB APP������

static vu8 usbConnectFlag = 0; // ���USB����״̬
u8 usbConnectStateGet(void)
{
	return usbConnectFlag;
}
void usbConnectStateSet(u8 state)
{
	usbConnectFlag = state;
}

static vu8 usbConnectSwitch = 0; // ��USB����
u8 usbConnectSwitchGet(void)
{
	return usbConnectSwitch;
}
void usbConnectSwitchSet(u8 state)
{
	usbConnectSwitch = state;
}
/**
 * @description: �ȴ�USB���ӳɹ�
 * @param timeout �ȴ�ʱ��
 * @return: ʧ�ܳ�ʱ����0���ɹ�����1
 */
u8 waitUsbConnectFlag(u16 timeout)
{
	u16 temp = 0;
	while (!usbConnectStateGet())
	{
		// usbapp_pulling();
		delay_ms(5);
		temp += 5;
		if (temp >= timeout)
		{
			return 0;
		}
	}
	return 1;
}
/**
 * @description: �ȴ�USB�Ͽ�
 * @param timeout �ȴ�ʱ��
 * @return: ʧ�ܳ�ʱ����0���ɹ�����1
 */
u8 waitUsbDisonnectFlag(u16 timeout)
{
	u16 temp = 0;
	while (usbConnectStateGet() != 0)
	{
		delay_ms(5);
		temp += 1;
		if (temp >= timeout)
		{
			return 0;
		}
	}
	return 1;
}
/**
 * @description: USB OTG �жϷ�����,�����ʹ�ôӻ�ģʽ
 * @param none
 * @return: none
 */
void OTG_FS_IRQHandler(void)
{
	OSIntEnter();
	USBH_OTG_ISR_Handler(&USB_OTG_Core);
	OSIntExit();
}
/**
 * @description: usb״̬��־��ʼ��
 * @param {type} 
 * @return: 
 */
//��ʼ��USB
void usbapp_init(void)
{
	usbx.bDeviceState = 0;
	usbx.hdevclass = 0;
	usbConnectStateSet(0);  // ���ӳɹ���־
	usbConnectSwitchSet(0);
}
/**
 * @description: USB��ѯ����,���������Եı�����
 * @param void
 * @return: void
 */
void usbapp_pulling(void)
{
	if (HCD_IsDeviceConnected(&USB_OTG_Core) == 0 || usbx.hdevclass != 1) // U������ʧ����,��Ҫ���´���USB����
	{
		USBH_Process(&USB_OTG_Core, &USB_Host); // ����USB����
		if (USBH_Check_EnumeDead(&USB_Host))	// ���USB HOST �Ƿ�������?������,�����³�ʼ��
		{
			printf("usbapp_mode_set\r\n");
			usbapp_mode_set(); // ����
		}
	}
}
/**
 * @description: USB������ǰ����ģʽ
 * @param none
 * @return: none
 */
void usbapp_mode_stop(void)
{		
	RCC->AHB2RSTR|=1<<7; // USB OTG FS ��λ
	delay_ms(5);
	RCC->AHB2RSTR&=~(1<<7); // ��λ����
	memset(&USB_OTG_Core,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	usbx.bDeviceState=0;
	usbx.hdevclass=0; 
	usbConnectStateSet(0);  // ����״̬����
}
/**
 * @description: ����USB����ģʽΪUSB HOST MSCģʽ(Ĭ��ģʽ,��U��)
 * @param void
 * @return: void
 */
void usbapp_mode_set(void)
{
	usbapp_mode_stop();  // ��λUSB	
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  

}
/**
 * @description: �û����Գ���
 * @param void
 * @return: USB��ȡ״̬
 */
u8 usbapp_user_app(void)
{
	u32 total, free;
	u8 res = 0;

	res = exf_getfree("1:", &total, &free);
	if (res == 0)
	{
		printf("U Disk Total Size:%d MB\r\n", total);
		printf("U Disk  Free Size:%d MB\r\n", free);
	}
	else
	{
		printf("01:Failed to get capacity\r\n");
	}
	return res;
}
