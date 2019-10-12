/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "I2C_OLED.h"
#include "myiic.h"
#include "codetab.h"
#include "BC95.h"
#include "algorithm.h"
#include "common.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"

//OLED_TEMP oled_temp = {"FL\0","CMD:\0","MSG:\0","\0","\0","\0","\0","\0","\0","\0",0};


void I2C_OLED_Init(void)
{
	IIC_Init();
}
void WriteCmd(u8 I2C_Command)//д����
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(u8 I2C_Data)//д����
{
	I2C_WriteByte(0x40, I2C_Data);
}

void I2C_WriteByte(u8 addr,u8 data)
{		   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(OLED_ADDRESS);	 //������ַ
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr);   //���ͼĴ�����ַ��0x00��ʾд���0x04��ʾд���ݣ�
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(data);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
//	HAL_Delay(1);	 //10ms	�����ﾡ������ʱ��
//	n_nops(1);
}


void OLED_Init(void)
{
	HAL_Delay(1000); //�������ʱ����Ҫ
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();//����
}


void OLED_SetPos(u8 x, u8 y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(u8 fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(u8 x, u8 y,u8 ch[], u8 TextSize)
{
	u8 c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(u8 x,u8 y,u8 N)
{
	u8 wm=0;
	u16  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//*******************************************************
//�������ƣ�void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 TextSize)
//������������꣬���֡����Ⱥ�����
//�������ã���ʾ���֣������ʾ10λ���֣�0~4294967295
//���ز�������
//*******************************************************
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len,u8 TextSize)
{
	u8 t,temp;
	u8 enshow=0;
	u8 str[10] = {0,0,0,0};

	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				str[t] = ' '; 
				continue;
			}else enshow=1; 
		 	 
		}
	 	str[t] = temp+'0';
	}
	str[t] = 0;
	OLED_ShowStr(x,y,str,TextSize);
}


//*******************************************************
//�������ƣ�MsgDisp
//���������ѡ���rcv��ֱ�ӿ�ʼ���ǻ��п�ʼ��ʾ
//�������ã���ʾ��\n\r����Ϣ
//���ز�������
//*******************************************************
void MsgDisp(u8 linefeed)
{
	u16 i=2;//�����ݵĵ�2����ʼ����Ϊ��0��1���ض���\r\n
//	u8 j = 1;//��������
	u8 row = RCV_START_Y;//�ӵ�һ�п�ʼ��ʾ
	u8 col=0;//�ӵ�0�п�ʼ
	u8 temp;//�����ַ�
	u8 x_now;//��ǰ��xλ��
	taskENTER_CRITICAL();           //�����ٽ���
//	//���msg
//	OLED_ShowStr(RCV_START_X,RCV_START_Y,"                ",1);	//15��
//	while(RCV_START_Y+j<=7)	
//	{
//		OLED_ShowStr(0,RCV_START_Y+j,"                      ",1);//21��
//		j++;
//	}
	while(i < usart.num)
	{
		if(usart.USART_RX_BUF[i]==0x0d&&usart.USART_RX_BUF[i+1]==0x0a)
		{
			i+=2;
			row++;
			col=0;
		}
		else 
		{
			if(usart.USART_RX_BUF[i]>=' '&&usart.USART_RX_BUF[i]<='~')//ֻҪ�ǿ���ʾ���ַ�
				temp = usart.USART_RX_BUF[i];
			else//����ǲ�����ʾ���ַ�������?����
				temp = '?';
			if(linefeed)//������п�ʼ��ʾ
				x_now = RCV_START_X+6*col;
			else//rcv����ֱ��д
			{
				if(row==RCV_START_Y)//��һ�к������Ĳ�һ��
					x_now = RCV_START_X_NoNewLine+6*col;
				else
					x_now = 6*col;
			}
			if(x_now<=120&&row<=7)//��ֹ������ȥ�����¿���
			{
				OLED_ShowStr(x_now,row,&temp,1);
			}
			col++;
			i++;
		}
	}
	taskEXIT_CRITICAL();            //�˳��ٽ���
	
}

//*******************************************************
//�������ƣ�InitDisp
//���������
//�������ã���������ʼ����Ļ��Ϣ
//���ز�������
//�������ڶ�ʱ����ִ��
//*******************************************************
void InitDisp(void)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_CLS();//����
	if(flag.BRD_STA==BRD_ATTACKED)
		OLED_ShowStr(0,0,"OL",1);//�ɹ����ϣ�online	
	else
		OLED_ShowStr(0,0,"FL",1);	//offline
	OLED_ShowStr(0,1,"CMD:",1);	
	OLED_ShowStr(0,2,"Rcv:",1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
	
}
//*******************************************************
//�������ƣ�cmdDisp
//���������
//�������ã���ʾcmd
//���ز�������
//*******************************************************
void cmdDisp(u8 * cmd)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_ShowStr(CMD_START_X,CMD_START_Y,cmd,1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
}
//*******************************************************
//�������ƣ�sgnDisp
//���������
//�������ã���ʾcmd
//���ز�������
//*******************************************************
void sgnDisp(u8 * sgn)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_ShowStr(SGN_START_X,SGN_START_Y,sgn,1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
}
//*******************************************************
//�������ƣ�MsgSentDisp
//���������
//�������ã���ʾ���͵�����
//���ز�������
//*******************************************************
void MsgSentDisp(u8 * msg)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_ShowStr(0,7,msg,1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
}

//*******************************************************
//�������ƣ�intMsgSentDisp
//���������
//�������ã���ʾ���͵�����
//���ز�������
//*******************************************************
void intMsgSentDisp(u16 intmsg)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_ShowNum(0,7, intmsg,6,1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
}
//*******************************************************
//�������ƣ�CMD_GOT_STA_Disp
//���������
//�������ã���ʾ���յ������ݵ�״̬
//���ز�������
//*******************************************************
void CMD_GOT_STA_Disp(void)
{
	taskENTER_CRITICAL();           //�����ٽ���
	switch(cmdgot.status)
	{
		case CMD_GOT_OK:
			sgnDisp("-  GOT_OK  -");break;
		case CMD_GOT_NUM_0:
			sgnDisp("-   NONE   -");break;
		case CMD_GOT_TOO_LONG:
			sgnDisp("-  2 LONG  -");break;
		case CMD_GOT_FORMAT_ERROR:
			sgnDisp("-  FMTERR  -");break;
		default:
			break;
	}
	taskEXIT_CRITICAL();           //�˳��ٽ���
}


//*******************************************************
//�������ƣ�CMD_GOT_Disp
//���������
//�������ã���ʾ���յ�������
//���ز�������
//*******************************************************
void CMD_GOT_Disp(void)
{
	taskENTER_CRITICAL();           //�����ٽ���
	OLED_ShowStr(0,7,cmdgot.lastcmd,1);
	OLED_ShowStr(0,6,cmdgot.str,1);
	taskEXIT_CRITICAL();           //�˳��ٽ���
}


//*******************************************************
//�������ƣ�BrdStaDisp
//���������
//�������ã���ʾ��ǰ���ӵ�״̬����LED
//���ز�������
//*******************************************************
void BrdStaDisp(void)
{
	switch(flag.BRD_STA)
	{
		case BRD_ATTACKNETWORKING://�������ڳ�������
		{
			taskENTER_CRITICAL();           //�����ٽ���
			OLED_ShowStr(0,0,"FL",1);
			taskEXIT_CRITICAL();           //�˳��ٽ���
			break;
		}
		case BRD_ATTACKED://�����Ѿ����ӳɹ�
		{
			taskENTER_CRITICAL();           //�����ٽ���
			OLED_ShowStr(0,0,"OL",1);
			taskEXIT_CRITICAL();           //�˳��ٽ���
			break;
		}
		default:
		{
			break;
		}
	}
}

//*******************************************************
//�������ƣ�Error_Handler_Disp
//�����������ǰ�ļ�������
//�������ã���ʾ������ļ�����������Error_Handler��ʹ��
//���ز�������
//*******************************************************
void Error_Handler_Disp(char * file,int line)
{
	OLED_CLS();//����
	OLED_ShowStr(MSG_START_X,0,"- ERROR -",1);
	OLED_ShowStr(0,2,file,1);
	OLED_ShowNum(0,3,line,5,1);
}

