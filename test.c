#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_tim.h"
#include "misc.h"

int state_tim2 = 0;
int state_tim3 = 0;

const GREEN_LED = 0x80;
const BLUE_LED = 0x40;

void led_init(void);
void timer_init(void);
void irq_init(void);

void TIM2_IRQHandler() {

	if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	 {

	 	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	 	if (state_tim2)
	 	{
	 		GPIOB->ODR |= GREEN_LED;
	 		//GPIO_Write(GPIOB,|GREEN_LED);
	 		state_tim2 = 0;
	 	}
	 	else {
	 		GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);
	 		//GPIO_Write(GPIOB,|0);
	 		state_tim2 = 1;
	 	}

	 }

}

void TIM3_IRQHandler() {

	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	 {

	 	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	 	if (state_tim3)
	 	{
	 		GPIOB->ODR |= BLUE_LED;
	 		//GPIO_Write(GPIOB,|BLUE_LED);
	 		state_tim3 = 0;
	 	}
	 	else {
	 		GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
	 		//GPIO_Write(GPIOB,|0);
	 		state_tim3 = 1;
	 	}

	 }

}

int main(void)
{

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	led_init();
	timer_init();
	irq_init();

	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);

	do {
	} while(1);
}

void led_init(void) {

	GPIO_InitTypeDef led;

	led.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
	led.GPIO_Mode  = GPIO_Mode_OUT;
	led.GPIO_OType = GPIO_OType_PP;
	led.GPIO_Speed = GPIO_Speed_400KHz;
	led.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB,&led);

}

void timer_init(void) {

	TIM_TimeBaseInitTypeDef tim2, tim3;

	tim2.TIM_Prescaler = 4000-1;
	tim2.TIM_CounterMode = TIM_CounterMode_Up;
	tim2.TIM_Period = 500-1;
	tim2.TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM2,&tim2);
	
	tim3.TIM_Prescaler = 4000-1;
	tim3.TIM_CounterMode = TIM_CounterMode_Up;
	tim3.TIM_Period = 250-1;
	tim3.TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM3,&tim3);

}

void irq_init(void) {

	NVIC_InitTypeDef nvic_tim2, nvic_tim3;

	nvic_tim2.NVIC_IRQChannel = TIM2_IRQn;
	nvic_tim2.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_tim2.NVIC_IRQChannelSubPriority = 0;
	nvic_tim2.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_tim2);

	nvic_tim3.NVIC_IRQChannel = TIM3_IRQn;
	nvic_tim3.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_tim3.NVIC_IRQChannelSubPriority = 1;
	nvic_tim3.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_tim3);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}