#ifndef _MENU_H
#define _MENU_H
#include "sys.h"

typedef enum
{
	Wait_Interface = 0x10, /****��������*****/
	Menu_Interface = 0x20, /****�˵�����***/
	LED_Interface = 0x30, /****LED����***/
	TEMP_Interface =0x04,/****�¶Ƚ���***/
	Supr_Interface =0x05,
}Interface;

typedef struct {
		u8 chooselist;    		//ѡ����
		Interface middleInter;//����Ŀ¼
		Interface lastInter;	//��һ��Ŀ¼
		Interface nextInter;	//��һ��Ŀ¼
		//char *Menuname[];			//Ŀ¼������ѡ������
}task_menu;

extern void list_choose(void);
extern void Menu_menu(task_menu *Menu_face);
extern void LED_menu(task_menu *Task_face);
extern void TEMP_menu(task_menu *Task_face);
extern void menu_Init(void);













#endif



