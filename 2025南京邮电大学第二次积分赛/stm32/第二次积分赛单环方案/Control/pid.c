#include "stm32f10x.h"                  // Device header
#include <math.h>
#include "Motor.h"
#include "Servo.h"
#include "pid.h"

void PIDcontrol(PID_t *p)//速度环，增量pid
{
	p->bias1 = p->bias0;
	p->bias0 = p->Target - p->Actual;
	
	if (p->position_Ki != 0)
	{
		p->biasInt += p->bias0;
	}
	else
	{
		p->biasInt = 0;
	}
	
	p->Inner_Out = p->position_Kp * p->bias0
		   + p->position_Ki * p->biasInt
		   + p->position_Kd * (p->bias0 - p->bias1);
	
	if(p->Inner_Out >= p->outMax)
	{p->Inner_Out = p->outMax;}
	if(p->Inner_Out <= p->outMin)
	{p->Inner_Out = p->outMin;}
	
}
