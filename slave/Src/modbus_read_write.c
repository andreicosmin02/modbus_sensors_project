#include "modbus_read_write.h"
#include "gpio.h"
#include "dht11.h"
#include "hcsr04.h"

uint8_t ReadStareLed1(void)
{
    return HAL_GPIO_ReadPin(GPIOA, Led1_Pin) == GPIO_PIN_SET ? 1 : 0;
}

uint8_t ReadStareLed2(void)
{
    return HAL_GPIO_ReadPin(GPIOA, Led2_Pin) == GPIO_PIN_SET ? 1 : 0;
}

uint8_t ReadStareLed3(void)
{
    return HAL_GPIO_ReadPin(GPIOA, Led3_Pin) == GPIO_PIN_SET ? 1 : 0;
}

uint8_t ReadStareLed4(void)
{
    return HAL_GPIO_ReadPin(GPIOA, Led4_Pin) == GPIO_PIN_SET ? 1 : 0;
}

uint8_t ReadStareLed5(void)
{
    return HAL_GPIO_ReadPin(GPIOA, Led5_Pin) == GPIO_PIN_SET ? 1 : 0;
}

uint16_t ReadDHT11(void)
{
    uint8_t humidity;
    uint8_t temperature;

    if (dht11_read(GPIOB, GPIO_PIN_0, &humidity, &temperature) != DHT11_ERROR)
    {
    	uint16_t response = (humidity << BYTE_SIZE) | temperature;
    	return response;
    }
    return READ_HOLDING_REGS_ERROR;

}

uint16_t ReadHCSR04(void)
{
    uint16_t distance = hcsr04_distance(HCSR04_Trig_GPIO_Port, HCSR04_Trig_Pin, HCSR04_Echo_Pin);
    return distance;
}

void WriteStareLed1(uint16_t stare)
{
    if (stare == 1)
    {
        HAL_GPIO_WritePin(GPIOA, Led1_Pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(GPIOA, Led1_Pin, GPIO_PIN_RESET);
    }
}

void WriteStareLed2(uint16_t stare)
{
    if (stare == 1)
    {
        HAL_GPIO_WritePin(GPIOA, Led2_Pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(GPIOA, Led2_Pin, GPIO_PIN_RESET);
    }
}

void WriteStareLed3(uint16_t stare)
{
    if (stare == 1)
    {
        HAL_GPIO_WritePin(GPIOA, Led3_Pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(GPIOA, Led3_Pin, GPIO_PIN_RESET);
    }
}

void WriteStareLed4(uint16_t stare)
{
    if (stare == 1)
    {
        HAL_GPIO_WritePin(GPIOA, Led4_Pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(GPIOA, Led4_Pin, GPIO_PIN_RESET);
    }
}

void WriteStareLed5(uint16_t stare)
{
    if (stare == 1)
    {
        HAL_GPIO_WritePin(GPIOA, Led5_Pin, GPIO_PIN_SET);
    } else
    {
        HAL_GPIO_WritePin(GPIOA, Led5_Pin, GPIO_PIN_RESET);
    }
}
