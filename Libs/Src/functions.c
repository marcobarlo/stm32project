/*
 * functions.c
 *
 *  Created on: 20 giu 2020
 *      Author: Andrea Marchetta
 */
#include "functions.h"

void tim_delay(int ticks, TIM_TypeDef* tim)
{
	tim->ARR = ticks;
	tim->CR1 |= TIM_CR1_CEN;
	while ((tim->SR & TIM_SR_UIF) != TIM_SR_UIF);
	tim->SR &= ~TIM_SR_UIF;
	tim->CR1 &= ~TIM_CR1_CEN;
}


void timer2Startup()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 1296000;				//18 millisec iniziali per hanshake
	TIM2->CR1 |= TIM_CR1_OPM;			//non deve essere un timer periodico => one pulse mode
}

