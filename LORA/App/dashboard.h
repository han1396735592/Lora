#ifndef _DASHBORAD_H
#define _DASHBORAD_H
#include "lora_app.h"
#include "lora_ui.h"
#include "lora_cfg.h"
#include "usart3.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "text.h"
#include "key.h"
#include "lsens.h"
#include "beep.h" 	 

#include "humanbodyinduction.h"


typedef struct {
	int light;			//����
	
	int rgb;				//RGBled �ʵ�
	int traffic_max;		//��������ֵ
	int total_numbers;	//ͨ����������
	int traffic;			//������
	u8 model;			// ģʽ
	int beep;			// ������
	int road_light;		//·��
	int alarm_light;		//������
	int light_max;		//������ֵ
	int alarm;			//����ӵ������
	
	int run_time;       //����ʱ��
	int upload_span_time;  // �ϴ����ݵļ��ʱ��
}_MYDATA;



void dashboard_ui(void);

void dashboard_process(void);

void dashboard_rece_rata(void);

void init_data(void);


void college_and_handle(void);

void collect_sensor_data(void);

void upload_data(void);

#endif





