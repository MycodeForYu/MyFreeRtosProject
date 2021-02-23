#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "sys.h"

//IO方向设置
#define DS18B20_InPut_Mode()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=8<<8;}    //PA2输入模
#define DS18B20_OutPut_Mode() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=3<<8;}     //PA2输出模

//IO操作                          
#define DS18B20_OUT PAout(2) //数据端口PA2
#define DS18B20_IN  PAin(2)  //数据端口PA2


extern void DS18B20_Init(void);
extern u8 CheckReadysingle(void);//检测DS18B20是否准备成功
extern void DS18B20_SendRestSingle(void);//向DS18B20发送一个复位信号
extern u8 DS18B20_CheckDevice(void);
extern u8 BS18B20_WriteByte(u8 cmd);
extern u8 DS18B20_ReadByte(void);
extern u8 DS18B20_ReadRomInfo(void);
extern u8 DS18B20_MatchROM(void);
extern u16 DS18B20_GetTemperature(void);









#endif 




