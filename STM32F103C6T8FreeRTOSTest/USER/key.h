#ifndef _KEY_H_
#define _KEY_H_
#include "sys.h"



#define KEY_Up 			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY_Dowm		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define KEY_Left		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define KEY_Right		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)


#define KEY_Up_PRES 		1
#define KEY_Dowm_PRES		2
#define KEY_Left_PRES		3
#define KEY_Right_PRES	4








#endif


