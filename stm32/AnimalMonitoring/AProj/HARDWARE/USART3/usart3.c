//#include "delay.h"
#include "usart3.h"  // self
#include "stdarg.h"  // standard arguments, for u3_printf.
#include "stdio.h"	 // standard IO, for u3_printf.
#include "string.h"	 // string function, for u3_printf.
#include "timer.h"	 // for uart3 to detect  idle time

//���ڷ��ͻ����� 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
#ifdef USART3_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������150ms�������ǲ���һ������������.
//���2���ַ����ռ������150ms,����Ϊ����1����������.Ҳ���ǳ���150msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART3_RX_STA=0;
//void USART3_IRQHandler(void)
//{
//	u8 res;	    
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
//	{	 

//		res =USART_ReceiveData(USART3);		
//		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//		{ 
//			if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//�����Խ�������
//			{
//				TIM_SetCounter(TIM7,0);//���������        				 
//				if(USART3_RX_STA==0)		
//					TIM_Cmd(TIM7, ENABLE);  //ʹ�ܶ�ʱ��7 
//				USART3_RX_BUF[USART3_RX_STA++]=res;		//��¼���յ���ֵ	 
//			}else 
//			{
//				USART3_RX_STA|=1<<15;	//ǿ�Ʊ�ǽ������
//			} 
//		}  	
//	}										 
//}

//===============================================
//=================�жϺ���
//===============================================
void USART3_IRQHandler(void)
{
	static u8 data=0;
	static u8 step=0;
	//
	//������󣬱��봦��,����ᵼ������
	if(0x0008&USART3->SR)
	{
		data=USART3->DR;
		step=0;
	}
	else if(0x0020&USART3->SR)
	{
		//�����ݻ��Զ�����жϱ�־λ
		data=USART3->DR;
		//==================================================
		//==================================================
		F407USART3_buffWrite(data);  // write data to buffer
	}
}

#endif	
//��ʼ��IO ����3
//bound:������	  
void usart3_init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

 	USART_DeInit(USART3);  //��λ����3
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOD9����ΪUSART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOD8����ΪUSART3	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD8��GPIOD9��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��GPIOD9����GPIOD8
	
	USART_InitStructure.USART_BaudRate = bound;//������ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
 
	USART_Cmd(USART3, ENABLE);               //ʹ�ܴ��� 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	TIM7_Int_Init(1500-1,8400-1);		//150ms�ж�
	USART3_RX_STA=0;		//����
	TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��7
  	

}

void F407USART3_SendByte(u8 Data)
{ 
	while (!(USART3->SR & USART_FLAG_TXE)); 
	USART_SendData(USART3, Data); 
}
void F407USART3_SendBytes(u8 *Data,u16 leng)
{ 
	u16 i=0;
	for(i=0;i<leng;i++)
	{
		F407USART3_SendByte(*(Data+i));
	}
}
void F407USART3_SendString(char *str)
{
	u16 i=0;
	while(*(str+i)!=0)
	{
		F407USART3_SendByte(*(str+i));
		i++;
	}
}

#if(F407USART3_RECEIVE_BUFF_ENABLE==1)  // Reveive flag

u8 F407USART3_buff[F407USART3_RECEIVE_BUFF_SIZE];
volatile u16 F407USART3_buffHead=0;
volatile u16 F407USART3_buffEnd=0;
volatile u8 F407USART3_buffOverFlag=0;


u8 F407USART3_buffWrite(u8 data)
{
	if(F407USART3_buffHead!=F407USART3_buffEnd||F407USART3_buffOverFlag==0)
	{
		F407USART3_buff[F407USART3_buffEnd]=data;
		F407USART3_buffEnd++;
		if(F407USART3_buffEnd>=F407USART3_RECEIVE_BUFF_SIZE)
		{
			F407USART3_buffEnd=0;
			F407USART3_buffOverFlag=1;
		}
		return 1;
	}
	return 0;
}
u8 F407USART3_buffWrites(u8 *data,u16 length)
{
	u16 i=0;
	for(i=0;i<length;i++)
	{
		if(F407USART3_buffWrite(*(data+i))==0)
		{
			return 0;
		}
	}
	return i;
}
u8 F407USART3_buffRead(u8 *data)
{
	if(F407USART3_buffHead!=F407USART3_buffEnd||F407USART3_buffOverFlag==1)
	{
		*data=F407USART3_buff[F407USART3_buffHead];
		F407USART3_buffHead++;
		if(F407USART3_buffHead>=F407USART3_RECEIVE_BUFF_SIZE)
		{
			F407USART3_buffHead=0;
			F407USART3_buffOverFlag=0;
		}
		return 1;
	}
	return 0;
}
u16 F407USART3_buffReads(u8 *data,u16 length)
{
	u16 i=0;
	for(i=0;i<length;i++)
	{
		if(F407USART3_buffRead(data+i)==0)return 0;
	}
	return length;
}
u16 F407USART3_buffLength()
{
	return F407USART3_RECEIVE_BUFF_SIZE*F407USART3_buffOverFlag+F407USART3_buffEnd-F407USART3_buffHead;
}

#endif

//void U3_Send_Byte(char byte)
//{
//	USART3->DR=byte;  
//	while((USART3->SR&0X40)==0);//ѭ������,ֱ��������� 
//}

//void U3_Send_Date(u8* dbuf,u16 len)
//{
//	u16 j;
//	for(j=0;j<len;j++)//ѭ����������
//	{
//		USART3->DR=dbuf[j]; 
//		while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
//		// ���Գ��򣬹۲����ݱ仯		
//		USART1->DR=dbuf[j];  
//		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	}	
//}


//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]); 	 //�������ݵ�����3 
	}
	
}
