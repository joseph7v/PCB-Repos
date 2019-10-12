#include "sys.h"
#include "usart2.h"	  


//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART2_RX_STA=0;   	 
void USART2_IRQHandler(void)
{
	u8 res;	    
	//OSIntEnter();    
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)//���յ�����
	{	 
		res=USART_ReceiveData(USART2); 	
		printf("%c",res);
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{	
			//if(res==0x3C)USART2_RX_STA=0;  //��ʼ����
			
			if(res=='>')
			{
				USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
				USART2_RX_STA++;
				USART2_RX_STA|=0x8000;
			}
			else
			{
				USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
				USART2_RX_STA++;
				if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	
			}		 
		}  		
	}  											 
	//OSIntExit();  											 
}   
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void USART2_init(u32 bound)
{  	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(USART2);  //��λ����2
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOA2����GPIOA3
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
		
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART2_RX_STA=0;				//���� 
}

/**
 * @description: �򴮿�2����һ���ֽ�����
 * @param uint8_t Data
 * @return: none
 */
void F407USART2_SendByte(uint8_t Data)
{
	while (!(USART2->SR & USART_FLAG_TXE))
		;
	USART_SendData(USART2, Data);
}

/**
 * @description: �򴮿�2����ָ�����ȵ��ֽ�
 * @param uint8_t * Data ���ݵ�ַ; uint16_t leng ���ݾֳ���  
 * @return: 
 */
void F407USART2_SendBytes(char *Data, uint16_t leng)
{
	uint16_t i = 0;
	for (i = 0; i < leng; i++)
	{
		F407USART2_SendByte(*(Data + i));
	}
}

/**
 * @description: �����ַ���
 * @param char *str string address
 * @return: 
 */
void F407USART2_SendString(char *str)
{
	u16 i = 0;
	while (*(str + i) != 0)
	{
		F407USART2_SendByte(*(str + i));
		i++;
	}
}
