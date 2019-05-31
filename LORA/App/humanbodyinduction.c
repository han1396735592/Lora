#include "usart3.h"
#include "humanbodyinduction.h"
#include "timer.h"
#include "sys.h"
#include "led.h"
#include "rgbled.h"
#include "dashboard.h"
#include "text.h"
extern  _MYDATA _mydata;



void EXTI1_IRQHandler(void)
{
	
	_mydata.total_numbers++;
	
	//count++;
	
	//u3_printf("有人来了 这是第%d个人通过\n",count);
	//delay_ms(100);	
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位  
} 



void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM6时钟使能    
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM6中断,允许更新中断
	
	TIM_Cmd(TIM6,ENABLE);//开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}



int _sss = 0;

void TIM6_IRQHandler(void)
{ 		
	// u3_printf("TIM2_IRQHandler");	
	if(TIM6->SR&0X0001)//溢出中断
	{		
		_sss ++;
		if(_sss%2==0){
			_mydata.run_time+=1;
		}
		
		collect_sensor_data(); //收集传感器数据
		
		
		if(_mydata.run_time%_mydata.upload_span_time==0){
			upload_data(); //上传数据	
		}
		
		//upload_data();
	}	
	
	
	

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update );
}





void gpio_f1_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
		//初始化 	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PF1设置成输入，默认下拉	  
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF.1
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);	//使能复用功能时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource1); 
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
		
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
		
 	NVIC_InitTypeDef NVIC_InitStructure;
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 
}




void human_body_induction_init(void){
	TIM6_Int_Init(4999,7199);//10Khz的计数频率，计数5K次为500ms  
	gpio_f1_init();
}	



