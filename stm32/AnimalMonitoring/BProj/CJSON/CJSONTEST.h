#ifndef _CJSONTEST_H_
#define _CJSONTEST_H_

void cjson_task_req(char *msg, int id);  // �������� 
void cjson_task_sdata(char *msg, int id);  // ���ʹ��������� 

void cjson_parse_sdataresp(char *msg);  // ����������ȷ������ 
void cjson_parse_tresp(char *msg);  // ���������б� 

#endif
