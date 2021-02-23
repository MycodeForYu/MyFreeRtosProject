#include "timer.h"
#include "oled.h"

#include "stm32f10x_tim.h"
#include "stm32f10x_flash.h"

extern CtrlLed ctrl;

volatile u16 LEDPWMMax = 300;
volatile u16 LEDPWMSetOPEN = 0;
volatile u16 LEDPWMSetCLOSE = 0;
volatile u32 softtime = 0;

void RCC_Config(void)
{
	RCC_DeInit();			//��ʼ��Ϊȱʡֵ
	RCC_HSEConfig(RCC_HSE_ON);	//ʹ���ⲿ�ĸ���ʱ�� 
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);	//�ȴ��ⲿ����ʱ��ʹ�ܾ���
	RCC_HCLKConfig(RCC_SYSCLK_Div1);		
	RCC_PCLK2Config(RCC_HCLK_Div1);		
	RCC_PCLK1Config(RCC_HCLK_Div1);		
   RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);	//Select HSE as system clock
	while(RCC_GetSYSCLKSource()!=0x04);		//Wait till HSE is used as system clock source

}


void TIM2_Config(u16 time)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_InitStruct.TIM_Period =time;
	TIM_InitStruct.TIM_Prescaler = 7;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
		TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
	
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		softtime++;
	}
	if(softtime==5)
	{
//		LIne_Clear();
//		softtime=0;
	}
}


//�����Ƴ�ʼ������
volatile u16 Iset = 10;
volatile u16 i = 0;
volatile u16 j = 0;
volatile u16 turn_on = 0;


//LED�ƿ��ƺ���
void ledctrl(CtrlLed ctrl)
{
	if(ctrl == OpenLED)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
}



//�����ƺ���
void led_Fresh(void)
{
	if(turn_on == 0)
	{
		if(LEDPWMSetOPEN	<	Iset)
		{
			++i;
			++LEDPWMSetOPEN;
			if(LEDPWMSetOPEN < Iset)
			{
				ledctrl(OpenLED);
				return ;
			}
			LEDPWMSetCLOSE = LEDPWMMax - Iset;
		}
		if(j < LEDPWMSetCLOSE)
			{
				++j;
				ledctrl(CloseLED);
				return ;
			}
			if((Iset<LEDPWMMax)&&(i == Iset)&&(j == LEDPWMSetCLOSE))
			{
				i = 0;j =0;
				LEDPWMSetOPEN = 0;
				LEDPWMSetCLOSE =0;
				Iset +=5;
			}
			else if((Iset>=LEDPWMMax)&&(i == Iset)&&(j == LEDPWMSetCLOSE))
			{
				turn_on = 1;
				i = 0;j = 0;
				LEDPWMSetOPEN = 0;
				LEDPWMSetCLOSE =0;
				ledctrl(OpenLED);
				Iset =10;
			}else
			{
				return;
			}
	}
	else
	{
		if(LEDPWMSetCLOSE	<	Iset)
		{
			++i;
			++LEDPWMSetCLOSE;
			if(LEDPWMSetCLOSE < Iset)
			{
				ledctrl(CloseLED);
				return ;
			}
			LEDPWMSetOPEN = LEDPWMMax - Iset;
		}
		if(j < LEDPWMSetOPEN)
			{
				++j;
				ledctrl(OpenLED);
				return ;
			}
			if((Iset<LEDPWMMax)&&(i == Iset)&&(j == LEDPWMSetOPEN))
			{
				i = 0;j =0;
				LEDPWMSetOPEN = 0;
				LEDPWMSetCLOSE =0;
				Iset +=5;
			}else if((Iset>=LEDPWMMax)&&(i == Iset)&&(j == LEDPWMSetOPEN))
			{
				Iset = 10;
				turn_on = 0;
				i = 0;j = 0;
				LEDPWMSetOPEN = 0;
				LEDPWMSetCLOSE =0;
				ledctrl(CloseLED);
				return;
			}else
			{
				return;
			}
	}
	
}




