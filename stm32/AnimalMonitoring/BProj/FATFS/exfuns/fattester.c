#include "fattester.h"	 
#include "sdio_sdcard.h"
#include "usmart.h"
#include "usart.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
#include "string.h"
#include "mqttApp.h"
#include "stmflash.h"  // for pic id
#include "iwdg.h" // watchdog
//Ϊ����ע�Ṥ����	 
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
u8 mf_mount(u8* path,u8 mt)
{		   
	return f_mount(fs[2],(const TCHAR*)path,mt); 
}
//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
u8 mf_open(u8*path,u8 mode)
{
	u8 res;	 
	res=f_open(file,(const TCHAR*)path,mode);//���ļ���
	return res;
} 
//�ر��ļ�
//����ֵ:ִ�н��
u8 mf_close(void)
{
	f_close(file);
	return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
u8 mf_read(u16 len)
{
	u16 i,t;
	u8 res=0;
	u16 tlen=0;
	printf("\r\nRead file data is:\r\n");
	for(i=0;i<len/512;i++)
	{
		res=f_read(file,fatbuf,512,&br);
		if(res)
		{
			printf("Read Error:%d\r\n",res);
			break;
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}
	}
	if(len%512)
	{
		res=f_read(file,fatbuf,len%512,&br);
		if(res)	//�����ݳ�����
		{
			printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	printf("Read data over\r\n");	 
	return res;
}
//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
u8 mf_write(u8*dat,u16 len)
{			    
	u8 res;	   					   

	printf("\r\nBegin Write file...\r\n");
	printf("Write data len:%d\r\n",len);	 
	res=f_write(file,dat,len,&bw);
	if(res)
	{
		printf("Write Error:%d\r\n",res);   
	}else printf("Writed data len:%d\r\n",bw);
	printf("Write data over.\r\n");
	return res;
}

//��Ŀ¼
 //path:·��
//����ֵ:ִ�н��
u8 mf_opendir(u8* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//�ر�Ŀ¼ 
//����ֵ:ִ�н��
u8 mf_closedir(void)
{
	return f_closedir(&dir);	
}
//���ȡ�ļ���
//����ֵ:ִ�н��
u8 mf_readdir(void)
{
	u8 res;
	char *fn;			 
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
	res=f_readdir(&dir,&fileinfo);//��ȡһ���ļ�����Ϣ
	if(res!=FR_OK||fileinfo.fname[0]==0)
	{
		myfree(SRAMIN,fileinfo.lfname);
		return res;//������.
	}
#if _USE_LFN
	fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
	fn=fileinfo.fname;;
#endif	
	printf("\r\n DIR info:\r\n");

	printf("dir.id:%d\r\n",dir.id);
	printf("dir.index:%d\r\n",dir.index);
	printf("dir.sclust:%d\r\n",dir.sclust);
	printf("dir.clust:%d\r\n",dir.clust);
	printf("dir.sect:%d\r\n",dir.sect);	  

	printf("\r\n");
	printf("File Name is:%s\r\n",fn);
	printf("File Size is:%d\r\n",fileinfo.fsize);
	printf("File data is:%d\r\n",fileinfo.fdate);
	printf("File time is:%d\r\n",fileinfo.ftime);
	printf("File Attr is:%d\r\n",fileinfo.fattrib);
	printf("\r\n");
	myfree(SRAMIN,fileinfo.lfname);
	return 0;
}			 

 //�����ļ�
 //path:·��
 //����ֵ:ִ�н��
u8 mf_scan_files(u8 * path)
{
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	{	
		printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
	        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			printf("%s/", path);//��ӡ·��	
			printf("%s\r\n",  fn);//��ӡ�ļ���	  
		} 
    }	  
	myfree(SRAMIN,fileinfo.lfname);
    return res;	  
}
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
u32 mf_showfree(u8 *drv)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
	    fre_sect = fre_clust * fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//������С��10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n����������:%d KB\r\n"
		           "���ÿռ�:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    printf("\r\n����������:%d MB\r\n"
		           "���ÿռ�:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
u8 mf_lseek(u32 offset)
{
	return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
u32 mf_tell(void)
{
	return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
u32 mf_size(void)
{
	return f_size(file);
} 
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_mkdir(u8*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//path:����·��������"0:"��"1:"
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
u8 mf_fmkfs(u8* path,u8 mode,u16 au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//��ʽ��,drv:�̷�;mode:ģʽ;au:�ش�С
} 
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_unlink(u8 *pname)
{
	return  f_unlink((const TCHAR *)pname);
}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
u8 mf_rename(u8 *oldname,u8* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"  
void mf_getlabel(u8 *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		printf("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
		printf("����%s �����к�:%X\r\n\r\n",path,sn); 
	}else printf("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"  
void mf_setlabel(u8 *path)
{
	u8 res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		printf("\r\n�����̷����óɹ�:%s\r\n",path);
	}else printf("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
} 

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void mf_gets(u16 size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//û�����ݶ���
	else
	{
		printf("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}
//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
u8 mf_putc(u8 c)
{
	return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
u8 mf_puts(u8*c)
{
	return f_puts((TCHAR*)c,file);
}

// �ܹ��Զ���ȡ�ַ����е�����
u32 srt2integer_pro(u8 *str)
{
    u8 len = 0;
    u8 i = 0;
    u8 buf[20] = {0};
    u32 wi = 1;
    u32 sum = 0;

    while (str[i])
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            buf[len] = str[i];
            //printf("%c",buf[len]);
            len++;
        }
        i++;
    }

    for (i = 0; i < len; i++)
    {
        sum = sum + (buf[len - 1 - i] - '0') * wi;
        wi *= 10;
    }
    return sum;
}



/** 
 * @brief send pictures of assigned path(folder)
 * @param psrc	Դ�ļ���
 * @param pdst	�鵵�ļ���
 * @param fwmode	
 *		<>1<> ����ģʽ
 *		<>0<> �Ǹ���ģʽ
 *
 * @return num of files
 * @note  max 20
 */
u8 mf_send_pics(u8 *psrc, u8 *pdst, u8 fwmode)
{
#define MAX_PATHNAME_DEPTH 100 + 1 //���Ŀ���ļ�·��+�ļ������
	u8 cnt = 0;
	
	u8 res = 0;
    DIR *srcdir = 0;    // ԴĿ¼
    DIR *dstdir = 0;    // ԴĿ¼
    FILINFO *finfo = 0; // �ļ���Ϣ
    u8 *fn = 0;         // ���ļ���

    u8 *dstpathname = 0; // Ŀ���ļ���·��+�ļ���
    u8 *srcpathname = 0; // Դ�ļ���·��+�ļ���

    u16 dstpathlen = 0; // Ŀ��·������
    u16 srcpathlen = 0; // Դ·������
	u32 id_pic;

    srcdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR)); // �����ڴ�
    dstdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR));
    finfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));

    if (srcdir == NULL || dstdir == NULL || finfo == NULL)
    {
        res = 100;
    }
    if (res == 0)
    {
        finfo->lfsize = _MAX_LFN * 2 + 1;
        finfo->lfname = mymalloc(SRAMIN, finfo->lfsize); // �����ڴ�
        dstpathname = mymalloc(SRAMIN, MAX_PATHNAME_DEPTH);
        srcpathname = mymalloc(SRAMIN, MAX_PATHNAME_DEPTH);
        if (finfo->lfname == NULL || dstpathname == NULL || srcpathname == NULL)
        {
            res = 101;
        }
        if (res == 0)
        {
            dstpathname[0] = 0;
            srcpathname[0] = 0;
            strcat((char *)srcpathname, (const char *)psrc); // ����ԭʼԴ�ļ�·��
            strcat((char *)dstpathname, (const char *)pdst); // ����ԭʼĿ���ļ�·��
            res = f_opendir(srcdir, (const TCHAR *)psrc);    // ��ԴĿ¼
            if (res == 0)                                    // ��Ŀ¼�ɹ�
            {
                res = f_mkdir((const TCHAR *)dstpathname);
                // ����ļ����Ѿ�����,�Ͳ�����.��������ھʹ����µ��ļ���.
                if (res == FR_EXIST)
                {
                    res = 0;
                }
                while (res == 0) // ��ʼ�����ļ�������Ķ���
                {
                    res = f_readdir(srcdir, finfo); // ��ȡĿ¼�µ�һ���ļ�
                    if (res != FR_OK || finfo->fname[0] == 0 || cnt > 3)
                    {
                        break;
                    } // ������/��ĩβ��,�˳�
                    if (finfo->fname[0] == '.')
                    {
                        continue;
                    } // �����ϼ�Ŀ¼
                    fn = (u8 *)(*finfo->lfname ? finfo->lfname : finfo->fname);
					id_pic=srt2integer_pro(fn);
                    // �õ��ļ���
                    dstpathlen = strlen((const char *)dstpathname); // �õ���ǰĿ��·���ĳ���
                    srcpathlen = strlen((const char *)srcpathname); // �õ�Դ·������

                    strcat((char *)srcpathname, (const char *)"/"); // Դ·����б��
                    if (finfo->fattrib & 0X10)                      // ����Ŀ¼   �ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;
                    {
                        continue;
                    }    // ������Ŀ¼
                    else // ��Ŀ¼
                    {
						u8 sta;
                        strcat((char *)dstpathname, (const char *)"/"); // Ŀ��·����б��
                        strcat((char *)dstpathname, (const char *)fn);  // Ŀ��·�����ļ���
                        strcat((char *)srcpathname, (const char *)fn);  // Դ·�����ļ���
                        printf("\r\nmove file %s to %s\r\n", srcpathname, dstpathname);
						//CPU_SR_ALLOC();
						//OS_CRITICAL_ENTER(); //�����ٽ���
						sta = mysend_picture(srcpathname, id_pic);	   // �����ļ�
						//OS_CRITICAL_EXIT();  
						
                        mf_copy(srcpathname, dstpathname, fwmode); // �����ļ�
						mf_unlink(srcpathname);					   // ɾ���ļ�
						printf("rm %s\r\n",srcpathname);
						//mycheck_Queue();
						cnt++;
						if(sta != 0) {
							printf("ERROR mysend_picture:%s,%d\r\n",srcpathname,id_pic);
							break;
						}
                    }
                    srcpathname[srcpathlen] = 0; // ���������
                    dstpathname[dstpathlen] = 0; // ���������
					
					// break;
                }
            }
            myfree(SRAMIN, dstpathname);
            myfree(SRAMIN, srcpathname);
            myfree(SRAMIN, finfo->lfname);
        }
    }
    myfree(SRAMIN, srcdir);
    myfree(SRAMIN, dstdir);
    myfree(SRAMIN, finfo);
    return res;
}

static vu8 mf_id_ctrl;

// �ļ�����
// ��psrc�ļ�,copy��pdst.
// psrc,pdst:Դ�ļ���Ŀ���ļ�
// fwmode:�ļ�д��ģʽ
// 0:������ԭ�е��ļ�
// 1:����ԭ�е��ļ�
u8 mf_copy(u8 *psrc, u8 *pdst, u8 fwmode)
{
    u8 res;
    u16 br = 0;
    u16 bw = 0;
    FIL *fsrc = 0;
    FIL *fdst = 0;
    u8 *fbuf = 0;
    fsrc = (FIL *)mymalloc(SRAMIN, sizeof(FIL)); // �����ڴ�
    fdst = (FIL *)mymalloc(SRAMIN, sizeof(FIL));
    fbuf = (u8 *)mymalloc(SRAMIN, 512);
    
	//ι��
	IWDG_Feed();//ι��
	//
	
	if (fsrc == NULL || fdst == NULL || fbuf == NULL)
    {
        res = 100;
    } // ǰ���ֵ����fatfs
    else
    {
        if (fwmode == 0)
        {
            fwmode = FA_CREATE_NEW;
        } // ������
        else
        {
            fwmode = FA_CREATE_ALWAYS;
        } // ���Ǵ��ڵ��ļ�

        res = f_open(fsrc, (const TCHAR *)psrc, FA_READ | FA_OPEN_EXISTING);
        // ��ֻ���ļ�
        if (res == 0)
        {
            res = f_open(fdst, (const TCHAR *)pdst, FA_WRITE | fwmode);
        }             // ��һ���򿪳ɹ�,�ſ�ʼ�򿪵ڶ���
        if (res == 0) // �������򿪳ɹ���
        {
            while (res == 0) // ��ʼ����
            {
                res = f_read(fsrc, fbuf, 512, (UINT *)&br); // Դͷ����512�ֽ�
                if (res || br == 0)
                {
                    break;
                }
                res = f_write(fdst, fbuf, (UINT)br, (UINT *)&bw); // д��Ŀ���ļ�
                if (res || bw < br)
                {
                    break;
                }
            }
            f_close(fsrc);
            f_close(fdst);
        }
    }
    myfree(SRAMIN, fsrc); // �ͷ��ڴ�
    myfree(SRAMIN, fdst);
    myfree(SRAMIN, fbuf);
    return res;
}

// �õ�·���µ��ļ���
// ����ֵ:0,·�����Ǹ�����.
// ����,�ļ��������׵�ַ
u8* get_src_dname(u8* dpfn)
{
	u16 temp=0;
 	while(*dpfn!=0)
	{
		dpfn++;
		temp++;	
	}
	printf("temp=%d\r\n",temp);
	if(temp<4)return 0; 
	while((*dpfn!=0x5c)&&(*dpfn!=0x2f)&&(*dpfn!=':')) {	
		dpfn--;
		temp--;	//׷����������һ��"\"����"/"�� 
		printf("dpfn[%d]=%c\r\n",temp,*dpfn);
	}
	printf("dpfn=%d\r\n",++temp);
	return ++dpfn;
}

// �ļ��и���
// ��psrc�ļ���,copy��pdst�ļ���.
// pdst:��������"X:"/"X:XX"/"X:XX/XX"֮���.����Ҫʵ��ȷ����һ���ļ��д���
// psrc,pdst:Դ�ļ��к�Ŀ���ļ���
// fwmode:�ļ�д��ģʽ
// 0:������ԭ�е��ļ�
// 1:����ԭ�е��ļ�
u8 mf_dcopy(u8 *psrc, u8 *pdst, u8 fwmode)
{
#define MAX_PATHNAME_DEPTH 100 + 1 //���Ŀ���ļ�·��+�ļ������
    u8 cnt=0;
	
	u8 res = 0;
    DIR *srcdir = 0;    // ԴĿ¼
    DIR *dstdir = 0;    // ԴĿ¼
    FILINFO *finfo = 0; // �ļ���Ϣ
    u8 *fn = 0;         // ���ļ���

    u8 *dstpathname = 0; // Ŀ���ļ���·��+�ļ���
    u8 *srcpathname = 0; // Դ�ļ���·��+�ļ���
	
	u8 *fn_t = 0; // #speicalĿ���ļ���·��+�ļ���

    u16 dstpathlen = 0; // Ŀ��·������
    u16 srcpathlen = 0; // Դ·������

	u16 id_in_flash; // #special
	id_in_flash = STMFLASH_Read_Num(FLASH_SAVE_ADDR); // ##special
    
	srcdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR)); // �����ڴ�
    dstdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR));
    finfo = (FILINFO *)mymalloc(SRAMIN, sizeof(FILINFO));

    if (srcdir == NULL || dstdir == NULL || finfo == NULL)
    {
        res = 100;
    }
    if (res == 0)
    {
        finfo->lfsize = _MAX_LFN * 2 + 1;
        finfo->lfname = mymalloc(SRAMIN, finfo->lfsize); // �����ڴ�
        dstpathname = mymalloc(SRAMIN, MAX_PATHNAME_DEPTH);
        fn_t = mymalloc(SRAMIN, 30); // #speical
		srcpathname = mymalloc(SRAMIN, MAX_PATHNAME_DEPTH);
        if (finfo->lfname == NULL || dstpathname == NULL || srcpathname == NULL)
        {
            res = 101;
        }
        if (res == 0)
        {
            dstpathname[0] = 0;
            srcpathname[0] = 0;
            strcat((char *)srcpathname, (const char *)psrc); // ����ԭʼԴ�ļ�·��
            strcat((char *)dstpathname, (const char *)pdst); // ����ԭʼĿ���ļ�·��
            res = f_opendir(srcdir, (const TCHAR *)psrc); // ��ԴĿ¼
            if (res == 0)                                 // ��Ŀ¼�ɹ�
            {
                res = f_mkdir((const TCHAR *)dstpathname);
                // ����ļ����Ѿ�����,�Ͳ�����.��������ھʹ����µ��ļ���.
                if (res == FR_EXIST)
                {
                    res = 0;
                }
                while (res == 0) // ��ʼ�����ļ�������Ķ���
                {
                    res = f_readdir(srcdir, finfo); // ��ȡĿ¼�µ�һ���ļ�
                    if (res != FR_OK || finfo->fname[0] == 0 || cnt>3)
                    {
                        break;
                    } // ������/��ĩβ��,�˳�
                    if (finfo->fname[0] == '.')
                    {
                        continue;
                    } // �����ϼ�Ŀ¼
                    fn = (u8 *)(*finfo->lfname ? finfo->lfname : finfo->fname);
                    // �õ��ļ���
                    dstpathlen = strlen((const char *)dstpathname); // �õ���ǰĿ��·���ĳ���
                    srcpathlen = strlen((const char *)srcpathname); // �õ�Դ·������
                    strcat((char *)srcpathname, (const char *)"/"); // Դ·����б��
                    if (finfo->fattrib & 0X10)                      // ����Ŀ¼   �ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;
                    {
                        continue;
                    } // ������Ŀ¼
                    else // ��Ŀ¼
                    {
                        strcat((char *)dstpathname, (const char *)"/"); // Ŀ��·����б��
                        strcat((char *)srcpathname, (const char *)fn);  // Դ·�����ļ���
                        sprintf((char *)fn_t, "IMAG%04d.JPG", id_in_flash); 
						strcat((char *)dstpathname, (const char *)fn_t);  // Ŀ��·�����ļ���

                        printf("\r\ncopy file %s to %s\r\n", srcpathname,
                               dstpathname);                       // �����ļ�
                        mf_copy(srcpathname, dstpathname, fwmode); // �����ļ�
						
						id_in_flash++;
						STMFLASH_Write_Num(FLASH_SAVE_ADDR, id_in_flash);
						
						mf_unlink(srcpathname);					   // ɾ���ļ�
						
						cnt++;
						printf("rm %s,cnt=%d\r\n",srcpathname,cnt);
						
                    }
                    srcpathname[srcpathlen] = 0; // ���������
                    dstpathname[dstpathlen] = 0; // ���������
                }
            }
            myfree(SRAMIN, dstpathname);
            myfree(SRAMIN, srcpathname);
            myfree(SRAMIN, finfo->lfname);
			myfree(SRAMIN, fn_t);
        }
    }
    myfree(SRAMIN, srcdir);
    myfree(SRAMIN, dstdir);
    myfree(SRAMIN, finfo);
    return res;
}
