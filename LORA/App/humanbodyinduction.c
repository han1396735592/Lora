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
	
	//u3_printf("�������� ���ǵ�%d����ͨ��\n",count);
	//delay_ms(100);	
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ  
} 



void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM6ʱ��ʹ��    
	
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM6�ж�,��������ж�
	
	TIM_Cmd(TIM6,ENABLE);//������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}



int _sss = 0;

void TIM6_IRQHandler(void)
{ 		
	// u3_printf("TIM2_IRQHandler");	
	if(TIM6->SR&0X0001)//����ж�
	{		
		_sss ++;
		if(_sss%2==0){
			_mydata.run_time+=1;
		}
		
		collect_sensor_data(); //�ռ�����������
		
		
		if(_mydata.run_time%_mydata.upload_span_time==0){
			upload_data(); //�ϴ�����	
		}
		
		//upload_data();
	}	
	
	
	

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update );
}





void gpio_f1_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
		//��ʼ�� 	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PF1���ó����룬Ĭ������	  
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF.1
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);	//ʹ�ܸ��ù���ʱ��

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
	TIM6_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms  
	gpio_f1_init();
}	



