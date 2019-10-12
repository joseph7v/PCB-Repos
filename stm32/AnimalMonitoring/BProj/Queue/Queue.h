#ifndef QUEUE_H
#define QUEUE_H

#define ERROR   0
#define SUCCESS 1

#define MAXSIZE 200 /* �洢�ռ��ʼ������ */

typedef int Status; 

typedef struct
{
	unsigned int uid;  // unique id
	unsigned int pic_id;  // picture id
	unsigned int pack_id;  // pack id
}QElemType;   /* QElemType���� */

/* ѭ�����е�˳��洢�ṹ */
typedef struct
{
	QElemType data[MAXSIZE];
	int front;    	/* ͷָ�� */
	int rear;		/* βָ�룬�����в��գ�ָ�����βԪ�ص���һ��λ�� */
}SqQueue;

Status InitQueue(SqQueue *Q);
Status ClearQueue(SqQueue *Q);
Status QueueEmpty(SqQueue Q);
int QueueLength(SqQueue Q);
Status GetHead(SqQueue Q,QElemType *e);
Status EnQueue(SqQueue *Q,QElemType e);
Status DeQueue(SqQueue *Q,QElemType *e);


#endif
