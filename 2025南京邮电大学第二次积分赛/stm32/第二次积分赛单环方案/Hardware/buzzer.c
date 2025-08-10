#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Buzzer_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB12初始化为推挽输出
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void beep(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay_s(1);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}
