#ifndef __DRAFT_H
#define __DRAFT_H
#include "ff.h"

#define u8 unsigned char 

//void SDCARD_read_test(void);
//void SDCARD_write_test(void);
//void SDCARD_read_write_test(void);

////u8 SDCARD_read_pic_send(char* path);

//u8 send_picture(char* file_path,int max_len);

// ���建��Ĵ�С
#define SDCARD_BUFFER 8192

// U���г�ʼ�ļ��У��Ա���
#define USB_FILE_DIR "1:DCIM/100IMAGE"
// // U���г�ʼ�ļ���λ��
// #define USB_FILE_PATH "1:DCIM/100IMAGE/"
// SD���е��ļ�λ��
#define SD_FILE_DIR "0:STORAGE"
#define SD_FILE_DIR_C "SEND"
#define SD_FILE_DIR_CR "0:SEND"
// U���и��ĺ������,���ܳ����̷�
#define USB_FILE_DIR_C "SEND"
#define USB_FILE_DIR_CR "1:SEND"

u8 dump_file_USB(TCHAR* folder_s, TCHAR* folder_d);
u8 mqtt_send_allpic(TCHAR* folder_s);

u8 mydump_file(void);
u8 mysend_allpic(void);

//u8 WaitDisconnectflag(unsigned short timeout);
//u8 WaitDistinguishflag(unsigned short timeout);
#endif

