#ifndef _OLED_H_
#define _OLED_H_
#include "sys.h"

extern void OLED_Config(void);
extern void OLED_WrCmd(u8 WrCmd);
extern void OLDE_Wrdata(u8 WrData);
extern void OLED_Init(void);
extern void OLED_Clear(void);
extern void OLED_SetPos(u8 x, u8 y);
extern void OLED_DrawPoint(u8 x,u8 y,u8 t);
extern void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
extern void OLED_Refresh_Gram(void);
extern void OLED_ON(void);
extern void OLED_OFF(void);
//extern void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N,unsigned char F16x16[]);
extern void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
extern void OLED_ShowInt(unsigned char x, unsigned char y, int Data, unsigned char TextSize);
extern void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize);
extern void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


extern void LIne_Clear(void);

#endif



