// ��ʱ��ȡ����������
fopen();
f_write(mes);
fclose();

// ��ʱ��ȡ��������Ϣ
fopen();
size = read_size();
f_read(size)

    send_pic();
waitresponse();
if (ok)
    ����������Ϣ
else
{
    resent();
    while (1)
    {
        cnt++ if (cnt > 3)
        {
            if (flag == 0)
            {
                flag = 1;
                restart();
                cnt = 0;
            }
            else
            {
                return Network_anomaly;
                // ��һ����Ҫ���ݻ�����
                // �����´λ���ʱ��

                // check �Ƿ����ã��ֶ����ƣ�
                // 44444444/DebugChangle
                // analy(json)
                // not sleep induction 10min
                // takephoto() & updata
                // take sen() & updata
            }
        }
    }
}

send_pic();
// ÿ�δ��ͽ���������Ƭת������һ���ļ�����
// ���������������쳣������resetӲ��һ��
// ����Ȼʧ��
return Network_anomaly;
//Ȼ�󱣴��ֳ�
//1
�����С��ݴ�����ת�Ƶ����ط�����
    //2
    save_resent_area()
{
    f_open(cache_file.data)
        len = queue_leng(Q_resent)
            visit() : f_printf(uid, pid, packid\r\n);
    Dequeue();
}
//3
�¸����㻽�ѣ����㻽�ѵ�Ч�����Զ��׼��ģ����̶ȱ����ͻ��

// ��Ӧ�ģ�������ȡ������
if (f_open == 0)
{
    f_scanf()
        ENQueue()
    //�����
}

1.ԭ��A ���flag 2.ԭ��B �ɼ�sensor 3.ԭ��C ��������������ѹ 4.ԭ��D ��ȡ��ʱ�� 3.ԭ��C ���ʹ��������ݣ��򵥴���������������� 4.ԭ��D ���ʹ���������

    ����һЩflag,
    �����龰�ж��Ƿ��� 1.�������� 2.ת�淢��

    ����check
        д���ʱ��check
    ?

    // ps �����Ƿ�ɹ����ļ�����д�뵽backfile

    //ÿ��һ���ļ�


// ����resent����
ÿ�ζ�ȡ��һ��ͼƬ֮ǰ�����Ƿ���һ��ͼƬ֮ʱ��

��Ҫһ���������浱ǰ��һЩ������Ϣ��
u16 now_pic_id
u16 now_pack_id

now_pic_id 

if(Q_resent == full)
{
    d=get_head_of_Q(resent_Q)
    if(now_pic_id == d.pic_id)
    {
        
        
    }
    else
    {
        f_close(now_pic_id);
        f_open(d.pic_id);
    }

    offset = caculate_offset(d.pack_id);
    pack_len = caculate_pack_len(d.pack_id)
        change_pp(offset);
    f_read(file, pp, len);

    send();
    // �˴����ط�����

    if(success)
        DeQueue(d);
        EnQueue(stage)
;
EnQueue(stage)
            check Q_resent
}

then

    fn ��ͨ��ɨ����õģ�
    id_pic = srt2integer_pro(fn);
"0:INBOX/

    strcat((char *)srcpathname, (const char *)psrc); // ???????????????��??

DIR *srcdir = 0; // ԴĿ¼

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

srcdir = (DIR *)mymalloc(SRAMIN, sizeof(DIR)); // �����ڴ�

if (srcdir == NULL || dstdir == NULL || finfo == NULL)
{
    res = 100;
}

if (res == 0)
{

    dstpathname = mymalloc(SRAMIN, MAX_PATHNAME_DEPTH);
    fn_t = mymalloc(SRAMIN, 30); // #speical


    fn_t = mymalloc(SRAMIN, 30); // #speical

    u8 mf_dcopy(u8 * psrc, u8 * pdst, u8 fwmode)
    {
        u8 cnt = 0;

        u8 res = 0;
        DIR *srcdir = 0;    // ԴĿ¼

        FILINFO *finfo = 0; // �ļ���Ϣ
        u8 *fn = 0;         // ���ļ���

        u8 *dstpathname = 0; // Ŀ���ļ���·��+�ļ���
        u8 *srcpathname = 0; // Դ�ļ���·��+�ļ���

        u8 *fn_t = 0; // #speicalĿ���ļ���·��+�ļ���

        u16 dstpathlen = 0; // Ŀ��·������
        u16 srcpathlen = 0; // Դ·������



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
                srcpathname[0] = 0;

                strcat((char *)srcpathname, "0:INBOX"); // ����ԭʼԴ�ļ�·��
                strcat((char *)srcpathname, (const char *)"/"); // Դ·����б��
                sprintf((char *)fn_t, "IMAG%05d.JPG", id_in_flash);
                strcat((char *)srcpathname, (const char *)fn_t); // Ŀ��·�����ļ���
                printf("file name: %s\r\n", srcpathname);

                

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
