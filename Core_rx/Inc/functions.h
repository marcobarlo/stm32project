/*
 * functions.h
 *
 *  Created on: 20 giu 2020
 *      Author: Andrea Marchetta
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "stm32f3xx.h"

#define NACK 0

void tim_delay(int ticks, TIM_TypeDef* tim);

void timer2Startup();

#endif /* INC_FUNCTIONS_H_ */
