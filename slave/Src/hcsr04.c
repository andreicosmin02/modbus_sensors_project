#include "hcsr04.h"
#include "main.h"
#include "timer.h"

uint16_t hcsr04_distance(GPIO_TypeDef* GPIOx, uint16_t PIN_Trig, uint16_t PIN_Echo)
{
    HAL_GPIO_WritePin(GPIOx, PIN_Trig, RESET);
    delay_us(4);

    HAL_GPIO_WritePin(GPIOx, PIN_Trig, SET);
    delay_us(15);
    HAL_GPIO_WritePin(GPIOx, PIN_Trig, RESET);

    while (HAL_GPIO_ReadPin(GPIOx, PIN_Echo) == GPIO_PIN_RESET);

    uint16_t wait = get_counter();
    while (HAL_GPIO_ReadPin(GPIOx, PIN_Echo) == GPIO_PIN_SET);
    uint16_t duration = get_counter() - wait;

    uint16_t distance = duration * (0.034 / 2);
    return distance;
}
