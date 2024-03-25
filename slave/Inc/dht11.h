/*
 * dht11.h
 *
 *  Created on: Mar 16, 2024
 *      Author: andre
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "main.h"
#include "timer.h"

#define SET (GPIO_PinState)1
#define RESET (GPIO_PinState)0
#define DHT11_ERROR (-1)
#define DHT11_SUCCESS 1
#define DHT11_TIMEOUT 200

int dht11_read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t *humidity, uint8_t *temperature);


#endif /* INC_DHT11_H_ */
