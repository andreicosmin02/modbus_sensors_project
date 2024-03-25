#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include <main.h>
#include <usart.h>
#include <stdint.h>
#include "FreeRTOSThreadConfig.h"
#include "modbus_slave.h"

void MODBUS_Receive_Data(UART_HandleTypeDef *huart);
#endif /* INC_MODBUS_H_ */
