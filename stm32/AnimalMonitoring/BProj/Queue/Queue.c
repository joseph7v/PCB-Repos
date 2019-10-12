#include "queue.h"  
#include "stdio.h"    
//#include "stdlib.h"   

// #include "io.h"  
// #include "math.h"  
// #include "time.h"



/* ��ʼ��һ���ն���Q */
Status InitQueue(SqQueue *Q)
{
	Q->front=0;
	Q->rear=0;
	return  SUCCESS;
}

/* ��Q��Ϊ�ն��� */
Status ClearQueue(SqQueue *Q)
{
	Q->front=Q->rear=0;
	return SUCCESS;
}

/* ������QΪ�ն���,�򷵻�SUCCESS,���򷵻�ERROR */
Status QueueEmpty(SqQueue Q)
{ 
	if(Q.front==Q.rear) /* ���пյı�־ */
		return SUCCESS;
	else
		return ERROR;
}

/* ����Q��Ԫ�ظ�����Ҳ���Ƕ��еĵ�ǰ���� */
int QueueLength(SqQueue Q)
{
	return  (Q.rear-Q.front+MAXSIZE)%MAXSIZE;
}

/* �����в���,����e����Q�Ķ�ͷԪ��,������SUCCESS,���򷵻�ERROR */
Status GetHead(SqQueue Q,QElemType *e)
{
	if(Q.front==Q.rear) /* ���п� */
		return ERROR;
	*e=Q.data[Q.front];
	return SUCCESS;
}

/* ������δ���������Ԫ��eΪQ�µĶ�βԪ�� */
Status EnQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* ���������ж� */
		return ERROR;
	Q->data[Q->rear]=e;			/* ��Ԫ��e��ֵ����β */
	Q->rear=(Q->rear+1)%MAXSIZE;/* rearָ�������һλ�ã� */
								/* ���������ת������ͷ�� */
	return  SUCCESS;
}

/* �����в��գ���ɾ��Q�ж�ͷԪ�أ���e������ֵ */
Status DeQueue(SqQueue *Q,QElemType *e)
{
	if (Q->front == Q->rear)			/* ���пյ��ж� */
		return ERROR;
	*e=Q->data[Q->front];				/* ����ͷԪ�ظ�ֵ��e */
	Q->front=(Q->front+1)%MAXSIZE;	/* frontָ�������һλ�ã� */
									/* ���������ת������ͷ�� */
	return  SUCCESS;
}

// �������������� 
//Status visit(QElemType c)
//{
//	printf("%d ",c);
//	return SUCCESS;
//}
///* �Ӷ�ͷ����β���ζԶ���Q��ÿ��Ԫ����� */
//Status QueueTraverse(SqQueue Q)
//{ 
//	int i;
//	i=Q.front;
//	while((i+Q.front)!=Q.rear)
//	{
//		visit(Q.data[i]);
//		i=(i+1)%MAXSIZE;
//	}
//	printf("\n");
//	return SUCCESS;
//}
