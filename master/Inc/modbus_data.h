#ifndef INC_MODBUS_DATA_H_
#define INC_MODBUS_DATA_H_

#include <stdint.h>

#define RXDATA_SIZE 16
#define UART_DATA_TIMEOUT 2000


extern uint8_t RxDataGUI[RXDATA_SIZE];
extern uint8_t RxDataSlave[RXDATA_SIZE];

#endif /* INC_MODBUS_DATA_H_ */
