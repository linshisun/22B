#include "stm32f10x.h"                  // Device header
#include <math.h>
#include "pid.h"

//void vel_filter(PID_t *f,float unfilt)
//{
//	float last_filt;
//	last_filt =f->Actual;
//	f->Actual = 0.3f * unfilt+0.7f * last_filt;
//}

int filter(int unfilt)
{
	static int last_filt;
	int filted;
	filted = 0.3f * unfilt+0.7f * last_filt;
	last_filt = filted;
	return filted;
}
