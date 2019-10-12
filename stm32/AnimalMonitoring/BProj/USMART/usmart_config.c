#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 	 
#include "fattester.h"  
#include "draft.h"
#include "transport.h"
#include "power.h"
extern void usart_set_task(int v_task);
//extern void sent_test_data(char* topic,int tralen);
//extern void usart_transport_test(void);	//���Է�������
//extern void open_WiFi(char sw);	//�򿪵�Դ/�ر�WIFI
//extern void network_init(void); //WIFI����·����

//extern void open_USB(char sw);
//extern void connect_USB(char sw);
//extern void takephoto_Cam(char sw);

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{

#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif	 
	(void*)mf_mount,"u8 mf_mount(u8* path,u8 mt)", 
	(void*)mf_open,"u8 mf_open(u8*path,u8 mode)", 
	(void*)mf_close,"u8 mf_close(void)", 
	(void*)mf_read,"u8 mf_read(u16 len)", 
	(void*)mf_write,"u8 mf_write(u8*dat,u16 len)", 
	(void*)mf_opendir,"u8 mf_opendir(u8* path)", 
	(void*)mf_closedir,"u8 mf_closedir(void)", 
	(void*)mf_readdir,"u8 mf_readdir(void)", 
	(void*)mf_scan_files,"u8 mf_scan_files(u8 * path)", 
	(void*)mf_showfree,"u32 mf_showfree(u8 *drv)", 
	(void*)mf_lseek,"u8 mf_lseek(u32 offset)", 
	(void*)mf_tell,"u32 mf_tell(void)", 
	(void*)mf_size,"u32 mf_size(void)", 
	(void*)mf_mkdir,"u8 mf_mkdir(u8*pname)", 
	(void*)mf_fmkfs,"u8 mf_fmkfs(u8* path,u8 mode,u16 au)", 
	(void*)mf_unlink,"u8 mf_unlink(u8 *pname)", 
	(void*)mf_rename,"u8 mf_rename(u8 *oldname,u8* newname)", 
	(void*)mf_getlabel,"void mf_getlabel(u8 *path)", 
	(void*)mf_setlabel,"void mf_setlabel(u8 *path)", 
	(void*)mf_gets,"void mf_gets(u16 size)", 
	(void*)mf_putc,"u8 mf_putc(u8 c)", 
	(void*)mf_puts,"u8 mf_puts(u8*c)", 		
	// ��������
//	(void*)usart_set_task,"void usart_set_task(int v_task)", 
//	
//	(void*)USB_CONNECT_ON,"void USB_CONNECT_ON(void)", 
//	(void*)USB_CONNECT_OFF,"void USB_CONNECT_OFF(void)",

//	(void*)USB_Connecting,"void USB_Connecting(u8 ctrl)",
//	(void*)USB_Photograph,"void USB_Photograph(void)",
//		
//	(void*)mydump_file,"u8 mydump_file(void)", 
//	(void*)mysend_allpic,"u8 mysend_allpic(void)",	
////	(void*)mf_scan_files,"u8 mf_scan_files(u8 * path)", 	 
//	//(void*)ai_load_picfile,"u8 ai_load_picfile(const u8 *filename,u16 x,u16 y,u16 width,u16 height,u8 fast)", 	 
//	//(void*)minibmp_decode,"u8 minibmp_decode(u8 *filename,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode)", 	 
//		
////	(void*)takephoto,"void takephoto(u8 ctrl)",
////	(void*)Usb_connected,"void Usb_connected(u8 ctrl)",
////	(void*)uf_scan_files,"u8 uf_scan_files(u8 * path)", 
//	//����һ������
//	//(void*)sent_test_data,"void sent_test_data(char* topic,int tralen)",	
//	//����ͼƬ����
//	//(void*)SDCARD_read_pic_send,"u8 SDCARD_read_pic_send(u8 * path)", 
//	(void*)send_picture,"unsigned char send_picture(char* file_path, int max_len, int send_picture_cnt);", 
//	//���Է�������
//	//(void*)usart_transport_test,"void usart_transport_test(void)",
//	//��������
//	(void*)transport_open,"int transport_open(void))", 
//	//�Ͽ�����
//	(void*)transport_close,"int transport_close(void))", 
//	//�򿪵�Դ/�ر�WIFI
//	(void*)open_WiFi,"void open_WiFi(char sw)", 
//	//WIFI����·����
//	(void*)network_init,"void network_init(void))", 
//	//��USB��Դ
//	(void*)open_USB,"void open_USB(char sw)", 
//	//������USB
//	(void*)connect_USB,"void connect_USB(char sw)", 
//	//�������
//	(void*)takephoto_Cam,"void takephoto_Cam(char sw)", 
};						
					  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















