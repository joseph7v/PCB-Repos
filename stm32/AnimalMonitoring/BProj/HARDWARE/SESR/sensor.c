   #include "sensor.h"  // self
#include <stdio.h>   // printf
#include <string.h>  // string func
#include "cJSON.h"   // json praser

#include "spi.h"     // spi
#include "myiic_sht20.h"  	// iic
#include "myiic_max44009.h" // iic
#include "ms5611.h"	 // ms5611
#include "SHT2x.h"   // SHT2x 
#include "max44009.h"// max44009
#include "battery.h"
#include "rtc.h"     // spi
#include "malloc.h"
//extern int32_t BaroPressure;
//extern float getEstimatedAltitude(int32_t baroPressure);
//extern float BaroAlt;
//extern float temp,Humi;	
//extern uint32_t flux;


//{"mt":"sdata","sid":"11","slist":[
//{"time":"2017-01-01 12:09:12","SHT21":"temp","value":"12.0"},
//{"time":"2017-01-01 12:09:12","SHT21":"hum","value":"50.5"},
//{"time":"2017-01-01 12:09:12","MS5611":" pres","value":"10000"},
//{"time":"2017-01-01 12:09:12","MAX44009":"lus","value":"100"},]}

static char now_time[30] = "2019-09-23";//һ���궨��/��������ֵ��Ϊʱ�� 
static float temp = 23.0;
static float hum = 50.2;
static uint32_t lus = 100/3;
int32_t BaroPressure=0;
float BaroAlt = 0.0f;

void Data_Packing_sens(char *msg)
{
	char * output;
	cJSON * root =  cJSON_CreateObject();
	cJSON * item =  cJSON_CreateObject();
	
    cJSON * item_temp =  cJSON_CreateObject();
    cJSON * item_hum =  cJSON_CreateObject();
    cJSON * item_pres =  cJSON_CreateObject();
    cJSON * item_lus =  cJSON_CreateObject();
	cJSON * item_volt =  cJSON_CreateObject();
	cJSON * item_cur =  cJSON_CreateObject();
	cJSON * item_cap =  cJSON_CreateObject();


	// get data
	calendar_get_time(&calendar);
	calendar_get_date(&calendar);
	sprintf(now_time,"%d-%d-%d %d:%d:%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	
	temp=24;
	hum=66;
	//getFluxMeasurement(&	lus);
	lus=30045;
	BaroPressure=100600;
	BaroAlt=300;
	
//	temp=SHT2x_GetTempPoll();
//	hum=SHT2x_GetHumiPoll();
//	getFluxMeasurement(&lus);

//	BaroPressure=MS5611_Compensation_Calcu();
//	BaroAlt=getEstimatedAltitude(BaroPressure);
//	printf("BaroPressure: %d,BaroAlt=%f\r\n",BaroPressure,BaroAlt);	
	
	if(BaroPressure<0 || BaroPressure>158823)
		BaroPressure=98823;
	cJSON_AddItemToObject(root, "mt", cJSON_CreateString("sdata"));//���ڵ������
	cJSON_AddItemToObject(root, "sid", cJSON_CreateString("11"));

	battery.charge_current = 9999;
	battery.total_voltage = 9999;
	battery.remain_capacity = 9999;
	battery_data_anay();  // ��ȡ�����Ϣ

	
	item = cJSON_CreateArray();    
    cJSON_AddItemToObject(root, "slist", item);//������� 
	 
    //SHT21-temp
    cJSON_AddItemToObject(item_temp, "time", cJSON_CreateString(now_time));//���time�ڵ�
    cJSON_AddItemToObject(item_temp, "sensor_type", cJSON_CreateString("temp"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_temp, "sensor_model", cJSON_CreateString("SHT21"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_temp, "value", cJSON_CreateNumber(temp));//���value�ڵ�
    //SHT21-humi
    cJSON_AddItemToObject(item_hum, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_hum, "sensor_type", cJSON_CreateString("hum"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_hum, "sensor_model", cJSON_CreateString("SHT21"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_hum, "value", cJSON_CreateNumber(hum));//���value�ڵ�
	//MS5611-pres 
	cJSON_AddItemToObject(item_pres, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_pres, "sensor_type", cJSON_CreateString("pres"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_pres, "sensor_model", cJSON_CreateString("MS5611"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_pres, "value", cJSON_CreateNumber((int)BaroPressure));//���value�ڵ�
    //MAX44009-lus
	cJSON_AddItemToObject(item_lus, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_lus, "sensor_type", cJSON_CreateString("lus"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_lus, "sensor_model", cJSON_CreateString("MAX44009"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_lus, "value", cJSON_CreateNumber(lus));//���value�ڵ�
    //Battery-total_voltage
	cJSON_AddItemToObject(item_volt, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_volt, "sensor_type", cJSON_CreateString("volt"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_volt, "sensor_model", cJSON_CreateString("Battery"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_volt, "value", cJSON_CreateNumber(battery.total_voltage));//���value�ڵ�
	
	cJSON_AddItemToObject(item_cur, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_cur, "sensor_type", cJSON_CreateString("cur"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_cur, "sensor_model", cJSON_CreateString("Battery"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_cur, "value", cJSON_CreateNumber(battery.charge_current));//���value�ڵ�
	
	cJSON_AddItemToObject(item_cap, "time", cJSON_CreateString(now_time));//���time�ڵ�
	cJSON_AddItemToObject(item_cap, "sensor_type", cJSON_CreateString("cap"));//���SHT21�ڵ�
	cJSON_AddItemToObject(item_cap, "sensor_model", cJSON_CreateString("Battery"));//���SHT21�ڵ�
    cJSON_AddItemToObject(item_cap, "value", cJSON_CreateNumber(battery.remain_capacity));//���value�ڵ�
	
	//�ϲ� 
	cJSON_AddItemToArray(item, item_temp);//root�ڵ������slist�ڵ�
    cJSON_AddItemToArray(item, item_hum);
    cJSON_AddItemToArray(item, item_pres);
    cJSON_AddItemToArray(item, item_lus);
    cJSON_AddItemToArray(item, item_volt);
	cJSON_AddItemToArray(item, item_cur);
	cJSON_AddItemToArray(item, item_cap);
	output=cJSON_Print(root);
	
	printf("%s\n length:%d\n", output,strlen(output));
	sprintf(msg,"%s", output);
	
    cJSON_Delete(root);  
	cJsonFree(output);
}

//{"mt":"sdata","sid":"12","slist":[
//{"time":"2017-01-01 12:09:12","valid":0,"long":2236.9453,"D_long":"N","lati":11408.4790,"D_lati":"E","alti":11408.4790}]}
void Data_Packing_gps(char *msg)
{
	char * output;
	
	cJSON * root =  cJSON_CreateObject();
	cJSON * item =  cJSON_CreateObject();
		
	char now_time[30] = "2019-09-23";//һ���궨��/��������ֵ��Ϊʱ�� 
	char  valid = 0;
	float longitude = 42.89057;
	char*  D_long = "N";
	float latitude = 130.97399;
	char*  D_lati = "E";
	float altitude = 230;

	calendar_get_time(&calendar);
	calendar_get_date(&calendar);
	sprintf(now_time,"%d-%d-%d %d:%d:%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	
	
	cJSON_AddItemToObject(root, "mt", cJSON_CreateString("sdata"));//���ڵ������
	cJSON_AddItemToObject(root, "sid", cJSON_CreateString("12"));
    cJSON_AddItemToObject(root, "slist", item);//root�ڵ������semantic�ڵ�
    
	cJSON_AddItemToObject(item, "time", cJSON_CreateString(now_time));//���time�ڵ�
    cJSON_AddItemToObject(item, "valid", cJSON_CreateBool(valid));//���name�ڵ�
    cJSON_AddItemToObject(item, "Long", cJSON_CreateNumber(longitude));//���long�ڵ�
    cJSON_AddItemToObject(item, "D_long", cJSON_CreateString(D_long));//���D_long�ڵ�
    cJSON_AddItemToObject(item, "Lati", cJSON_CreateNumber(latitude));//���long�ڵ�
    cJSON_AddItemToObject(item, "D_lati", cJSON_CreateString(D_lati));//���D_long�ڵ�
    cJSON_AddItemToObject(item, "Alti", cJSON_CreateNumber(altitude));//���long�ڵ�
	
	output=cJSON_Print(root);
	printf("%s\n length:%d\r\n", output,strlen(output));
	sprintf(msg,"%s", output);
    cJSON_Delete(root);  
	cJsonFree(output); 	
}






