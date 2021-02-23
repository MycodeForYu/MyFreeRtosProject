#include "ds18b20.h"
#include "delay.h"
#include "usart.h"

u8 ROM_ID[8];
 
u8 Refresh_OLED_Data=0;

void DS18B20_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_2); 	//PA2 输出高
	
	switch(DS18B20_CheckDevice())
	{
		case 0:	printf("DS18B20_Init OK!\n");break;
		case 1:	printf("DS18B20设备响应复位信号失败！\n");break;
		case 2:	printf("DS18B20设备释放总线失败！\n");break;
	}
	
}


void DS18B20_SendRestSingle(void)
{
	/*主机发送复位信号*/
	DS18B20_OutPut_Mode();
	DS18B20_OUT=0; //拉低总线480~960 us ，对 DS18B20 芯片进行复位
	delay_us(750);
	DS18B20_OUT=1;
	delay_us(15);         //释放总线15~60 us
}


u8 DS18B20_CheckReadySingle(void)
{
	u8 cnt=0;
	/*1.检测存在脉冲*/
	DS18B20_InPut_Mode();
	while(DS18B20_IN && cnt < 240) //等待DS18B20 拉低总线 （60~240 us 响应复位信号）
	{
		delay_us(1);
		cnt++;
	}
	if(cnt>240) return 1;
	/*2.检测DS18B20是否释放总线*/	
	cnt=0;
	DS18B20_InPut_Mode();
	while((!DS18B20_IN) && cnt<240)  //判断DS18B20是否释放总线（60~240 us 响应复位信号之后会释放总线）
	{
		delay_us(1);
		cnt++;
	}
	if(cnt>240)	return 2;
	else return 0;
}


u8 DS18B20_CheckDevice(void)
{
	DS18B20_SendRestSingle();/*1.主机发送复位信号*/
	return DS18B20_CheckReadySingle();/*2.检测存在脉冲*/
}

u8 BS18B20_WriteByte(u8 cmd)
{
	u8 i=0;
	DS18B20_OutPut_Mode();
	for(i=0;i<8;i++)
	{
		DS18B20_OUT=0;
		delay_us(2);     //主机拉低总线写数据时间隙2us
		DS18B20_OUT=cmd&0x01;  //如果该写入位为1，必须在15us之内把总线拉高，为0 保持60us即可。
		delay_us(60);    //确保DS18B20已经成功读取到该位数据
		DS18B20_OUT=1;  //一位发送完成
		cmd >>=1;
		delay_us(2);     //位间隙2us
	}
	return 0;
}

u8 DS18B20_ReadByte(void)
{
	u8 i,data=0;
	for(i=0;i<8;i++)
	{
		DS18B20_OutPut_Mode();//初始化为输出模式
		DS18B20_OUT=0;  
		delay_us(2);  //主机拉低总线读数据时间隙2us		
		DS18B20_OUT=1; //释放总线，准备读取位数据
		DS18B20_InPut_Mode(); //初始化为输入模式
		delay_us(10); //等待DS18B20的数据输出
		data >>=1 ;  //高位补0，默认以0为准
		if(DS18B20_IN) data |=0x80;
		delay_us(60); //延时确保DS18B20采样周期已经过去（非常重要）
		DS18B20_OUT=1;  //释放总线准备读取下一位位数据
	}
	return data;
}

u16 DS18B20_GetTemperature(void)
{
	  u16 temp=0;
		u8 temp_H,temp_L;
		int intT,decT;
	  DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
		BS18B20_WriteByte(0xCC); //跳过ROM序列检测
		BS18B20_WriteByte(0x44); //启动一次温度转换
		
		//等待温度转换完成
		while(DS18B20_ReadByte()!=0xFF){}
		
		DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
		BS18B20_WriteByte(0xCC); //跳过ROM序列检测
		BS18B20_WriteByte(0xBE); //读取温度
		
		temp_L=DS18B20_ReadByte(); //读取的温度低位数据
		temp_H=DS18B20_ReadByte(); //读取的温度高位数据
		temp=temp_L|(temp_H<<8);   //合成温度
		intT = temp>>4 ;          /*合成实际温度整数部分****精度相对上面的更高*/ 
		decT = temp&0xF ;         /*合成实际温度小数部分*/
		printf("温度:%d.%d ℃ \n",intT,decT);
		return temp;
}


u8 DS18B20_ReadRomInfo(void)
{
	u8 i=0;
	BS18B20_WriteByte(0x33);  /*4.启动读取ROM*/
	for(i=0;i<8;i++)
	{
		ROM_ID[i]=DS18B20_ReadByte();
	}
	printf("ROM_ID= ");
	for(i=0;i<8;i++)
	{
		printf("%X",ROM_ID[i]);
		if(i==7) printf("\n");
		else printf("-");
	}
	return 0;
}


u8 DS18B20_MatchROM(void)
{
	u8 i=0;
	BS18B20_WriteByte(0x55);  /*4.匹配64位 ROM 信息*/
	for(i=0;i<8;i++)
	{
		BS18B20_WriteByte(ROM_ID[i]);
	}
	return 0;
}








