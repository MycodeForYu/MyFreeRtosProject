#include "ds18b20.h"
#include "delay.h"
#include "usart.h"

u8 ROM_ID[8];
 
u8 Refresh_OLED_Data=0;

void DS18B20_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_2); 	//PA2 �����
	
	switch(DS18B20_CheckDevice())
	{
		case 0:	printf("DS18B20_Init OK!\n");break;
		case 1:	printf("DS18B20�豸��Ӧ��λ�ź�ʧ�ܣ�\n");break;
		case 2:	printf("DS18B20�豸�ͷ�����ʧ�ܣ�\n");break;
	}
	
}


void DS18B20_SendRestSingle(void)
{
	/*�������͸�λ�ź�*/
	DS18B20_OutPut_Mode();
	DS18B20_OUT=0; //��������480~960 us ���� DS18B20 оƬ���и�λ
	delay_us(750);
	DS18B20_OUT=1;
	delay_us(15);         //�ͷ�����15~60 us
}


u8 DS18B20_CheckReadySingle(void)
{
	u8 cnt=0;
	/*1.����������*/
	DS18B20_InPut_Mode();
	while(DS18B20_IN && cnt < 240) //�ȴ�DS18B20 �������� ��60~240 us ��Ӧ��λ�źţ�
	{
		delay_us(1);
		cnt++;
	}
	if(cnt>240) return 1;
	/*2.���DS18B20�Ƿ��ͷ�����*/	
	cnt=0;
	DS18B20_InPut_Mode();
	while((!DS18B20_IN) && cnt<240)  //�ж�DS18B20�Ƿ��ͷ����ߣ�60~240 us ��Ӧ��λ�ź�֮����ͷ����ߣ�
	{
		delay_us(1);
		cnt++;
	}
	if(cnt>240)	return 2;
	else return 0;
}


u8 DS18B20_CheckDevice(void)
{
	DS18B20_SendRestSingle();/*1.�������͸�λ�ź�*/
	return DS18B20_CheckReadySingle();/*2.����������*/
}

u8 BS18B20_WriteByte(u8 cmd)
{
	u8 i=0;
	DS18B20_OutPut_Mode();
	for(i=0;i<8;i++)
	{
		DS18B20_OUT=0;
		delay_us(2);     //������������д����ʱ��϶2us
		DS18B20_OUT=cmd&0x01;  //�����д��λΪ1��������15us֮�ڰ��������ߣ�Ϊ0 ����60us���ɡ�
		delay_us(60);    //ȷ��DS18B20�Ѿ��ɹ���ȡ����λ����
		DS18B20_OUT=1;  //һλ�������
		cmd >>=1;
		delay_us(2);     //λ��϶2us
	}
	return 0;
}

u8 DS18B20_ReadByte(void)
{
	u8 i,data=0;
	for(i=0;i<8;i++)
	{
		DS18B20_OutPut_Mode();//��ʼ��Ϊ���ģʽ
		DS18B20_OUT=0;  
		delay_us(2);  //�����������߶�����ʱ��϶2us		
		DS18B20_OUT=1; //�ͷ����ߣ�׼����ȡλ����
		DS18B20_InPut_Mode(); //��ʼ��Ϊ����ģʽ
		delay_us(10); //�ȴ�DS18B20���������
		data >>=1 ;  //��λ��0��Ĭ����0Ϊ׼
		if(DS18B20_IN) data |=0x80;
		delay_us(60); //��ʱȷ��DS18B20���������Ѿ���ȥ���ǳ���Ҫ��
		DS18B20_OUT=1;  //�ͷ�����׼����ȡ��һλλ����
	}
	return data;
}

u16 DS18B20_GetTemperature(void)
{
	  u16 temp=0;
		u8 temp_H,temp_L;
		int intT,decT;
	  DS18B20_CheckDevice();   //���͸�λ���塢����������
		BS18B20_WriteByte(0xCC); //����ROM���м��
		BS18B20_WriteByte(0x44); //����һ���¶�ת��
		
		//�ȴ��¶�ת�����
		while(DS18B20_ReadByte()!=0xFF){}
		
		DS18B20_CheckDevice();   //���͸�λ���塢����������
		BS18B20_WriteByte(0xCC); //����ROM���м��
		BS18B20_WriteByte(0xBE); //��ȡ�¶�
		
		temp_L=DS18B20_ReadByte(); //��ȡ���¶ȵ�λ����
		temp_H=DS18B20_ReadByte(); //��ȡ���¶ȸ�λ����
		temp=temp_L|(temp_H<<8);   //�ϳ��¶�
		intT = temp>>4 ;          /*�ϳ�ʵ���¶���������****�����������ĸ���*/ 
		decT = temp&0xF ;         /*�ϳ�ʵ���¶�С������*/
		printf("�¶�:%d.%d �� \n",intT,decT);
		return temp;
}


u8 DS18B20_ReadRomInfo(void)
{
	u8 i=0;
	BS18B20_WriteByte(0x33);  /*4.������ȡROM*/
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
	BS18B20_WriteByte(0x55);  /*4.ƥ��64λ ROM ��Ϣ*/
	for(i=0;i<8;i++)
	{
		BS18B20_WriteByte(ROM_ID[i]);
	}
	return 0;
}








