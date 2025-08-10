#ifndef __PID_H
#define __PID_H

typedef struct {
	float Target;
	float Actual;
	float Inner_Out;
	
	float position_Kp;
	float position_Ki;
	float position_Kd;
	
	float bias0;
	float bias1;
	float biasInt;
	
	float outMax;
	float outMin;
} PID_t;

void PIDcontrol(PID_t *p);

#endif
