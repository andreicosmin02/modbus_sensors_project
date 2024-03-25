#ifndef INC_MODBUS_READ_WRITE_H_
#define INC_MODBUS_READ_WRITE_H_

#include <stdint.h>
#include "main.h"

#define BYTE_SIZE 					8
#define READ_HOLDING_REGS_ERROR		0xFFFF

uint8_t ReadStareLed1(void);

uint8_t ReadStareLed2(void);

uint8_t ReadStareLed3(void);

uint8_t ReadStareLed4(void);

uint8_t ReadStareLed5(void);

uint16_t ReadDHT11(void);

uint16_t ReadHCSR04(void);

void WriteStareLed1(uint16_t stare);

void WriteStareLed2(uint16_t stare);

void WriteStareLed3(uint16_t stare);

void WriteStareLed4(uint16_t stare);

void WriteStareLed5(uint16_t stare);

#endif /* INC_MODBUS_READ_WRITE_H_ */
