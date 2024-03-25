#include "timer.h"

void timer_init()
{
    MX_TIM1_Init();
    HAL_TIM_Base_Start(&htim1);
}

void delay_us (uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}

uint16_t get_counter(void)
{
    uint16_t counter_value = __HAL_TIM_GET_COUNTER(&htim1);
    return counter_value;
}
