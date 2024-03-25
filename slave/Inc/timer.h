#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>
#include "tim.h"

void timer_init(void);
void delay_us(uint16_t us);
uint16_t get_counter(void);

#endif /* INC_TIMER_H_ */
