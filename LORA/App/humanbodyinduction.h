
#ifndef _HUMAN_BODY_INDUCTION_H
#define _HUMAN_BODY_INDUCTION_H
 
#include "sys.h"
#include "delay.h"
#include "usart3.h"
// ���������Ӧ�ж� ����IO��



# define human_body_induction_input 

void human_body_induction_init(void);



void TIM6_IRQHandler(void);

void TIM6_Int_Init(u16 arr,u16 psc);






#endif

