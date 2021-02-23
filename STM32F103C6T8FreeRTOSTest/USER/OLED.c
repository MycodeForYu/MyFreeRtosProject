#include "OLED.h"
#include "myiic.h"
#include "codetab.h"
#include "menu.h"

#include "stm32f10x.h"

#define OLED_WrCmd WriteCmd   //给写命令函数重命名
#define OLED_WrData WriteDat  //给写数据函数重命名

extern int chooselist;
extern Interface face;

//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];

void OLED_Config()
{
	IIC_Init();
}

void OLED_WrCmd(u8 WrCmd)
{
	IIC_Start();
	IIC_Send_Byte(0X78);	 
	IIC_Wait_Ack();
	IIC_Send_Byte(0X00);	 
	IIC_Wait_Ack();
	IIC_Send_Byte(WrCmd);	 
	IIC_Wait_Ack();
	IIC_Stop();
}

void OLED_WrData(u8 WrData)
{
	IIC_Start();
	IIC_Send_Byte(0X78);	 
	IIC_Wait_Ack();
	IIC_Send_Byte(0X40);	 
	IIC_Wait_Ack();
	IIC_Send_Byte(WrData);	 
	IIC_Wait_Ack();
	IIC_Stop();
}


void OLED_Init()
{
	unsigned int a;
	for(a=0;a<5000;a++);
	OLED_WrCmd(0xAE);//--turn off oled panel
 	OLED_WrCmd(0x00);//---set low column address
 	OLED_WrCmd(0x10);//---set high column address
 	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0xB0);
 	OLED_WrCmd(0x81);//--set contrast control register
 	OLED_WrCmd(0xFF); // Set SEG Output Current Brightness
 	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
 	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
 	OLED_WrCmd(0xa6);//--set normal display
 	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
 	OLED_WrCmd(0x3f);//--1/64 duty
 	OLED_WrCmd(0xd3);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
 	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
 	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
 	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
 	OLED_WrCmd(0xda);//--set com pins hardware configuration
 	OLED_WrCmd(0x12);
 	OLED_WrCmd(0xdb);//--set vcomh
 	OLED_WrCmd(0x40);//Set VCOM Deselect Level
 	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
 	OLED_WrCmd(0x02);//
 	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
 	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
 	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
 	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Clear();//OLED清屏
}


/*************************************************************************/
/*函数功能: 更新显存到OLED                                                 */
/*************************************************************************/
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd(0xb0+i);   //设置页地址（0~7）
		WriteCmd(0x00);      //设置显示位置—列低地址
		WriteCmd(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)WriteDat(OLED_GRAM[n][i]); 
	}   
}

void OLED_Clear(void)
{
  unsigned char i,n;
  for(i=0; i<8; i++)
  {
    OLED_WrCmd(0xb0+i); //设置页地址（0~7）
    OLED_WrCmd(0x00); //设置显示位置—列低地址
    OLED_WrCmd(0x10); //设置显示位置—列高地址
    for(n=0; n<128; n++)	WriteDat(0x00); //写0x00到屏幕寄存器上
  }
}
 
void LIne_Clear(void)
{
  unsigned char i,n;
	if(chooselist==0)
	{
		switch(face)
		{
			case Menu_Interface:	
													i=6;
													break;
			case LED_Interface :
													i=4;
													break;
			default:	
													break;
													
		}
	}else if (chooselist==1)
	{
		i = 0;
	}
	else if (chooselist==2)
	{
		i = 2;
	}
	else if (chooselist==3)
	{
		i = 4;
	}
  {
		if((face!=Supr_Interface)&&(face!=TEMP_Interface)&&(face!=Wait_Interface))
		{
			OLED_WrCmd(0xb0+i); //设置页地址（0~7）
			OLED_WrCmd(0x00); //设置显示位置—列低地址
			OLED_WrCmd(0x10); //设置显示位置—列高地址
			for(n=0; n<20; n++)	WriteDat(0x00); //写0x00到屏幕寄存器上
		}
  }
}


/*************************************************************************/
/*函数功能: 填充                                                          */
/*入口参数： 																														 */
/*						x1,y1,x2,y2 填充区域的对角坐标                              */
/*						确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 								 */
/*						dot:0,清空;1,填充	  																			 */				
/*************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}

/*************************************************************************/
/*函数功能: 画点                                                          */
/*入口参数： 															*/
/*						x：横坐标       0~127              				 */
/*						y：纵坐标	 	 		0~63                     */
/*						dot:0,清空;1,填充	  																			 */				
/*************************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

void OLED_SetPos(u8 x, u8 y)
{
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd(x&0x0f);
}


void OLED_ON(void)
{
  WriteCmd(0X8D);  //设置电荷泵
  WriteCmd(0X14);  //开启电荷泵
  WriteCmd(0XAF);  //OLED唤醒
}
 
//------让OLED休眠 -- 休眠模式下,OLED功耗不到10uA------
void OLED_OFF(void)
{
  WriteCmd(0X8D);  //设置电荷泵
  WriteCmd(0X10);  //关闭电荷泵
  WriteCmd(0XAE);  //OLED休眠
}
 /*
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N,unsigned char F16x16[adder])
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}*/

//--------------------------------------------------------------
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
  unsigned char c = 0,i = 0,j = 0;
  switch(TextSize)
  {
  case 1:
  {
    while(ch[j] != '\0')
    {
      c = ch[j] - 32;
      if(x > 126)
      {
        x = 0;
        y++;
      }
      OLED_SetPos(x,y);
      for(i=0; i<6; i++)
        WriteDat(F6x8[c][i]);
      x += 6;
      j++;
    }
  }
  break;
  case 2:
  {
    while(ch[j] != '\0')
    {
      c = ch[j] - 32;
      if(x > 120)
      {
        x = 0;
        y++;
      }
      OLED_SetPos(x,y);
      for(i=0; i<8; i++)
        WriteDat(F8X16[c*16+i]);
      OLED_SetPos(x,y+1);
      for(i=0; i<8; i++)
        WriteDat(F8X16[c*16+i+8]);
      x += 8;
      j++;
    }
  }
  break;
  }
}
 
//****************功能描述： 显示6*8或8*16的5位整数   显示的坐标（x,y），y为页范围0～7****************************
 
/*例
OLED_ShowInt(0,0,0,1);   //在(0,0)处，显示6*8的"0"
OLED_ShowInt(5,4,12345,2);//在(5,4)处，显示8*16的"12345"
*/
void OLED_ShowInt(unsigned char x, unsigned char y, int Data, unsigned char TextSize)
{
  unsigned char temp;
  OLED_SetPos(x,y);
  switch(TextSize)
  {
  case 1:
  {
    if(Data<0)
    {
      OLED_ShowChar(x,y,'-',1);
      x+=6;
      Data=-Data;
    }
    //接下来要显示正数，清空上一次显示负数的个位
    //负数比正数多一个负号，额外占了一个显示位
    OLED_ShowChar(x+30,y,' ',1);
 
    temp=Data/10000;
    OLED_ShowChar(x,y,(temp+'0'),1);
 
    Data%=10000;
    temp=Data/1000;
    OLED_ShowChar(x+6,y,(temp+'0'),1);
 
    Data%=1000;
    temp=Data/100;
    OLED_ShowChar(x+12,y,(temp+'0'),1);
 
    Data%=100;
    temp=Data/10;
    OLED_ShowChar(x+18,y,(temp+'0'),1);
 
    Data%=10;
    temp=Data;
    OLED_ShowChar(x+24,y,(temp+'0'),1);
  }
  break;
  case 2:
  {
    if(Data<0)
    {
      OLED_ShowChar(x,y,'-',2);
      x+=8;
      Data=-Data;
    }
    //接下来要显示正数，清空上一次显示负数的个位
    //负数比正数多一个负号，额外占了一个显示位
    OLED_ShowChar(x+40,y,' ',2);
 
    temp=Data/10000;
    OLED_ShowChar(x,y,(temp+'0'),2);
 
    Data%=10000;
    temp=Data/1000;
    OLED_ShowChar(x+8,y,(temp+'0'),2);
 
    Data%=1000;
    temp=Data/100;
    OLED_ShowChar(x+16,y,(temp+'0'),2);
 
    Data%=100;
    temp=Data/10;
    OLED_ShowChar(x+24,y,(temp+'0'),2);
 
    Data%=10;
    temp=Data;
    OLED_ShowChar(x+32,y,(temp+'0'),2);
  }
  break;
  }
}
 
/***************功能描述：显示6*8或8*16一个标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
/*例：  OLED_ShowChar(39,0,'A',1)*/
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize)
{
  unsigned char c=0,i=0;
  c =ch-32;
  if(x>120)
  {
    x=0;
    y++;
  }
  OLED_SetPos(x,y);
  switch(TextSize)
  {
  case 1:
  {
    for(i=0; i<6; i++)
      WriteDat(F6x8[c][i]);
    break;
  }
  case 2:
  {
    for(i=0; i<8; i++)
      WriteDat(F8X16[c*16+i]);
    OLED_SetPos(x,y+1);
    for(i=0; i<8; i++)
      WriteDat(F8X16[c*16+i+8]);
    x += 8;
    break;
  }
  }
}
 
//--------------------------------------------------------------
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
  unsigned int j=0;
  unsigned char x,y;
 
  if(y1%8==0)
    y = y1/8;
  else
    y = y1/8 + 1;
  for(y=y0; y<y1; y++)
  {
    OLED_SetPos(x0,y);
    for(x=x0; x<x1; x++)
    {
      WriteDat(BMP[j++]);
    }
  }
}










