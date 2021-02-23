#include "OLED.h"
#include "myiic.h"
#include "codetab.h"
#include "menu.h"

#include "stm32f10x.h"

#define OLED_WrCmd WriteCmd   //��д�����������
#define OLED_WrData WriteDat  //��д���ݺ���������

extern int chooselist;
extern Interface face;

//��Ÿ�ʽ����.
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
 	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
 	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
	OLED_Clear();//OLED����
}


/*************************************************************************/
/*��������: �����Դ浽OLED                                                 */
/*************************************************************************/
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd(0xb0+i);   //����ҳ��ַ��0~7��
		WriteCmd(0x00);      //������ʾλ�á��е͵�ַ
		WriteCmd(0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)WriteDat(OLED_GRAM[n][i]); 
	}   
}

void OLED_Clear(void)
{
  unsigned char i,n;
  for(i=0; i<8; i++)
  {
    OLED_WrCmd(0xb0+i); //����ҳ��ַ��0~7��
    OLED_WrCmd(0x00); //������ʾλ�á��е͵�ַ
    OLED_WrCmd(0x10); //������ʾλ�á��иߵ�ַ
    for(n=0; n<128; n++)	WriteDat(0x00); //д0x00����Ļ�Ĵ�����
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
			OLED_WrCmd(0xb0+i); //����ҳ��ַ��0~7��
			OLED_WrCmd(0x00); //������ʾλ�á��е͵�ַ
			OLED_WrCmd(0x10); //������ʾλ�á��иߵ�ַ
			for(n=0; n<20; n++)	WriteDat(0x00); //д0x00����Ļ�Ĵ�����
		}
  }
}


/*************************************************************************/
/*��������: ���                                                          */
/*��ڲ����� 																														 */
/*						x1,y1,x2,y2 �������ĶԽ�����                              */
/*						ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 								 */
/*						dot:0,���;1,���	  																			 */				
/*************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}

/*************************************************************************/
/*��������: ����                                                          */
/*��ڲ����� 															*/
/*						x��������       0~127              				 */
/*						y��������	 	 		0~63                     */
/*						dot:0,���;1,���	  																			 */				
/*************************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
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
  WriteCmd(0X8D);  //���õ�ɱ�
  WriteCmd(0X14);  //������ɱ�
  WriteCmd(0XAF);  //OLED����
}
 
//------��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA------
void OLED_OFF(void)
{
  WriteCmd(0X8D);  //���õ�ɱ�
  WriteCmd(0X10);  //�رյ�ɱ�
  WriteCmd(0XAE);  //OLED����
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
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
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
 
//****************���������� ��ʾ6*8��8*16��5λ����   ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************
 
/*��
OLED_ShowInt(0,0,0,1);   //��(0,0)������ʾ6*8��"0"
OLED_ShowInt(5,4,12345,2);//��(5,4)������ʾ8*16��"12345"
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
    //������Ҫ��ʾ�����������һ����ʾ�����ĸ�λ
    //������������һ�����ţ�����ռ��һ����ʾλ
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
    //������Ҫ��ʾ�����������һ����ʾ�����ĸ�λ
    //������������һ�����ţ�����ռ��һ����ʾλ
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
 
/***************������������ʾ6*8��8*16һ����׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
/*����  OLED_ShowChar(39,0,'A',1)*/
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
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
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










