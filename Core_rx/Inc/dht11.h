/*
 * dht11.h
 *
 *  Created on: 21 giu 2020
 *      Author: Starsky
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "functions.h"
#include "stm32f3xx.h"

#define tim TIM2			//uso il timer 2 per i delay

uint8_t check_payload(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t sum);

uint8_t read_data_dht11();

uint8_t handshake_dht11();

#endif /* INC_DHT11_H_ */
