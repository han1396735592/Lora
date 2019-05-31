#include "sys.h"
#include "beep.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"		
#include "usart3.h"
#include "string.h"
#include "touch.h"
#include "lora_app.h"
#include "lsens.h"
#include "rgbled.h"
#include "dashboard.h"
#include "humanbodyinduction.h"
/************************************************
 ALIENTEK STM32F103开发板 扩展实验23
 ATK-LORA-01无串口线模块测试实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


void app_init(){
	u8 key=0;
	u8 fontok=0; 
  	
	delay_init();	    	                        //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	                        //串口初始化为115200
    LCD_Init();	 
	tp_dev.init();				                    //初始化触摸屏
	LED_Init();
	KEY_Init();
    W25QXX_Init();			  	                    //初始化W25Q128
	my_mem_init(SRAMIN);		                    //初始化内部内存池
	exfuns_init();				                    //为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 	                        //挂载SD卡 
 	f_mount(fs[1],"1:",1); 	                        //挂载FLASH.
	key=KEY_Scan(0);  
	if(key==KEY0_PRES&&((tp_dev.touchtype&0X80)==0))//强制校准
	{
		LCD_Clear(WHITE);		                    //清屏
		TP_Adjust();  			                    //屏幕校准 
		TP_Save_Adjdata();	  
		LCD_Clear(WHITE);		                    //清屏
	}
	fontok=font_init();		                        //检查字库是否OK
	if(fontok||key==KEY1_PRES)	                    //需要更新字库				 
	{
		LCD_Clear(WHITE);		                    //清屏
 		POINT_COLOR=RED;		                    //设置字体为红色	   	   	  
		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
		while(SD_Init())		                    //检测SD卡
		{
			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(60,70,200+60,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(60,70,200,16,16,"SD Card OK");
		LCD_ShowString(60,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");           //从SD卡更新
		while(key)                                 //更新失败		
		{			 		  
			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
			delay_ms(50);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(50);		       
		} 		  
		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
		delay_ms(100);	
		LCD_Clear(WHITE);                         //清屏	       
	}  
	
	while(LoRa_Init())//初始化ATK-LORA-01模块
	{
		Show_Str(40+30,50+20,200,16,"未检测到模块!!!",16,0); 	
		delay_ms(300);
		Show_Str(40+30,50+20,200,16,"                ",16,0);
	}
	Show_Str(40+30,50+20,200,16,"检测到模块!!!",16,0);
	LoRa_Set();
	delay_ms(50);
	BEEP_Init();
	rgb_led_init();
	Lsens_Init();
	init_data();
	human_body_induction_init();
}




//主函数
int main(void)
{	 	 
	app_init();
	
	
	lora_app_run();


}


