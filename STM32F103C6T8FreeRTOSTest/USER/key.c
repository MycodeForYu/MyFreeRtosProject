#include "key.h"
#include "delay.h"



u8 key_scan(void)
{
	delay_xms(10);
	if(KEY_Up==0)					return KEY_Up_PRES;
	else if(KEY_Dowm==0)	return KEY_Down_PRES;
	else if(KEY_Left==0)	return KEY_Left_PRES;
	else if(KEY_Right==0) return KEY_Right_PRES;
	else  								return 0;
}




