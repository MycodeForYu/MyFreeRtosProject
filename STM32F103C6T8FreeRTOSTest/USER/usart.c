#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif


void Usart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef   USART_ClockInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);
    /* PA9 USART1_Tx */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    /* PA10 USART1_Rx  */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;      
 
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;      
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; 
    
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

}




