#ifndef _MENU_H
#define _MENU_H
#include "sys.h"

typedef enum
{
	Wait_Interface = 0x10, /****待机界面*****/
	Menu_Interface = 0x20, /****菜单界面***/
	LED_Interface = 0x30, /****LED界面***/
	TEMP_Interface =0x04,/****温度界面***/
	Supr_Interface =0x05,
}Interface;

typedef struct {
		u8 chooselist;    		//选择项
		Interface middleInter;//所处目录
		Interface lastInter;	//上一级目录
		Interface nextInter;	//下一级目录
		//char *Menuname[];			//目录中所有选项名字
}task_menu;

extern void list_choose(void);
extern void Menu_menu(task_menu *Menu_face);
extern void LED_menu(task_menu *Task_face);
extern void TEMP_menu(task_menu *Task_face);
extern void menu_Init(void);













#endif



