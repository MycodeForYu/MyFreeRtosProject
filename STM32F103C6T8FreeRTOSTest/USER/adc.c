#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

void ADC_config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;   
    ADC_InitTypeDef ADC_InitStructure;     
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);              
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);     
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);           
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                    
    GPIO_Init(GPIOA,&GPIO_InitStructure);                    
    
    ADC_DeInit(ADC1);                         
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                      
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;               
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;           
    ADC_InitStructure.ADC_NbrOfChannel = 1;                          
    ADC_Init(ADC1,&ADC_InitStructure);             
    
    ADC_Cmd(ADC1,ENABLE);                      
    
    ADC_ResetCalibration(ADC1);             
    while(ADC_GetResetCalibrationStatus(ADC1));  
    ADC_StartCalibration(ADC1);                 
    while(ADC_GetCalibrationStatus(ADC1));   
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);  
}


u16 Get_ADC_value(void)
{
		ADC_RegularChannelConfig(ADC1,0,1,ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);        
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1); 
}

