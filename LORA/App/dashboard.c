#include "humanbodyinduction.h"
#include "dashboard.h"
#include "timer.h"
#include "rgbled.h"
#include "cJSON.h"
#include "lcd.h"
#include "text.h"
#include "delay.h"
_MYDATA _mydata;

void is_alert(){
	
	if(_mydata.traffic>_mydata.traffic_max){
		BEEP = 1;
		delay_ms(100);
		BEEP=0;
	}
		
	if(_mydata.light<_mydata.light_max){
		ROADLIGHT = 1;
	}else{
		ROADLIGHT = 0;
	}

}

// 收集传感器的数据 ok
void collect_sensor_data(){
	
	_mydata.light = Lsens_Get_Val();
	if(_mydata.run_time>60){
		_mydata.traffic = _mydata.total_numbers*60/_mydata.run_time;
	}else{
		_mydata.traffic = 0;
	}

	is_alert();
	
}
// 上传数据到云端 ok
void upload_data(void){
	
	u3_printf("{\"light\":%d}",_mydata.light);
	//delay_ms(50);
	u3_printf("{\"rgb\":%d}",_mydata.rgb);
	//delay_ms(50);
	u3_printf("{\"traffic_max\":%d}",_mydata.traffic_max);
	//delay_ms(50);
	u3_printf("{\"total_numbers\":%d}",_mydata.total_numbers);
	//delay_ms(50);
	u3_printf("{\"traffic_max\":%d}",_mydata.traffic_max);
	//delay_ms(50);
	u3_printf("{\"traffic\":%d}",_mydata.traffic);
	//delay_ms(50);
	u3_printf("{\"model\":%d}",_mydata.model);
	//delay_ms(50);
	u3_printf("{\"beep\":%d}",_mydata.beep);
	//delay_ms(50);
	u3_printf("{\"road_light\":%d}",_mydata.road_light);	
	//delay_ms(50);
	u3_printf("{\"alarm_light\":%d}",_mydata.alarm_light);
	//delay_ms(50);
	u3_printf("{\"light_max\":%d}",_mydata.light_max);
	//delay_ms(50);
	u3_printf("{\"alarm\":%d}",_mydata.alarm);
	//delay_ms(50);
	u3_printf("{\"run_time\":%d}",_mydata.run_time);
	//delay_ms(50);
	u3_printf("{\"upload_span_time\":%d}",_mydata.upload_span_time);
	//delay_ms(50);	
	
}




void show_dashboard_ui(void){	
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	POINT_COLOR=RED;
	Show_Str_Mid(0,40,"在线监控",16,lcddev.width); 
	POINT_COLOR=BLUE;
	
	int start=60;
	int stop = 30;	
	
	Show_Str(10,start+stop*0,400,16,"光敏电阻",16,0);
	Show_Str(10,start+stop*1,400,16,"通过次数",16,0);
	Show_Str(10,start+stop*2,400,16,"人流量（人/分钟）",16,0);
	Show_Str(10,start+stop*3,400,16,"监控时间（s）",16,0);	
	Show_Str(10,start+stop*4,400,16,"上传数据时间间隔",16,0);
	Show_Str(10,start+stop*5,400,16,"光照阈值",16,0);	
	Show_Str(10,start+stop*6,400,16,"人流量阈值",16,0);	
	Show_Str(10,start+stop*7,400,16,"当前模式",16,0);	
	
}

// 
void show_data(){
	int start=60;
	int stop = 30;	
	LCD_ShowxNum(150,start+stop*0,_mydata.light,3,16,0);// 亮度 	
	
	LCD_ShowxNum(150,start+stop*1,_mydata.total_numbers,3,16,0);// 总人数

	LCD_ShowxNum(150,start+stop*2,_mydata.traffic,3,16,0);//  人流量

	LCD_ShowxNum(150,start+stop*3,_mydata.run_time,3,16,0);// 时间
	LCD_ShowxNum(150,start+stop*4,_mydata.upload_span_time,3,16,0);//
	LCD_ShowxNum(150,start+stop*5,_mydata.light_max,3,16,0);//
	LCD_ShowxNum(150,start+stop*6,_mydata.traffic_max,3,16,0);//
	
	
	u8 *model ;
	if(_mydata.model==2){
		model = "警察模式";
	}else if(_mydata.model==1){
		model = "路灯模式";
	}else{
		model = "节日模式";
	}
	
	
	Show_Str(150,start+stop*7,200,16,model,16,0);

}

// 收集云端的数据  ok
void  college_and_handle(){
	
	LCD_Fill(0,0,lcddev.width,24,WHITE);
	Show_Str_Mid(0,0,USART3_RX_BUF,12,lcddev.width); 
	
	cJSON *json = cJSON_Parse((char*)USART3_RX_BUF);
	

	if(json){

		cJSON *rgb = cJSON_GetObjectItem(json, "rgb");

		if(rgb){
			_mydata.rgb=rgb->valueint;
			rgb_set_color(rgb->valueint);
		}
		
		cJSON *traffic_max = cJSON_GetObjectItem(json, "traffic_max");
		if(traffic_max){	
			_mydata.traffic_max =traffic_max-> valueint;
		}
			
		cJSON *model = cJSON_GetObjectItem(json, "model");
		if(model){	
			if(model->valueint==1 || model->valueint==2){
				_mydata.model =model-> valueint;
			}else{
				_mydata.model =0;
			}
			
		}
		
		
		cJSON *beep = cJSON_GetObjectItem(json, "beep");
		if(beep){	
			_mydata.beep =beep-> valueint;
			BEEP = beep->valueint;
			delay_ms(100);	
		}
		
		
		cJSON *road_light = cJSON_GetObjectItem(json, "road_light");
		if(road_light){	
			_mydata.road_light =road_light-> valueint;
			
			ROADLIGHT = road_light-> valueint;
		}

		
		cJSON *alarm_light = cJSON_GetObjectItem(json, "alarm_light");
		if(alarm_light){	
			_mydata.alarm_light =alarm_light-> valueint;
			
			ALARMLIGHT = alarm_light-> valueint;
		}
		
		cJSON *light_max = cJSON_GetObjectItem(json, "light_max");
		if(light_max){	
			_mydata.light_max =light_max-> valueint;
			
			ROADLIGHT = light_max-> valueint;
		}
		
		cJSON *upload_span_time = cJSON_GetObjectItem(json, "upload_span_time");
		if(upload_span_time){	
			_mydata.upload_span_time =upload_span_time-> valueint;
			
		}
		
		cJSON_Delete(json);
	}
}

// 数据接收
void dashboard_rece_rata(void)
{
	u16 len=0;
	//有数据
	if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;
		USART3_RX_STA=0;
		college_and_handle();					
		memset((char*)USART3_RX_BUF,0x00,len);//清除数据的BUf	
	}
}


void dashboard_process(void){
	u8 key=0;
	//int adcx;
	LoRa_Set();
	show_dashboard_ui();
	
	int rgb = 0;
	while(1){
		
		show_data();
		key = KEY_Scan(0);
		
		if(key==WKUP_PRES)//返回主页
		{
			
			break;
		}
		
		
		else if(key==KEY1_PRES)//立即发送数据
		{
			  if(!LORA_AUX)//
			  {
				  Lora_mode=2;//
				  upload_data();
	  }
				
		}else if(key==KEY0_PRES)//切换彩灯颜色
		{
			
			rgb_set_color(rgb);
			rgb++;
			if(rgb==8)
				rgb=0;
			
		}
		dashboard_rece_rata();
	}

}


void init_data(){
	_mydata.light = Lsens_Get_Val();
	_mydata.rgb = 0;
	_mydata.traffic_max = 5;
	_mydata.total_numbers=0;
	_mydata.model=0;
	_mydata.beep=0;
	_mydata.light_max = 50;
	_mydata.road_light = 0;
	_mydata.alarm_light = 0;
	_mydata.alarm = 0;
	_mydata.run_time = 0;
	_mydata.upload_span_time = 10;
}

