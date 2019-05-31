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
	int light;			//亮度
	
	int rgb;				//RGBled 彩灯
	int traffic_max;		//人流量阈值
	int total_numbers;	//通过的总人数
	int traffic;			//人流量
	u8 model;			// 模式
	int beep;			// 蜂鸣器
	int road_light;		//路灯
	int alarm_light;		//警报灯
	int light_max;		//光照阈值
	int alarm;			//过度拥挤警报
	
	int run_time;       //运行时间
	int upload_span_time;  // 上传数据的间隔时间
}_MYDATA;



void dashboard_ui(void);

void dashboard_process(void);

void dashboard_rece_rata(void);

void init_data(void);


void college_and_handle(void);

void collect_sensor_data(void);

void upload_data(void);

#endif





