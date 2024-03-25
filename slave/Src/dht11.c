/*
 * dht11.c
 *
 *  Created on: Nov 4, 2023
 *      Author: andre
 */


#include "dht11.h"

#include "main.h"
#include "gpio.h"





int dht11_read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t* humidity, uint8_t* temperature)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t bits[5] = {0};


	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	HAL_Delay(100);

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, RESET);
	HAL_Delay(18);

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	delay_us(40);

	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
	{
		return DHT11_ERROR;
	}
	delay_us(80);
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
	{
		return DHT11_ERROR;
	}
	delay_us(80);

	uint16_t timeout_counter = 0;

	for (int j = 0; j < 5; j++)
	{
		uint8_t result = 0;
		for (int i = 0; i < 8; i++)
		{
			while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
			{
				timeout_counter++;
				if (timeout_counter > DHT11_TIMEOUT)
					return DHT11_ERROR;
			}
			delay_us(30);
			if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
			{
				result |= (1 << (7 - i));
			}
			timeout_counter = DHT11_ERROR;
			while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
			{
				timeout_counter++;
				if (timeout_counter > DHT11_TIMEOUT)
					return DHT11_ERROR;
			}
		}
		bits[j] = result;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_Delay(100);
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4])
	{
		*temperature = bits[2];
		*humidity = bits[0];

		return DHT11_SUCCESS;
	}
	return DHT11_ERROR;
}
