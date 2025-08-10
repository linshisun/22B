#include "stm32f10x.h"                  // Device header
#include "stm32f10x.h"

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);//使用内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//配置时基单元
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 360 - 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器，只有高级计数器有
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//打开更新中断到NVIC的通路
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//定时器2在NVIC中的通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4, ENABLE);
}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}
