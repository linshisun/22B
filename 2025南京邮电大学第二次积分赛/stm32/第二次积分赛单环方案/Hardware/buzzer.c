#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Buzzer_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOB��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PB12��ʼ��Ϊ�������
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void beep(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay_s(1);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}
