#include "menu.h"
#include "oled.h"
#include "stm32f10x_gpio.h"

extern int chooselist;
extern Interface face;

task_menu menu[]=
{
	{0,Menu_Interface,Wait_Interface,LED_Interface|TEMP_Interface},//菜单界面
	{0,LED_Interface,Menu_Interface,0x00},												 //LED界面
	{0,TEMP_Interface,Menu_Interface,0x00},												 //TEMP界面
}; 


static int value;
void list_choose(void)
{
	if((face != Wait_Interface)&&(face != TEMP_Interface)&&(face != Supr_Interface))
	{
		value = 2*chooselist;
		OLED_ShowStr(0,0+value,">>",1);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
	{
		chooselist+=1;
		if(chooselist==3)
		{
			if(face ==LED_Interface)
			chooselist = 0;
		}else if(chooselist==4)
			chooselist = 0;
	}
}

void menu_Init(void)
{

			
	
	
}




void Menu_menu(task_menu *Menu_face)
{
	
}

void LED_menu(task_menu *Task_face)
{
	
}

void TEMP_menu(task_menu *Task_face)
{
	
}

	
	
	
	
	




