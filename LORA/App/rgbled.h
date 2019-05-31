#ifndef _RGB_LED_H
#define _RGB_LED_H

#include "sys.h"


//LED¶Ë¿Ú¶¨Òå
#define RGB_R PFout(2)	// 
#define RGB_G PFout(3)	// 	
#define RGB_B PFout(4)	// 	


#define ROADLIGHT PFout(5)	// 	
#define ALARMLIGHT PFout(6)	// 	




void rgb_led_init(void);


void rgb_set_color(int rgb);



#endif



