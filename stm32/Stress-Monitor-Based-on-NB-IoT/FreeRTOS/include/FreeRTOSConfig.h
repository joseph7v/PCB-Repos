
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    #include "main.h" 
    extern uint32_t SystemCoreClock;
#endif


//FreeRTOS�ĵ͹�������
//------------------------------------------------------------------------------------------//
#define configUSE_TICKLESS_IDLE					1   /*<���õ͹���ticklessģʽ*/

extern void PreSleepProcessing(uint32_t ulExpectedTime);
extern void PostSleepProcessing(uint32_t ulExpectedTime);
extern void RealWaitingTimeProcessing(uint32_t ulCompleteTickPeriods);
////����͹���ģʽǰҪ���Ĵ���
#define configPRE_SLEEP_PROCESSING	PreSleepProcessing
///�˳��͹���ģʽ��Ҫ���Ĵ���
#define configPOST_SLEEP_PROCESSING		PostSleepProcessing
///�˳��͹���ģʽ��õ���tickless��ʵ�ʵȴ���ʱ�䣬�Ƿ���Ҫʹ��
//Ӧ�ó������û�����ʹ�õδ�ʱ���ж��з���һ���������������ڼ����鿴�Ƿ�ʱ
//��ô�������Tickless�ͻ���ֽ���͹��ĺ�ʱ�䲻�ܼ��ϵ�����
//������PostSleepProcessing������ֻ֪��Ԥ�ڵ�ʱ���Ƕ��٣������ǵ���ʵ�¼�
//������vPortSuppressTicksAndSleep��PostSleepProcessing�д����꿪��ʱ�ӵȲ����󣬺��ڻ�õ�
//ulCompleteTickPeriods��ʵ�ʵȴ���ʱ��
#define configREAL_WAITINGTIME_PROCESSING		RealWaitingTimeProcessing

//------------------------------------------------------------------------------------------//


#define configAPPLICATION_ALLOCATED_HEAP         0/*<���ڴ��ɱ������Զ�����>����������壬Ĭ��Ϊ0*/
#define configENABLE_BACKWARD_COMPATIBILITY      1 /*<�������ʹ��>��Ĭ��Ϊ1*/


#define configUSE_PREEMPTION                     1		/*<������ռʽ����������Э��>��1����ռʽ��������0��Э��*/
#define configSUPPORT_STATIC_ALLOCATION          0
#define configSUPPORT_DYNAMIC_ALLOCATION         1  /*<֧�ֶ�̬����>���Զ���FreeRTOS�Ķ��з��䡣��������壬Ĭ��Ϊ1.*/
#define configUSE_IDLE_HOOK                      0		/*<ʹ�ÿ��������Ӻ���>��1��ʹ�ã�0����ʹ��*/
#define configUSE_TICK_HOOK                      0		/*<ʹ��ʱ��Ƭ���Ӻ���>��1��ʹ�ܣ�0��ʧ��*/
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )   /*<����CPU�Ĺ���>*/
#define configTICK_RATE_HZ                       ((TickType_t)1000)    /*<ʱ�ӽ���Ƶ��>�����δ�ʱ�����ж�Ƶ��*/


/*configUSE_PORT_OPTIMISED_TASK_SELECTION����Ϊ1����configMAX_PRIORITIES��󲻳���32*/
#define configMAX_PRIORITIES                     ( 7 )	/*<�������������ȼ���Ŀ>�����ȼ���0�����ò���-1��,0��������ȼ���*/

#define configUSE_TIME_SLICING					 1   /*<֧��ʱ����Ƭ>��Ĭ��Ϊ1*/
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)/*<������������С�Ķ�ջ��С>���õ�λΪ�֣�STM32��32λ����4���ֽڣ�������ֵ��100������400���ֽ�*/
#define configTOTAL_HEAP_SIZE                    ((size_t)3072)    /*<�ܹ��Ķ�ջ��С>��*/
#define configMAX_TASK_NAME_LEN                  ( 16 )	/*<�����������Ƴ���>*/
#define configUSE_16_BIT_TICKS                   0    /*<����ϵͳ���ļ�����������������>,0:32bits;1:16bits*/
#define configUSE_MUTEXES                        1		/*<ʹ�û����ź���>��1:ʹ�ã�0���ر�*/
#define configQUEUE_REGISTRY_SIZE                8	/*<����ע��Ķ��к��ź���������>*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1	/*<��һ��Ҫ���е���������ַ�����1��Ӳ��������STM32��������ָ��CLZ��2��ͨ�÷�����>*/

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0	/*<����Э��>��1��������0���ر�*/
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )  /*<Э�̵�������ȼ���Ŀ>�������ȼ���0�����ò���-1����������ȼ���0*/

/* INCLUDE��ʼ�ĺ꣬����API��ʹ�ܺ�ʧ�� */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             0
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1


/*<MCUʹ�ü�λ���ȼ�>��STM32����4bits*/
#ifdef __NVIC_PRIO_BITS
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         4
#endif

/*<����������ȼ�>��STM32�ж����ȼ�������4����ռ���ȼ���0��15���������ȼ�������������Ϊ15*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15


/*<����FreeRTOSϵͳ�ɹ����������ȼ�>,���ڸ�ֵ�����ȼ���FreeRTOS�������ڸ�ֵ�����ȼ�����FreeRTOS����*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/*<�����ں��ж����ȼ�>,��configLIBRARY_LOWEST_INTERRUPT_PRIORITY��������ȼ���д��STM32���ȼ��Ĵ����ĸ�4λ*/
/*��ͬ��MCU���ò�ͬ�����ȼ�λ����������8bits*/
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*<����BASEPRI��С�ڵ��ڸ��ж����ȼ���ֵ����ֵԽ�ߣ��ж����ȼ�Խ�ͣ����ж���FreeRTOS����>�����ڸ����ȼ����жϿ��԰�ȫ�ĵ���FreeRTOS��API����*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


/* USER CODE BEGIN 1 */ 
//��ʽ1��CubeMX���ɵĴ���
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );} 

/*��ʽ2���Ѵ���ĵص��ӡ����ӡ������*/
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char ,int );
//#define configASSERT( x ) if ((x) == 0) vAssertCalled(__FILE__,__LINE__) ;
/* USER CODE END 1 */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: This define MUST be commented when used with STM32Cube firmware, 
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */
/* #define xPortSysTickHandler SysTick_Handler */

/* USER CODE BEGIN Defines */   	      
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */ 

#endif /* FREERTOS_CONFIG_H */
