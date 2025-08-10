#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Serial.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将引脚初始化为推挽输出	
	
	PWM_Init();													//初始化直流电机的底层PWM
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int8_t left,int8_t right)
{
	if(left < 0)
	{
		//左轮
		GPIO_SetBits(GPIOA,GPIO_Pin_10);	//PA10置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);	//PA11置低电平，设置方向为反转
		PWM_SetCompare2(-left);			//PWM设置为速度值
	}
	if(left >= 0)
	{
		//左轮
		GPIO_SetBits(GPIOA,GPIO_Pin_11);	//PA11置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);	//PA10置低电平，设置方向为正转
		PWM_SetCompare2(left);				//PWM设置为速度值
	}
	if(right <0)
	{
		//右轮
		GPIO_SetBits(GPIOA, GPIO_Pin_5);	//PA4置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA5置低电平，设置方向为反转
		PWM_SetCompare1(-right);			//PWM设置为速度值
	}		
	if(right >= 0)
	{
		//右轮
		GPIO_SetBits(GPIOA, GPIO_Pin_4);	//PA5置高电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);	//PA4置低电平，设置方向为正转
		PWM_SetCompare1(right);				//PWM设置为速度值
	}
}

void Motor_Stop(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_10);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}
