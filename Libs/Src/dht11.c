/*
 * dht11.c
 *
 *  Created on: 21 giu 2020
 *      Author: Starsky
 */

#include "dht11.h"
#include "functions.h"

uint8_t check_payload(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t sum)
{
	uint8_t buffer = b1 + b2 + b3 + b4;							//la checksum è il byte meno significativo della somma degli altri 4
	if (sum == buffer) return 1;
	else return 0;
}

uint8_t handshake_dht11()
{
	GPIOE->MODER |= GPIO_MODER_MODER3_0;                 //canale 3 per il sensore su output digitale
	GPIOE->ODR &= ~(GPIO_ODR_3);		//linee giù inizio handshake
	tim_delay(1296000, tim);                                    //faccio passare 18 millisec
	GPIOE->ODR |= GPIO_ODR_3;			//linee su in attesa di risposta
	tim_delay(2880, tim);                               // aspetto 40 microsec per la risposta
	GPIOE->MODER &= ~(GPIO_MODER_MODER3_0); //moder della linea in modalità input digitale
	//attesa risposta
	TIM2->ARR = 7200;
	TIM2->CR1 |= TIM_CR1_CEN;
	while ((GPIOE->IDR & GPIO_IDR_3) == GPIO_IDR_3)
	{
		if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
		{
			TIM2->SR &= ~TIM_SR_UIF;
			TIM2->CR1 &= ~TIM_CR1_CEN;
			return -1;                              //timeout
		}
	}
	TIM2->CNT = 0;
	while ((GPIOE->IDR & GPIO_IDR_3) == 0)
	{
		if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
		{
			TIM2->SR &= ~TIM_SR_UIF;
			TIM2->CR1 &= ~TIM_CR1_CEN;
			return -1;                              //timeout
		}
	}
	TIM2->CNT = 0;
	while ((GPIOE->IDR & GPIO_IDR_3) == GPIO_IDR_3)
	{
		if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
		{
			TIM2->SR &= ~TIM_SR_UIF;
			TIM2->CR1 &= ~TIM_CR1_CEN;
			return -1;                              //timeout
		}
	}
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->CNT = 0;

	return 0;
}

uint8_t read_data_dht11()
{
	uint8_t data = 0, count;
	for (count = 0; count < 8; count++)						//eseguo la lettura del bit 8 volte (byte)
	{
		TIM2->ARR = 7200;
		TIM2->CR1 |= TIM_CR1_CEN;
		while ((GPIOE->IDR & GPIO_IDR_3) == 0)		//aspetto che la linea si alzi
		{
			if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
			{
				TIM2->SR &= ~TIM_SR_UIF;
				TIM2->CR1 &= ~TIM_CR1_CEN;
				return -1;
			}
		}
		TIM2->CNT = 0;
		while ((GPIOE->IDR & GPIO_IDR_3) == GPIO_IDR_3)		//aspetto che la linea si abbassi
		{
			if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF)
			{
				TIM2->SR &= ~TIM_SR_UIF;
				TIM2->CR1 &= ~TIM_CR1_CEN;
				return -1;
			}
		}
		if (TIM2->CNT > 2880)
		{
			data |= (1 << (7 - count));					//in posizione 7-count ci scrivo 0	(big endian)
		}
		TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0;
	}
	return data;
}
