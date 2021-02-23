

//头文件
#include "stm32f10x.h"
#include "GPIOLIKE51.h"
#include "stm32f10x_gpio.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "adc.h"
#include "delay.h"
#include "OLED.h"
#include "ds18b20.h"
#include "menu.h"
#include "event_groups.h"

//GPIO初始化
void GPIO_Configuration(void);

//任务初始化
void AppTask(void* parameter);
void LedTask(void* parameter);
void AdcTask(void* parameter);
void display_logotask(void* parameter);
void MenuTask(void* parameter);
void Temp_ds18b20Task(void* parameter);

//任务句柄
TaskHandle_t AppTask_Handle;
TaskHandle_t LEDTask_Handle;
TaskHandle_t ADCTask_Handle;
TaskHandle_t DISPLAY_LOGOTask_Handle;
TaskHandle_t MENUTask_Handle;
TaskHandle_t TEMPTask_Handle;

//事件标记任务句柄
EventGroupHandle_t EventGroupHandler;	//事件标志组句柄
#define EVENTBIT_0	(1<<0)						//事件:adc采集
#define EVENTBIT_1	(1<<1)						//事件:led闪烁
#define EVENTBIT_2	(1<<2)						//事件:Null
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2)

Interface face = Wait_Interface;

static float vol;
char temperature[5]="temp:";
extern const unsigned char BMP1[];
extern const unsigned char BMP2[];

int chooselist=0;


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	GPIO_Configuration();
	RCC_Config();
	TIM2_Config(2000);
	Usart_Init();
	ADC_config();
	delay_init();
	OLED_Config();
	DS18B20_Init();
	//DS18B20_ReadRomInfo();ROM_ID= FF-FF-FF-FF-FF-FF-FF-FF

	{
		BaseType_t xReturn = pdPASS;
		xReturn = xTaskCreate((TaskFunction_t )AppTask,
                        (const char*    )"AppTask",
                        (uint16_t       )100,
                        (void*          )NULL,
                        (UBaseType_t    ) 0, 
                        (TaskHandle_t*  )&AppTask_Handle);         
		if(pdPASS == xReturn)
    vTaskStartScheduler();
  else
    return -1; 
	}
	while(1);
}

//任务创建任务
void AppTask(void* parameter)
{
		
		EventGroupHandler=xEventGroupCreate();	 //创建事件标志组
	//Led闪烁任务
		xTaskCreate((TaskFunction_t )LedTask,			
                (const char*    )"LedTask",
                (uint16_t       )200,
                (void*          )NULL,
                (UBaseType_t    ) 2, 
                (TaskHandle_t*  )&LEDTask_Handle);
	//Adc采集任务
		xTaskCreate((TaskFunction_t )AdcTask,
                (const char*    )"AdcTask",
                (uint16_t       )300,
                (void*          )NULL,
                (UBaseType_t    ) 2, 
								(TaskHandle_t*  )&ADCTask_Handle);
	//Display开机显示任务
		xTaskCreate((TaskFunction_t )display_logotask,
                (const char*    )"display_logotask",
                (uint16_t       )300,
                (void*          )NULL,
                (UBaseType_t    ) 1, 
								(TaskHandle_t*  )&DISPLAY_LOGOTask_Handle);
	//Menu菜单任务
		xTaskCreate((TaskFunction_t )MenuTask,
                (const char*    )"MenuTask",
                (uint16_t       )500,
                (void*          )NULL,
                (UBaseType_t    ) 2, 
								(TaskHandle_t*  )&MENUTask_Handle);
	//Temp_DS18B20温度采集任务
		xTaskCreate((TaskFunction_t )Temp_ds18b20Task,
                (const char*    )"Temp_ds18b20Task",
                (uint16_t       )100,
                (void*          )NULL,
                (UBaseType_t    ) 2, 
								(TaskHandle_t*  )&TEMPTask_Handle);
		vTaskDelete(AppTask_Handle);
		
}

void display_logotask(void* parameter)
{
		u8 n;
		taskENTER_CRITICAL();
		OLED_Init();
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);
		delay_xms(2000);
		OLED_Clear();
		OLED_ShowStr(20,1,"starting...",2);
		for( n = 5; n > 0; n--)
    {
        OLED_ShowInt(20, 3, n,2);     //启动倒计时
        delay_xms(800);
    }
		OLED_Clear();//清屏
		taskEXIT_CRITICAL();
		vTaskDelete(DISPLAY_LOGOTask_Handle);
		
}

void LedTask(void* parameter)
{
	//u8 i;//,j,q;
	EventBits_t EventValue;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			//等待事件组中的相应事件位
			EventValue=xEventGroupWaitBits((EventGroupHandle_t	)EventGroupHandler,		
										   (EventBits_t			)EVENTBIT_1,
										   (BaseType_t			)pdFALSE,				
										   (BaseType_t			)pdFALSE,
								       (TickType_t			)portMAX_DELAY);	
			
			
			ledctrl(OpenLED);
			vTaskDelay(500);
			ledctrl(CloseLED);
			vTaskDelay(500);
		}
		else
		{
			vTaskDelay(10);
		}
//		for(i=0;i<7;i++)
//		{
//			OLED_ShowCN(22+i*12,0,i);//测试显示中文
//		}
//		vTaskDelay(10);
		
	}
}

void AdcTask(void* parameter)
{
	static u16 temp;
	EventBits_t EventValue;
	while(1)
	{
		temp = Get_ADC_value();
    vol = (float)temp*3300/4096; 
		if(EventGroupHandler!=NULL)
		{
			//等待事件组中的相应事件位
			EventValue=xEventGroupWaitBits((EventGroupHandle_t	)EventGroupHandler,		
										   (EventBits_t			)EVENTBIT_0,
										   (BaseType_t			)pdFALSE,				
										   (BaseType_t			)pdFALSE,
								       (TickType_t			)portMAX_DELAY);	
			printf("Voltage measurement is : %f mV\r\n",vol); 
			OLED_ShowStr(20,1,"temp:",2);
			OLED_ShowInt(60,1,(int)vol,2);
		}
		else
		{
			vTaskDelay(10);
		}

	}
}

void MenuTask(void* parameter)
{
	static int key_down=0;
	while(1)
	{
		list_choose();
		LIne_Clear();
		
		if(face==Wait_Interface)
		{
			OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP2);
			if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)||(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0))
			{
				face = Menu_Interface;
				OLED_Clear();
			}
		}
		else if(face == Menu_Interface)
		{
			OLED_ShowStr(20,0,"LEDContral",1);
			OLED_ShowStr(0,1,"-----------------",1);
			OLED_ShowStr(20,2,"TEMPCheck",1);
			OLED_ShowStr(0,3,"-----------------",1);
			OLED_ShowStr(20,4,"BLACKTOLast",1);
			OLED_ShowStr(0,5,"-----------------",1);
			OLED_ShowStr(20,6,"Suprise",1);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
			{
					if(chooselist==0)
					{
						OLED_Clear();
						face = LED_Interface;
					}
					else if(chooselist == 1)
					{
						OLED_Clear();
						xEventGroupSetBits(EventGroupHandler,EVENTBIT_0);
						face = TEMP_Interface;
					}
					else if(chooselist == 2)
					{
						OLED_Clear();
						face = Wait_Interface;				
					}
					else
					{
						OLED_Clear();
						face = Supr_Interface;	
						OLED_ShowStr(20,2,"Baby Baby",2);
						OLED_ShowStr(20,4,"I Love you",2);
					}
			}
		}else if(face == LED_Interface)
		{
			if(key_down==0)
			{
			OLED_ShowStr(20,0,"LEDFlashOpen",1);
			OLED_ShowStr(0,1,"-----------------",1);
			OLED_ShowStr(20,2,"LEDFlashClose",1);
			OLED_ShowStr(0,3,"-----------------",1);
			OLED_ShowStr(20,4,"backtoMenu",1);
			}
			else if (key_down==1)
			{
				OLED_ShowStr(20,0,"LEDOpen",1);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
				{
					key_down = 0;
					face = LED_Interface;
					OLED_Clear();
				}
			}

			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
			{			
					if(chooselist==0)
					{
						key_down=1;
						xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
						OLED_Clear();
					}
					else if(chooselist == 1)
					{
						xEventGroupClearBits(EventGroupHandler,EVENTBIT_1);
						OLED_Clear();
						OLED_ShowStr(20,0,"LEDOclose",1);
						vTaskDelay(2000);
						OLED_Clear();
						face = LED_Interface;
					}
					else
					{
						OLED_Clear();
						face = Menu_Interface;
					}
			}
			
		}else if(face == TEMP_Interface)
		{
			delay_xms(100);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
			{	
				xEventGroupClearBits(EventGroupHandler,EVENTBIT_0);
				face = Menu_Interface;
				OLED_Clear();
			}
		}else if(face == Supr_Interface)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==0)
			{	
				face = Menu_Interface;
				OLED_Clear();
			}
		}
		vTaskDelay(10);
	}
}

void Temp_ds18b20Task(void* parameter)
{
	static u16 temp;
	while(1)
	{
		temp = DS18B20_GetTemperature();
		vTaskDelay(1000);
	}
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 						 	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	ledctrl(CloseLED);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}
