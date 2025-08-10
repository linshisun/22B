#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Servo.h"
#include "buzzer.h"

uint8_t mode,flag;//flag防止和之前模式指令冲突

void Point_Decision(int p,int angle_d)
{
	switch(p)
	{
		case 0://未启动
		{
			mode = 0;//停止
			break;
		}
		case 1://任务1，检测到巡线停车点
		{
			if(!flag)
			{mode = 0;}//停止
			else
			{mode = 2;}//倒车转弯入库
			break;
		}
		case 2://任务1，检测到小车与库线平行且位于库口
		{
			if(flag)
			{mode = 3;}//入库直行
			else
			{
				mode = 4;//出库转弯
			}
			break;
		}
		case 3://任务1，检测到小车已入库
		{
			if(flag)
			{mode = 0;}//停止
			else
			{mode = 1;}//出库
			break;
		}
		case 4://任务2转弯1
		{
			if(!flag)
			{mode = 0;}//停止
			else
			{mode = 5;}//倒车转弯入库1
			break;
		}
		case 5:
		{
			mode = 3;
			break;
		}
		case 6://任务2，向后转弯入库
		{
			mode = 6;
			break;
		}
		case 7:
		{
			if(flag)
			{mode = 0;}
			else
			{mode = 7;}
			break;
		}
		case 8://第一段巡线
		{
			mode = 1;
			break;
		}
		case 9://第二段巡线
		{
			mode = 8;
			break;
		}
	}
}

void mode_choice(int p,int angle_d,uint8_t *s)
{
	Point_Decision(p,angle_d);
	switch(mode)
	{
		case 0://停车
		{
			Motor_SetSpeed(0,0);
			TIM_SetCompare1(TIM4, 1500);
			if(p == 1 || p == 4)//蜂鸣
			{
				Delay_s(1);
				beep();
				flag = 1;
			}
			if(p == 3 || p == 7)
			{
				Delay_s(1);
				beep();
				Delay_s(3);
				flag = 0;
			}
			break;
		}
		case 1://巡线
		{
			if(p == 3)
			{
				Servo_SetAngle(90);
				Motor_SetSpeed(20,20);
			}
			else
			{
				Servo_SetAngle(90 + angle_d);
				Motor_SetSpeed(15,15);
			}
			break;
		}
		case 2://任务1转弯倒车
		{
			Servo_SetAngle(180);
			Motor_SetSpeed(-24,-16);
			break;
		}
		case 3://任务1直行入库
		{
			Servo_SetAngle(90);
			Motor_SetSpeed(-20,-20);
			break;
		}
		case 4://任务1转弯出库
		{
			Servo_SetAngle(180);
			Motor_SetSpeed(24,16);
			break;
		}
		case 5:
		{
			Servo_SetAngle(160);
			Motor_SetSpeed(-24,-16);
			break;
		}
		case 6:
		{
			Servo_SetAngle(30);
			Motor_SetSpeed(-17,-23);
			break;
		}
		case 7:
		{
			Servo_SetAngle(22);
			Motor_SetSpeed(17,23);
			Delay_ms(1500);
			Servo_SetAngle(158);
			Motor_SetSpeed(23,17);
			Delay_ms(2500);
			Motor_SetSpeed(0,0);
			TIM_SetCompare1(TIM4, 1500);
			Delay_s(1);
			beep();
			Delay_s(1);
			*s = 1;
			break;
		}
		case 8:
		{
			Servo_SetAngle(90 + angle_d);
			Motor_SetSpeed(20,20);
			break;
		}
	}
}
