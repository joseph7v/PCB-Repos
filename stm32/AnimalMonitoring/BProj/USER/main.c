#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "os_include.h"
#include "adc.h"
#include "malloc.h"
#include "usb_app.h"

#include "usart3.h"
#include "UART_TCPbuff.h"

#include "mqttBase.h"
#include "mqttApp.h"
#include "MQTTPacket.h"
#include "mqttTransport.h"

#include "MyFunction_C.h"

#include "usart2.h"

#include "key.h"
#include "led.h"
#include "ec25.h"

#include "power.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "malloc.h"  
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "usbh_usr.h" 

#include "cjsontest.h"
#include "usmart.h"

#include "draft.h"

#include "string.h"
#include "fattester.h"	
//sensors
#include "myiic_sht20.h"
#include "myiic_max44009.h"
#include "SHT2x.h"
#include "max44009.h" 
#include "ms5611.h"
#include "sensor.h"  
#include "usart2.h"
#include "rtc.h"
#include "iwdg.h"
//#include "wwdg.h"

#include "usbh_usr.h" 
#include "queue.h" 
#include "rng.h"

#define MQTT_RECEIVEBUFF_MAXLENGTH		2048
uint8_t MQTT_Receivebuf[MQTT_RECEIVEBUFF_MAXLENGTH];

#define MQTT_SEND_BUFF_MAXLENGTH		12000
extern uint8_t MQTT_SEND_buf[MQTT_SEND_BUFF_MAXLENGTH];

#define MQTT_DATA_BUFF_MAXLENGTH		12000
extern uint8_t MQTT_DATA_buf[MQTT_DATA_BUFF_MAXLENGTH];

void NVIC_Configuration(void);
void RCC_Config(void);

// <s>USB_CON
//USBH_HOST  USB_Host;
//USB_OTG_CORE_HANDLE  USB_OTG_Core;
extern USBH_HOST USB_Host;
extern USB_OTG_CORE_HANDLE USB_OTG_Core;


//ϵͳ������ز������ú�����
#define SYSTASK_PRIO									20
//�����ջ��С32bit
#define CPU_STK_SYSTASK_SIZE							1000
//ʱ��Ƭ����
#define SYSTASK_TICK_LEN								0
static  OS_TCB											SysTaskTCB;
static	CPU_STK											SysTaskStk[CPU_STK_SYSTASK_SIZE];
static void  SysTask(void* p_arg);

/* ϵͳ������ */
#define MAIN_TASK_PRIO									21
//�����ջ��С32bit
#define CPU_STK_MAIN_SIZE								1000
//ʱ��Ƭ����
#define MAIN_TICK_LEN									0
static  OS_TCB											MainTaskTCB;
static	CPU_STK											MainTaskStk[CPU_STK_SYSTASK_SIZE];
static void  MainTask(void* p_arg);



/* ����MQTTָ�� */
#define MQTTRECEIVE_PRIO								22
//�����ջ��С32bit
#define CPU_STK_MQTTRECEIVE_SIZE						1000
//ʱ��Ƭ����
#define MQTTRECEIVE_TICK_LEN							0
static  OS_TCB											MQTTRECEIVEtcb;
static	CPU_STK											MQTTRECEIVEstk[CPU_STK_MQTTRECEIVE_SIZE];
static void  MQTTRECEIVEtask(void* p_arg);

/* ����MQTTָ�� */
#define SIM7100_PRIO									23
//�����ջ��С32bit
#define CPU_STK_SIM7100_SIZE							1000
//ʱ��Ƭ����
#define SIM7100_TICK_LEN								0
static  OS_TCB											SIM7100tcb;
static	CPU_STK											SIM7100stk[CPU_STK_SIM7100_SIZE];
static void  SIM7100task(void* p_arg);

/* ����ɨ�� */
#define KeyScan_TASK_PRIO								24
#define CPU_STK_KeyScan_TASK_SIZE						300
#define KeyScan_TICK_LEN								0
static  OS_TCB											KeyScan_TCB;
static	CPU_STK											KeyScan_Stk[CPU_STK_KeyScan_TASK_SIZE];

/* Watch task */
#define HELLO_LED_PRIO									25
#define CPU_STK_HELLO_LED_SIZE							2000
#define HELLO_LED_TICK_LEN								0
static  OS_TCB											HELLO_LEDtcb;
static	CPU_STK											HELLO_LEDstk[CPU_STK_HELLO_LED_SIZE];
static void  HELLO_LEDtask(void* p_arg);

#define  SystemDatasBroadcast_PRIO                      30 // ͳ���������ȼ���ͣ���������12���Ѿ�����������������ȼ���
#define  SystemDatasBroadcast_STK_SIZE                  200 // ����Ķ�ջ��С����ͳ��һ�㹻�ˣ�ͳ�ƽ�������󲻹��ټ�..
#define  SystemDatasBroadcast_LED_TICK_LEN              0
static   OS_TCB                                         SystemDatasBroadcast_TCB;		                // ����ͳ�������TCB
static   CPU_STK                                        SystemDatasBroadcast_STK [SystemDatasBroadcast_STK_SIZE];// ����������Ϊ����ջ������ʹ��
static void  SystemDatasBroadcast (void *p_arg);

void SoftReset(void)
{
    __set_FAULTMASK(1); // �ر������ж�
    NVIC_SystemReset(); // ��λ
}

static vu16 watchdog_f;
static vu16 function_f;

extern int32_t BaroPressure;
extern float BaroAlt;

extern SqQueue Q_stage;   // ��������������
extern SqQueue Q_resent;  // �����ط�������
int main()
{
	//u8 sta;
    OS_ERR err;
    CPU_SR_ALLOC();
	
    SystemInit();
    RCC_Config();
    NVIC_Configuration();
	InitQueue(&Q_stage);  // ��ʼ������ 
	InitQueue(&Q_resent);  // ��ʼ������ 
	watchdog_f=0;
	function_f=0;
    delay_init(168); //ʱ�ӳ�ʼ��
    uart_init(921600);
	USART2_init(9600);
	
//	while(1)
//	{
//		printf("test\r\n");
//		delay_ms(1000);
//		if(USART2_RX_STA&0x8000)
//		{
//			printf("test+++++\r\n");
//			USART2_RX_STA=0;
//		}
//	}
	//IWDG_Init(4,500);1s  64/32*500
	IWDG_Init(6,2500); //256/32=8   2500/500=5
    F407USART1_SendString("usart is ok:");
    LED_Init();   // LED��ʼ��
    Power_Ctrl_Init(); // ��Դ��ʼ��
    Cam_Crtl_Init();   // ����������ų�ʼ��
	adc_Init();
	usbapp_init();
	rng_Init();
//    SHT2x_Init();  			//SHT20��ʼ��
//    max44009_initialize();  //MAX44009��ʼ��
//	MS5611_Init();  //MS5611��ʼ��
//	MS5611_PROM_READ();
//		
//	SHT2x_Init();  //SHT20��ʼ��
//	MS5611_Init();  //MS5611��ʼ��
//	MS5611_PROM_READ();
//	max44009_initialize();  //MAX44009��ʼ��
//	delay_ms(500);
//	MS5611_Temperature_Get();
//	MS5611_Pressure_Get();
//	BaroPressure=MS5611_Compensation_Calcu();
//	BaroAlt=getEstimatedAltitude(BaroPressure);

    My_RTC_Init();  //��ʼ��RTC
	calendar_get_time(&calendar);
	calendar_get_date(&calendar);
	printf("DATA:%d-%d-%d	Time:%d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
    //RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,600*2);		//����WAKE UP�ж�,1�����ж�һ��
    
    //��ʼ��USB����
    USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);
	delay_ms(1000);
    if (!SD_Init())
        printf("SD_Init ok\r\n"); //�ж�SD���Ƿ����
    else
        printf("SD_Init Error\r\n");

    mymem_init(SRAMIN);      //��ʼ���ڲ��ڴ��
    exfuns_init();           //Ϊfatfs��ر��������ڴ�
    f_mount(fs[0], "0:", 1); //����SD��
    f_mount(fs[1], "1:", 1); //����U��
    
//	sta = SIM7100_Init();
//    if(sta)
//		F407USART1_SendString("SIM7100_Init OK\r\n");
//	else
//		F407USART1_SendString("SIM7100_Init ERROR\r\n");
	IWDG_Feed();//ι��
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����ϵͳ
    OSInit(&err);        //���ڳ�ʼ�� uC/OS-III
    OS_CRITICAL_ENTER(); //�����ٽ���
	printf("OS_CRITICAL_ENTER\r\n");
    //
    OSTaskCreate((OS_TCB *)&SysTaskTCB,                               //�������ջ�Ŀ�ʼ��ַ
                 (CPU_CHAR *)"SysTask",                               //�����������
                 (OS_TASK_PTR)SysTask,                                //ָ����������ָ��
                 (void *)0,                                           //ָ�룬��һ��ִ������ʱ���ݸ�������ʵ���ָ�����*p_arg
                 (OS_PRIO)SYSTASK_PRIO,                               //���ȼ�����	 ����ֵԽС���ȼ�Խ��
                 (CPU_STK *)&SysTaskStk[0],                           //�����ջ�Ļ���ַ������ַͨ���Ƿ����������Ķ�ջ������ڴ�λ��
                 (CPU_STK_SIZE)CPU_STK_SYSTASK_SIZE / 10,             //���߸������ǵ�ַ��ˮӡ�� ������ջ������ָ��λ��ʱ�Ͳ�������������
                 (CPU_STK_SIZE)CPU_STK_SYSTASK_SIZE,                  //����Ķ�ջ��С
                 (OS_MSG_QTY)0,                                       //
                 (OS_TICK)SYSTASK_TICK_LEN,                           //��������ӵ�ж��ٸ�ʱ��Ƭ��������ʱ��Ƭ��ѯ��������ʱ��Ч
                 (void *)0,                                           //
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //
                 (OS_ERR *)&err);                                     //���ص�ǰ����ִ�еĽ��OS_ERR_NONE��ʾ�ɹ�
    OS_CRITICAL_EXIT();                                               //�˳��ٽ���
    OSStart(&err);                                                    //��ʼ��������
}
   
static void SysTask(void *p_arg) // == appCreat
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
    // stackMonitoring
    OSTaskCreate((OS_TCB *)&SystemDatasBroadcast_TCB,
                 (CPU_CHAR *)"SystemDatasBroadcast",
                 (OS_TASK_PTR)SystemDatasBroadcast,
                 (void *)0,
                 (OS_PRIO)SystemDatasBroadcast_PRIO,
                 (CPU_STK *)&SystemDatasBroadcast_STK[0],
                 (CPU_STK_SIZE)SystemDatasBroadcast_STK_SIZE / 10, /*ջ����ٽ�ֵ��������ջ��С��90%��*/
                 (CPU_STK_SIZE)SystemDatasBroadcast_STK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    //


    //
    OSTaskCreate((OS_TCB *)&MQTTRECEIVEtcb,
                 (CPU_CHAR *)"MQTTRECEIVE",
                 (OS_TASK_PTR)MQTTRECEIVEtask,
                 (void *)0,
                 (OS_PRIO)MQTTRECEIVE_PRIO,
                 (CPU_STK *)&MQTTRECEIVEstk[0],
                 (CPU_STK_SIZE)CPU_STK_MQTTRECEIVE_SIZE / 10,
                 (CPU_STK_SIZE)CPU_STK_MQTTRECEIVE_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)MQTTRECEIVE_TICK_LEN,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    //
    OSTaskCreate((OS_TCB *)&SIM7100tcb,
                 (CPU_CHAR *)"SIM7100",
                 (OS_TASK_PTR)SIM7100task,
                 (void *)0,
                 (OS_PRIO)SIM7100_PRIO,
                 (CPU_STK *)&SIM7100stk[0],
                 (CPU_STK_SIZE)CPU_STK_SIM7100_SIZE / 10,
                 (CPU_STK_SIZE)CPU_STK_SIM7100_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)SIM7100_TICK_LEN,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    // KeyScan
    OSTaskCreate((OS_TCB *)&KeyScan_TCB,
                 (CPU_CHAR *)"KeyScan",
                 (OS_TASK_PTR)KeyScan_Task,
                 (void *)0,
                 (OS_PRIO)KeyScan_TASK_PRIO,
                 (CPU_STK *)&KeyScan_Stk[0],
                 (CPU_STK_SIZE)CPU_STK_KeyScan_TASK_SIZE / 10,
                 (CPU_STK_SIZE)CPU_STK_KeyScan_TASK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)KeyScan_TICK_LEN,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    // watchTask
    OSTaskCreate((OS_TCB *)&HELLO_LEDtcb,
                 (CPU_CHAR *)"HELLO_LED",
                 (OS_TASK_PTR)HELLO_LEDtask,
                 (void *)0,
                 (OS_PRIO)HELLO_LED_PRIO,
                 (CPU_STK *)&HELLO_LEDstk[0],
                 (CPU_STK_SIZE)CPU_STK_HELLO_LED_SIZE / 10,
                 (CPU_STK_SIZE)CPU_STK_HELLO_LED_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)HELLO_LED_TICK_LEN,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
				 
    OSTaskCreate((OS_TCB *)&MainTaskTCB,                              // �������ջ�Ŀ�ʼ��ַ
                 (CPU_CHAR *)"MainTask",                              // �����������
                 (OS_TASK_PTR)MainTask,                               // ָ����������ָ��
                 (void *)0,                                           // ָ�룬��һ��ִ������ʱ���ݸ�������ʵ���ָ�����*p_arg
                 (OS_PRIO)MAIN_TASK_PRIO,                             // ���ȼ�����	 ����ֵԽС���ȼ�Խ��
                 (CPU_STK *)&MainTaskStk[0],                          // �����ջ�Ļ���ַ������ַͨ���Ƿ����������Ķ�ջ������ڴ�λ��
                 (CPU_STK_SIZE)CPU_STK_MAIN_SIZE / 10,                // ���߸������ǵ�ַ��ˮӡ�� ������ջ������ָ��λ��ʱ�Ͳ�������������
                 (CPU_STK_SIZE)CPU_STK_MAIN_SIZE,                     // ����Ķ�ջ��С
                 (OS_MSG_QTY)0,                                       //
                 (OS_TICK)MAIN_TICK_LEN,                              // ��������ӵ�ж��ٸ�ʱ��Ƭ��������ʱ��Ƭ��ѯ��������ʱ��Ч
                 (void *)0,                                           //
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //
                 (OS_ERR *)&err);                                     // ���ص�ǰ����ִ�еĽ��OS_ERR_NONE��ʾ�ɹ�		 
		
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}

// <s>HELLO_LEDtask
// check usb = watch_LEDtask
//static u8 memdevflag=0;
//void  HELLO_LEDtask(void *pdata)
//{
//	OS_ERR err;
//	u16 t;
//	//u16 cnt = 0;
//	printf("HELLO_LED task run\r\n");
//	while(1)
//	{
//		t++;
//			
//		// ֻ������Ҫʹ��USB��ʱ��������
//		if(1)
//		{
//			if((t%60)==0)//600ms���Ҽ��1��
//			{
//				if (usbx.hdevclass == 1)
//				{
//					if (memdevflag == 0) //U�̲���λ?
//					{
//						fs[1]->drv=2;  //��ʱ��ΪҲ���Բ���,����Ϊ������������
//						f_mount(fs[1],"1:",1); 	//���¹���U��
//						usbapp_user_app();
//						usbConnectStateSet(1);
//						memdevflag = 1;
//					}
//				}
//				else
//					memdevflag = 0; // U�̱��γ���
//			}
//			// U��ģʽ,�Ŵ���
//			while ((usbx.bDeviceState & 0XC0) == 0X40) //USB�豸������,���ǻ�û���ӳɹ�,�Ͳ�ѯ.
//			{
//				usbapp_pulling(); //��ѯ����USB����
//				delay_us(1000);   //������HID��ô��...,U�̱Ƚ���
//			}
//			usbapp_pulling(); //����USB����
//		}
//		UART_TCPbuff_Run(F407USART3_buffRead);
//		OSTimeDly(8,OS_OPT_TIME_DLY,&err);
//	}									 
//}
// <e>HELLO_LEDtask

static u8 memdevflag=0;
void  HELLO_LEDtask(void *pdata)
{
	OS_ERR err;
	u16 t;
	//u16 cnt = 0;
	printf("HELLO_LED task run\r\n");
	while(1)
	{
		t++;

		// ֻ������Ҫʹ��USB��ʱ��������
		if(usbConnectSwitchGet())
		{
			if((t%60)==0)
			{
				if(usbx.hdevclass==1)
				{
					//printf("U disk is ON,memdevflag=%d\r\n",memdevflag);
					if(memdevflag==0) // U�̲���λ?
					{
						fs[1]->drv=2; // ��ʱ��ΪҲ���Բ���,����Ϊ������������
						f_mount(fs[1],"1:",1); 	// ���¹���U��
						usbapp_user_app();

						usbConnectStateSet(1);						
						memdevflag=1;
					}  
				}else memdevflag=0; // U�̱��γ���
			}
			
			while((usbx.bDeviceState&0XC0)==0X40) // USB�豸������,���ǻ�û���ӳɹ�,�Ͳ�ѯ.
			{
				usbapp_pulling(); // ��ѯ����USB����
				delay_ms(1); // ������HID��ô��...,U�̱Ƚ���
				#if (EN_log_print >= 2)
				printf(".");
				#endif // EN_log_print
			}
			usbapp_pulling();//????USB????
			#if (EN_log_print >= 2)
			printf("\\");
			#endif // EN_log_print
		}
		UART_TCPbuff_Run(F407USART3_buffRead);
		OSTimeDly(5,OS_OPT_TIME_DLY,&err);
	}									 
}

// <s>MainTask
void openUSB(void);
void closeUSB(void);
u8 bufmessages[1500];
static void MainTask(void *p_arg) // test fun
{
    OS_ERR err;
    u8 res;

    while (1)
    {
		// ��������
		// ������1HzƵ����˸����
        if (function_f == 1)
        {
            if (key1_down == 0 && key2_down==0)
            {
				//u8 res;
				//u8* messages;
				
				//messages = mymalloc(SRAMIN, 1500);
				//if(messages)
					//F407USART1_SendString("mymalloc\r\n");
				
				LED_GREEN_OFF();
				
				//#if USART1_DEBUG
				F407USART1_SendString("Data_Packing_sens\r\n");
				//#endif
				Data_Packing_sens((char *)bufmessages);
				mysend_data((char *)bufmessages);
				//mysend_data("123456\r\n");
				delay_ms(500);
				LED_GREEN_OFF();
				delay_ms(500);
				
				LED_GREEN_OFF();
				//Data_Packing_gps((char *)bufmessages);
				//#if USART1_DEBUG
				F407USART1_SendString("Data_Packing_gps\r\n");
				//#endif
				//mysend_data((char *)bufmessages);

				delay_ms(500);
				LED_GREEN_OFF();
				
				//myfree(SRAMIN, messages);		

				//mycheck_Queue();
				function_f = 0;
            }
        }
// ��ɨ��ģʽ ģ�����USB 		
        if (key1_down == 1)
        {
            F407USART1_SendString("key1_down..openUSB...\r\n");
            // ��ʼ��USB
			usbConnectSwitchSet(1);  // ��������Ҫ�ȴ�ɨ��ģʽ
			usbapp_mode_set();  // ��������USBģʽ����λUSB
			// �������
			usbConnectStateSet(0);  // ��ձ�־λ
			openUSB();  // �������USB�ɵ�Դ
			// �ȴ����ӳɹ�
            res = waitUsbConnectFlag(10000);
            if (res == 1) // ���������
            {
				delay_ms(2000);
				printf("successful find usb��open camera!\r\n");
				mf_scan_files("1:DCIM/100IMAGE");
				mf_dcopy("1:DCIM/100IMAGE","0:INBOX",1);
				mf_scan_files("1:DCIM/100IMAGE");
				mf_scan_files("0:INBOX");
            }
            else
            {
				//mysend_log("Fail find camera");
                F407USART1_SendString("Fail WaitDistinguishflag...\r\n");
            }
			closeUSB();  // close usb power
			res = waitUsbDisonnectFlag(5000);
			if (res == 1) // �����ر����
            {
				F407USART1_SendString("success closeUSB...\r\n");
			}
			else
			{
				F407USART1_SendString("closeUSB..fail\r\n");
			}
			usbapp_mode_stop();
			usbConnectSwitchSet(0);
			key1_down = 0;
		}
		// take photo
		if(key_wkup_down == 1)
		{
			if(usbConnectSwitchGet() == 0)
			{
				USB_Photograph();
				key_wkup_down = 0;
			}
		}
		// ����ͼƬ
        if (key2_down == 1)
        {
			u8 i;
            F407USART1_SendString("key2_down..\r\n");
			//2Hz 2.5S
			for(i=0;i<3;i++)  
			{
				LED_GREEN_OFF();
				delay_ms(250);
				LED_GREEN_OFF();
				delay_ms(250);
			}
			
			mf_scan_files("0:INBOX");
			if(mqtt_state_get() == 1)
			{
				//mqtt_state_set(5); // ��æ״̬����ֹping�ͷ�������
				mf_send_pics("0:INBOX","0:ARCH",1);
				mf_scan_files("0:INBOX");
				mf_scan_files("0:ARCH");
				//mqtt_state_set(1);
			}
			else
			{
			
			}

            key2_down = 0;
			//mqtt_state_set(1);  // �ظ�����ģʽ
            
//			if(usb_state_get() == 0)
//				USB_Photograph();
//			else
//				printf("warning:USB is connected\r\n");
        }
		

        OSTimeDly(1, OS_OPT_TIME_DLY, &err);
    }
}
// <e>MainTask

//	InitQueue(&Q_stage);  // ��ʼ������ 
//	InitQueue(&Q_resent);  // ��ʼ������
void check_response(u8* load, int len)
{
	u8 i,cnt;
	u32 uid=0;
	QElemType elem;
	if(load[0]==0xa5)
	{
		if(load[1]==0x96)
		{
			uid = (load[2]<<24) +  (load[3]<<16) + (load[4]<<8) +  load[5];
			// printf(">>>>>>>>>>>>>>>>>>>>>>>>R*UID:%0X\r\n",uid);
			printf(">>>>>RUID:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n", 0, 0, 0, uid, 0XFFFF);
			// visit Q_stage squeue
			cnt=0;
			i=Q_stage.front;
			while(i!=Q_stage.rear && cnt++<90)
			{
				// printf("<VUID:%0X,%3d,pic=%3d\r\n",Q_stage.data[i].uid, i, Q_stage.data[i].pic_id);
				printf("<<<<<VUID:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n", i,Q_stage.rear,(Q_stage.rear-i+MAXSIZE)%MAXSIZE,Q_stage.data[i].uid,Q_stage.data[i].pack_id);
				if(Q_stage.data[i].uid==uid)
				{
					while(--cnt)  //�������еĶ����Ļ���
					{
						DeQueue(&Q_stage, &elem);
						printf("*-Q_stage:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n",Q_stage.front,Q_stage.rear,QueueLength(Q_stage),elem.uid,elem.pack_id);
						EnQueue(&Q_resent,elem);
						//printf("*+Q_resnt:[%3d,%3d,%3d],Q+UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), elem.uid,elem.pack_id);	
						printf("* Q_resnt:[%3d,%3d,%3d],Q*UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);						
					}
					
					DeQueue(&Q_stage, &elem);
					printf("- Q_stage:[%3d,%3d,%3d],Q-UID:%08X,pid:%04X\r\n",Q_stage.front,Q_stage.rear,QueueLength(Q_stage),elem.uid,elem.pack_id);
					// printf("* Q_resnt:[%3d,%3d,%3d],Q+UID:%08X,pid:%04X\r\n",Q_resent.front,Q_resent.rear,QueueLength(Q_resent),elem.uid,elem.pack_id);
					printf("* Q_resnt:[%3d,%3d,%3d],Q*UID:%08X,pid:%04X\r\n", Q_resent.front, Q_resent.rear, QueueLength(Q_resent), Q_resent.data[Q_resent.front].uid, Q_resent.data[Q_resent.front].pack_id);
					break;		
				}
				i=(i+1)%MAXSIZE;
				
			}
//			if(cnt==1)
//			{
//				DeQueue(&Q_stage, &elem);
//				printf("Q_stage->front=%d,Q_stage->rear=%d,cnt=%d\r\n",Q_stage.front,Q_stage.rear,cnt);
//			}
//			else if(cnt<89)
//			{
//				cnt--;
//				while(cnt--)
//				{
//					printf("DeQueue & EnQueue cnt=%d\r\n",cnt);
//					DeQueue(&Q_stage, &elem);
//					printf("Q_resent->front=%d,Q_stage->rear=%d\r\n",Q_stage.front,Q_stage.rear);
//					EnQueue(&Q_resent,elem);
//					printf("Q_resent->front=%d,Q_resent->rear=%d\r\n",Q_resent.front,Q_resent.rear);					
//				}
//				DeQueue(&Q_stage, &elem);
//				printf("Q_stage->front=%d,Q_stage->rear=%d,cnt=%d\r\n",Q_stage.front,Q_stage.rear,cnt);
//			}
		}
	}
}
// <e>MQTTRECEIVEtask
// ����MQTT��������ָ��
static void MQTTRECEIVEtask(void *p_arg)
{
    OS_ERR err;
	
	

    int type; // �������յ�����ֵ
    //===========================
    unsigned char dup;
    int qos;
    // ������־
    unsigned char retained;
    // ��id
    unsigned short packetid;
    // ������
    MQTTString topicName;

    // ����
    unsigned char *payload;
    // ���ݳ���
    int payloadlen;
	//
	int test_cnt=0;
    //==============================
    while (1)
    {
        if (UART_TCP_buffLength() != 0)
        {
			#if (EN_log_print >= 3)
            F407USART1_SendString("+UART_TCP\r\n");
			#endif // EN_log_print	
            //������յ���MQTT��Ϣ�������ݲ�ͬ����Ϣ��������ͬ�Ĵ���
            type = MQTTPacket_read(MQTT_Receivebuf, MQTT_RECEIVEBUFF_MAXLENGTH, transport_getdata);
            switch (type)
            {
            case CONNECT:
                break;
            case CONNACK:          //����������Ӧ
				// mqtt_state_set(1); // �������ӳɹ�
				mqtt_subscribe(MY_TOPIC_MSGDOWN);
				printf("%s\r\n",MY_TOPIC_MSGDOWN);
                F407USART1_SendString("MQTT Connect CONNACK\r\n");
                break;
            case PUBLISH: //���ĵ���Ϣ,�ɱ��˷���
                if (MQTTDeserialize_publish(&dup, &qos, &retained, &packetid, &topicName, &payload, &payloadlen,
                                            MQTT_Receivebuf, MQTT_RECEIVEBUFF_MAXLENGTH) == 1)
                {					
//					// �������յ����ַ���
//					if (MystringSeek((uint8_t *)(topicName.lenstring.data), topicName.lenstring.len, (uint8_t *)MY_TOPIC_PICDOWN, strlen(MY_TOPIC_PICDOWN)) != -1)
//					{ 
//						printf("\r\nTOPIC1=%s,payloadlen=%d\r\n", MY_TOPIC_PICDOWN, payloadlen);
//					}
					
					#if (EN_log_print >= 3)
					int i;
					F407USART1_SendString("payload:");
					for (i = 0; i < payloadlen; i++)
					{
						printf("%0X",payload[i]);
						// ��ӡ�������������
					}
					F407USART1_SendString("\r\n");
					#endif // EN_log_print	
					
//					test_cnt++;
//					if(test_cnt%10!=1)
//					{
						check_response(payload,payloadlen);
//						F407USART1_SendString("miss one and try resend\r\n");
//					}
                }
                break;
            case PUBACK: //������Ϣ��Ӧ����ʾ�����ɹ�
                break;
            case PUBREC:
                break;
            case PUBREL:
                break;
            case PUBCOMP:
                break;
            case SUBSCRIBE:
                break;
            case SUBACK: //������Ϣack	
				printf("MQTT subscrible SUBACK\r\n ");
				mqtt_state_set(1); // �������ӳɹ�
                break;
            case UNSUBSCRIBE:
                break;
            case UNSUBACK:
                break;
            case PINGREQ:
                break;
            case PINGRESP: //Ping����
				mqtt_ping_state_set(1);
                break;
            case DISCONNECT:
                break; //�ɿͻ��˷��͸�����������ʾ�����Ͽ�����
            default:
                break;
            }
        }
        OSTimeDly(5, OS_OPT_TIME_DLY, &err);
    }
}
// <e>MQTTRECEIVEtask
//static vu16 my_heart=0;
// <s>SIM7100task
// �����ʹ�õ�ʱ��Ŵ������񣬲��õ�ʱ�򲻴���
u8 f_reconnect=0; 
//uint8_t sss = 0;
static void SIM7100task(void *p_arg)
{
    OS_ERR err;

    uint16_t time = 0;
    
	//MY_MQTT_ERR mqtterr;
	static u8 mqtt_connect_flag=0;
    F407USART1_SendString("SIM7100task run\r\n");
	ec25_Init();
    while (1)
    {        
        if (time % 10 == 0 && f_reconnect == 0)
        { //�������ʧ�ܣ�ÿ��5������³�������һ��
            if (mqtt_state_get() == 0)
            {
                mqtt_connect();
				printf("Try mqtt_connect f:%d\r\n",f_reconnect);
				
				//f_reconnect=0;
				mqtt_connect_flag++;  // �ۼƳ������Ӵ���
				if(mqtt_connect_flag*5>120)
				{
					SoftReset();
					delay_ms(1000);
				}
				
				//sss = 0;
				
				LED_YELLOW_OFF();
            }
        }
		if (time % 10 == 1)  // mqtt ping
		{
			if (mqtt_state_get() == 1 && function_f == 0 && key1_down==0 && key2_down==0)
            {
				u16 cnt=0;
				mqtt_Ping();
				printf("ping mqtt .....\r\n");
				while(!mqtt_ping_state_get())
				{
					cnt++;
					OSTimeDly(10,OS_OPT_TIME_DLY,&err);
					if(cnt>=500) // 5s
					{
						printf("none ping back .....\r\n");
						
						mqtt_state_set(0);
						LED_YELLOW_OFF();
						break;
					}
				}
				printf("receive ping back .....\r\n");
				mycheck_Queue();
				mqtt_ping_state_set(0);  // ��ձ�־λ
            }
		}
		

        if (mqtt_state_get() == 1)
        {
//            if (sss == 0)
//            {
//				//�������MQTT�������ɹ����ᶩ��һ��picdown������Ϣ�����Ը�����Ҫ�����Ƿ��ж���Ӧ���ж��Ƿ��ĳɹ�
////                mqtt_subscribe(MY_TOPIC_PICDOWN);
////				delay_ms(10);
////                mqtt_subscribe(MY_TOPIC_MSGDOWN);
////				delay_ms(10);
////				//mysend_log((char*)"{\"log\":\"mqtt connect\"}");
////				sss = 1;
////				printf("mqtt_subscribe \r\n ");
//            }
			mqtt_connect_flag=0; // һ���ɹ�������
			LED_YELLOW_ON();
        }
        else
        {
			//sss=0;
            LED_YELLOW_OFF();
        }
		time++;
        time %= 600;
		//printf("time=%d ..... cflag%d\r\n",time,mqtt_connect_flag);
        OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
    }
}
// <e>SIM7100task

// <s>SystemDatasBroadcast
void  SystemDatasBroadcast (void *p_arg)
{
  OS_ERR err;
  CPU_STK_SIZE free,used;
  (void)p_arg;
  
  while(DEF_TRUE)
  {
    IWDG_Feed();//ι��
	OSTaskStkChk (&SystemDatasBroadcast_TCB,&free,&used,&err);
	
	#if USART1_DEBUG
	printf("SystemDatasBroadcast  used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
    #endif
	OSTaskStkChk (&MainTaskTCB,&free,&used,&err);
	#if USART1_DEBUG
	printf("MainTaskTCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
	#endif	
	OSTaskStkChk (&MQTTRECEIVEtcb,&free,&used,&err);
	#if USART1_DEBUG
	printf("MQTTRECEIVEtcb             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
	#endif	
	OSTaskStkChk (&SIM7100tcb,&free,&used,&err);
	#if USART1_DEBUG
	printf("SIM7100tcb              used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
	#endif	
	OSTaskStkChk (&KeyScan_TCB,&free,&used,&err);
	#if USART1_DEBUG
	printf("KeyScan_TCB          used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
	#endif	
	OSTaskStkChk (&HELLO_LEDtcb,&free,&used,&err);
	#if USART1_DEBUG
	printf("HELLO_LEDtcb             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
	#endif
	if(watchdog_f*10>7200) //1h,SoftReset
	{
		printf("\r\n\r\n\r\nsleep\r\n\r\n");
		SoftReset();
	}
	//if((watchdog_f*10)%180 == 10)  // 5min��60s��ʼ���ʹ���������
	{
		function_f=1;
	}
	
	if((watchdog_f*10)%1800 == 70) // 30min��70s ��ʼɨ�����
	{
		key_wkup_down = 1;
	}
	if((watchdog_f*10)%1800 == 100) // 30min��70s ��ʼɨ�����
	{
		key1_down = 1;
	}
	if((watchdog_f*10)%1800 == 150) // 30min��130s ��ʼ�ϴ�ͼƬ
	{
		key2_down = 1;
	}

	watchdog_f++;
	printf("<%d>-heart f:%d,ka:%d,kb:%d\r\n\r\n\r\n",watchdog_f,function_f,key1_down,key2_down);
	OSTimeDlyHMSM(0,0,10,0,(OS_OPT)OS_OPT_TIME_DLY,(OS_ERR*)&err);  
   }
}
// <e>SystemDatasBroadcast

//============================================================
//ϵͳʱ��168M
void RCC_Config(void)
{
    RCC_DeInit(); 
    RCC_HSEConfig(RCC_HSE_ON);  
    if(RCC_WaitForHSEStartUp() == SUCCESS) 
    {
        RCC_PLLCmd(DISABLE); 
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
        RCC_HCLKConfig(RCC_SYSCLK_Div1);  
        RCC_PCLK1Config(RCC_HCLK_Div4);  
        RCC_PCLK2Config(RCC_HCLK_Div2); 
        RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);
        RCC_PLLCmd(ENABLE); 
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    }
}
//���ܣ� ϵͳ��ʱ����ϵͳ��ʱ�ж�����
//������ void
//���أ� void
//˵����
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	//
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick->CTRL&=~(1<<2);									//SYSTICKʹ���ⲿʱ��Դ,����Ƶ
	//
	SysTick->CTRL|=1<<1;   									//����SYSTICK�ж�
	SysTick->LOAD=20000;   								//��ϵͳʱ��Ϊ120Mʱ������1ms�ж�,��Լ
	//
	NVIC_InitStructure.NVIC_IRQChannel =RCC_IRQn;							//�ж�ͨ��Ϊϵͳʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;		//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;					//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);	
	//
	SysTick->CTRL|=1<<0;   		//����SYSTICK
}//*/
