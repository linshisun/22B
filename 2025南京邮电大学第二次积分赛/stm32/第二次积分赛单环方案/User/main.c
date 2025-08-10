#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "Motor.h"
#include "Serial.h"
#include "Key.h"
#include "Servo.h"
#include "mode_choose.h"
#include "buzzer.h"

int Angle_Diff,point;
uint8_t KeyNum,count,stop = 1;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Serial2_Init();
	Key_Init();
	Servo_Init();
	Motor_Init();//定时器中断在其中
	Buzzer_Init();
	
	TIM_SetCompare1(TIM4, 1500);
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum!=0)//按键启动
		{
			stop = 0;
			Servo_SetAngle(45);
			Motor_SetSpeed(17,23);
			Delay_ms(600);
			Servo_SetAngle(135);
			Motor_SetSpeed(23,17);
			Delay_ms(600);
			point = 8;
		}
		
		
		if (Serial2_RxFlag == 1)		//如果接收到数据包
		{
			sscanf(Serial2_RxPacket,"D%d",&Angle_Diff);
			sscanf(Serial2_RxPacket,"t%d",&point);
			
			Serial2_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
		if(!stop)
		{mode_choice(point,Angle_Diff,&stop);}
	}
}

void TIM2_IRQHandler(void)//10kHz
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		count ++;
		
		if(count >=200)
		{
			count = 0;
			Key_Tick();
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
