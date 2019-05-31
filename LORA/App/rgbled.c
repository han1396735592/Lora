

#include "rgbled.h"

void rgb_led_init(void){

	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	RCC->APB2ENR|=1<<7;    //使能PORTE时钟	
	   	 
   //PE.1 输出高
	
	GPIOF->CRL&=0XFFFFF0FF; 
	GPIOF->CRL|=0X00000300;//PE.2 推挽输出   	 
    GPIOF->ODR|=1<<2;      //PE.2 输出高
	GPIOF->CRL&=0XFFFF0FFF; 
	GPIOF->CRL|=0X00003000;//PE.3 推挽输出   	 
    GPIOF->ODR|=1<<3;      //PB.3 输出高
	
	GPIOF->CRL&=0XFFF0FFFF; 
	GPIOF->CRL|=0X00030000;//PE.4 推挽输出   	 
    GPIOF->ODR|=1<<4;   
	
	RGB_R = 0;
	RGB_G = 0;
	RGB_B = 0;
	
	GPIOF->CRL&=0XFF0FFFFF; 
	GPIOF->CRL|=0X00300000;//PE.5 推挽输出   	 
    GPIOF->ODR|=1<<5;      //PB.5 输出高
	
	GPIOF->CRL&=0XF0FFFFFF; 
	GPIOF->CRL|=0X03000000;//PE.6 推挽输出   	 
    GPIOF->ODR|=1<<6;  
	
	ROADLIGHT  = 0;
	ALARMLIGHT = 0;
											 
}








void rgb_set_color(int rgb){

	switch(rgb){
		case 7:
			RGB_R = 1;
			RGB_G = 1;
			RGB_B = 1;
			break;
		case 6:
			RGB_R = 1;
			RGB_G = 1;
			RGB_B = 0;
			break;
		case 5:
			RGB_R = 1;
			RGB_G = 0;
			RGB_B = 1;
			break;
		case 4:
			RGB_R = 1;
			RGB_G = 0;
			RGB_B = 0;
			break;
		case 3:
			RGB_R = 0;
			RGB_G = 1;
			RGB_B = 1;
			break;
		case 2:
			RGB_R = 0;
			RGB_G = 1;
			RGB_B = 0;
			break;
		case 1:
			RGB_R = 0;
			RGB_G = 0;
			RGB_B = 1;
			break;
		case 0:
			RGB_R = 0;
			RGB_G = 0;
			RGB_B = 0;
			break;
	}

}









