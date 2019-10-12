#include "usbh_usr.h"
#include "usb_app.h"

#include "ff.h"
#include "usart.h"
#include "delay.h"
#include <string.h>
#include "power.h"
//////////////////////////////////////////////////////////////////////////////////


// extern USBH_HOST USB_Host;
// extern USB_OTG_CORE_HANDLE USB_OTG_Core;
// _usb_app usbx; //USB APP������

// vu8 usb_state = 0; // �ж�U���Ƿ���ȷ����
// unsigned char usb_state_get()
// {
// 	return usb_state;
// }
// void usb_state_set(unsigned char state)
// {
// 	usb_state = state;
// }
// //�ȴ�һ�����ù���������֣�ʧ�ܳ�ʱ����0���ɹ�����1
// u8 WaitDiscnctUSBflag(u16 timeout)
// {
// 	OS_ERR err;
// 	static u16 time = 0;
// 	while (usb_state_get())
// 	{
// 		OSTimeDly(1, OS_OPT_TIME_DLY, &err);
// 		time += 1;
// 		if (time >= timeout)
// 		{
// 			time = 0;
// 			return 0;
// 		}
// 	}
// 	time = 0;
// 	return 1;
// }

// vu8 usb_cflag = 0;
// unsigned char usb_cflag_get()
// {
// 	return usb_cflag;
// }
// void usb_cflag_set(unsigned char state)
// {
// 	usb_cflag = state;
// }
// //�ȴ�һ�����ù���������֣�ʧ�ܳ�ʱ����0���ɹ�����1
// u8 WaitFindUSBflag(u16 timeout)
// {
// 	OS_ERR err;
// 	static u16 time1 = 0;
// 	while (!usb_cflag_get())
// 	{
// 		OSTimeDly(1, OS_OPT_TIME_DLY, &err);
// 		time1 += 1;
// 		if (time1 >= timeout)
// 		{
// 			time1 = 0;
// 			return 0;
// 		}
// 	}
// 	time1 = 0;
// 	return 1;
// }

//USB OTG �жϷ�����
//��������USB�ж�
//void OTG_FS_IRQHandler(void)
//{
//	OSIntEnter();
//	USBH_OTG_ISR_Handler(&USB_OTG_Core);
//	OSIntExit();
//}

// //USB������ǰ����ģʽ
// void usbapp_mode_stop(void)
// {
// 	RCC->AHB2RSTR |= 1 << 7; // USB OTG FS ��λ
// 	delay_ms(5);
// 	RCC->AHB2RSTR &= ~(1 << 7); // ��λ����
// 	memset(&USB_OTG_Core, 0, sizeof(USB_OTG_CORE_HANDLE));
// 	memset(&USB_Host, 0, sizeof(USB_Host));
// 	usbx.bDeviceState = 0;
// 	usbx.hdevclass = 0;
// 	usb_cflag_set(0);
// }

// /**
//  * @description: ����USB����ģʽΪUSB HOST MSCģʽ(Ĭ��ģʽ,��U��)
//  * @param void
//  * @return: void
//  */
// void usbapp_mode_set(void)
// {
// 	USB_CONNECT_ON();   // ��USB��Դ
// 	delay_ms(100);		// �ȴ�100ms��ϵͳ�ȶ�
// 	usbapp_mode_stop(); // ��ֹͣ��ǰUSB����ģʽ

// 	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);
// }

//USBö��״̬�������,��ֹUSBö��ʧ�ܵ��µ�����
//phost:USB_HOST�ṹ��ָ��
//����ֵ:0,û������
//       1,������,�ⲿ������������USB����.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt = 0;
	//���״̬,�����������,��˵��USB������.
	if (phost->gState == HOST_CTRL_XFER && (phost->EnumState == ENUM_IDLE || phost->EnumState == ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if (errcnt > 2000) //������
		{
			errcnt = 0;
			RCC->AHB2RSTR |= 1 << 7; //USB OTG FS ��λ
			delay_ms(5);
			RCC->AHB2RSTR &= ~(1 << 7); //��λ����
			return 1;
		}
	}
	else
		errcnt = 0;
	return 0;
}
static u8 AppState;
// //USB��ѯ����,���������Եı�����.
// void usbapp_pulling(void)
// {

// 	//if(HCD_IsDeviceConnected(&USB_OTG_Core)==0||usbx.hdevclass!=1)//U������ʧ����,��Ҫ���´���USB����
// 	{
// 		USBH_Process(&USB_OTG_Core, &USB_Host); //����USB����
// 		if (USBH_Check_EnumeDead(&USB_Host))	//���USB HOST �Ƿ�������?������,�����³�ʼ��
// 		{
// 			printf("usbapp_mode_set\r\n");
// 			usbapp_mode_set(); //����
// 		}
// 	}
// }
//USB HOST �û��ص�����.
USBH_Usr_cb_TypeDef USR_Callbacks =
	{
		USBH_USR_Init,
		USBH_USR_DeInit,
		USBH_USR_DeviceAttached,
		USBH_USR_ResetDevice,
		USBH_USR_DeviceDisconnected,
		USBH_USR_OverCurrentDetected,
		USBH_USR_DeviceSpeedDetected,
		USBH_USR_Device_DescAvailable,
		USBH_USR_DeviceAddressAssigned,
		USBH_USR_Configuration_DescAvailable,
		USBH_USR_Manufacturer_String,
		USBH_USR_Product_String,
		USBH_USR_SerialNum_String,
		USBH_USR_EnumerationDone,
		USBH_USR_UserInput,
		USBH_USR_MSC_Application,
		USBH_USR_DeviceNotSupported,
		USBH_USR_UnrecoveredError};
/////////////////////////////////////////////////////////////////////////////////
//����Ϊ���ص�����ʵ��.

//USB HOST ��ʼ��
void USBH_USR_Init(void)
{
	USB_OTG_GINTMSK_TypeDef intmsk;
	intmsk.d32 = USB_OTG_READ_REG32(&USB_OTG_Core.regs.GREGS->GINTMSK);
	intmsk.b.usbsuspend = 0; //�رչ����ж�
	USB_OTG_WRITE_REG32(&USB_OTG_Core.regs.GREGS->GINTMSK, intmsk.d32);
	printf("USB OTG HS MSC Host\r\n");
	printf("> USB Host library started.\r\n");
	printf("  USB Host Library v2.1.0\r\n\r\n");
}
//��⵽U�̲���
void USBH_USR_DeviceAttached(void) //U�̲���
{
	usbx.bDeviceState |= 1 << 6; //��⵽USB����
	printf("��⵽USB�豸����!\r\n");
}
//��⵽U�̰γ�
void USBH_USR_DeviceDisconnected(void) //U���Ƴ�
{
	printf("USB�豸�γ�!\r\n");
	usbx.bDeviceState = 0; //����ʧЧ
	usbx.hdevclass = 0;	//�豸�������
	usbConnectStateSet(0);  // ���豸�Ѿ��γ�
}
//��λ�ӻ�
void USBH_USR_ResetDevice(void)
{
	printf("��λ�豸...\r\n");
}
//��⵽�ӻ��ٶ�
//DeviceSpeed:�ӻ��ٶ�(0,1,2 / ����)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
	{
		printf("����(HS)USB�豸!\r\n");
	}
	else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
	{
		printf("ȫ��(FS)USB�豸!\r\n");
	}
	else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
	{
		printf("����(LS)USB�豸!\r\n");
	}
	else
	{
		printf("�豸����!\r\n");
	}
}
//��⵽�ӻ���������
//DeviceDesc:�豸������ָ��
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs;
	hs = DeviceDesc;
	printf("VID: %04Xh\r\n", (uint32_t)(*hs).idVendor);
	printf("PID: %04Xh\r\n", (uint32_t)(*hs).idProduct);
}
//�ӻ���ַ����ɹ�
void USBH_USR_DeviceAddressAssigned(void)
{
	printf("�ӻ���ַ����ɹ�!\r\n");
}
//��������������Ч
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef *cfgDesc,
										  USBH_InterfaceDesc_TypeDef *itfDesc,
										  USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id;
	id = itfDesc;
	if ((*id).bInterfaceClass == 0x08)
	{
		printf("���ƶ��洢���豸!\r\n");
	}
	else if ((*id).bInterfaceClass == 0x03)
	{
		printf("HID �豸!\r\n");
	}
}
//��ȡ���豸Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	printf("Manufacturer: %s\r\n", (char *)ManufacturerString);
}
//��ȡ���豸Product String
void USBH_USR_Product_String(void *ProductString)
{
	printf("Product: %s\r\n", (char *)ProductString);
}
//��ȡ���豸SerialNum String
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	printf("Serial Number: %s\r\n", (char *)SerialNumString);
}
//�豸USBö�����
void USBH_USR_EnumerationDone(void)
{
	printf("�豸ö�����!\r\n\r\n");
}
//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{
	printf("�޷�ʶ���USB�豸!\r\n\r\n");
}
//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)
{
	printf("�����û�ȷ�ϲ���!\r\n");
	usbx.bDeviceState |= 0X80; //���������
	return USBH_USR_RESP_OK;
}
//USB�ӿڵ�������
void USBH_USR_OverCurrentDetected(void)
{
	printf("�˿ڵ�������!!!\r\n");
}
//�û�Ҫ�����³�ʼ���豸
void USBH_USR_DeInit(void)
{
	AppState = USH_USR_FS_INIT;
	printf("���³�ʼ��!!!\r\n");
}
//�޷��ָ��Ĵ���!!
void USBH_USR_UnrecoveredError(void)
{
	printf("�޷��ָ��Ĵ���!!!\r\n\r\n");
}

//USB HOST MSC���û�Ӧ�ó���
int USBH_USR_MSC_Application(void)
{
	u8 res = 0;
	switch (AppState)
	{
	case USH_USR_FS_INIT: //��ʼ���ļ�ϵͳ
		printf("��ʼִ���û�����!!!\r\n");
		AppState = USH_USR_FS_TEST;
		break;
	case USH_USR_FS_TEST: //ִ��USB OTG ����������
		//res=USH_User_App(); //�û�������
		//res=0;
		//if(res)AppState=USH_USR_FS_INIT;
		printf("ִ��USB OTG ����������!!!\r\n");
		if (HCD_IsDeviceConnected(&USB_OTG_Core)) //�豸���ӳɹ�
		{
			printf("�豸���ӳɹ�\r\n");
			usbx.hdevclass = 1; //��⵽U�̲���
								//do nothing
		}
		else
		{
			AppState = USH_USR_FS_INIT;
		}
		break;
	default:
		break;
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////////////
//�û����庯��,ʵ��fatfs diskio�Ľӿں���
extern USBH_HOST USB_Host;

//��ȡU��״̬
//����ֵ:0,U��δ����
//      1,����
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core); //����U��״̬
}

//��U��
//buf:�����ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
u8 USBH_UDISK_Read(u8 *buf, u32 sector, u32 cnt)
{
	u8 res = 1;
	if (HCD_IsDeviceConnected(&USB_OTG_Core) && AppState == USH_USR_FS_TEST) //���ӻ�����,����APP����״̬
	{
		do
		{
			res = USBH_MSC_Read10(&USB_OTG_Core, buf, sector, 512 * cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
			if (!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res = USBH_MSC_FAIL; //��д����
				break;
			};
		} while (res == USBH_MSC_BUSY);
	}
	else
		res = USBH_MSC_FAIL;
	if (res == USBH_MSC_OK)
		res = USBH_MSC_OK;
	return res;
}

//дU��
//buf:д���ݻ�����
//sector:������ַ
//cnt:��������
//����ֵ:����״̬;0,����;����,�������;
u8 USBH_UDISK_Write(u8 *buf, u32 sector, u32 cnt)
{
	u8 res = 1;
	if (HCD_IsDeviceConnected(&USB_OTG_Core) && AppState == USH_USR_FS_TEST) //���ӻ�����,����APP����״̬
	{
		do
		{
			res = USBH_MSC_Write10(&USB_OTG_Core, buf, sector, 512 * cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);
			if (!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res = 1; //��д����
				break;
			};
		} while (res == USBH_MSC_BUSY);
	}
	else
		res = 1;
	if (res == USBH_MSC_OK)
		res = 0;
	return res;
}
