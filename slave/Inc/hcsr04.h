#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_


#include "main.h"
#include <timer.h>


#define SET (GPIO_PinState)1
#define RESET (GPIO_PinState)0

uint16_t hcsr04_distance(GPIO_TypeDef* GPIOx, uint16_t PIN_Trig, uint16_t PIN_Echo);


#endif /* INC_HCSR04_H_ */
