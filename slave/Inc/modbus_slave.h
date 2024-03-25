#ifndef INC_MODBUS_SLAVE_H_
#define INC_MODBUS_SLAVE_H_

#include <usart.h>
#include <stdint.h>
#include "modbus_crc.h"
#include "FreeRTOSThreadConfig.h"
#include "modbus_read_write.h"

#define RX_DATA_SIZE                32
#define TX_DATA_SIZE                32
#define SLAVE_ADDRESS               0x05
#define FUNCTION_CODE_OX03          0x03
#define FUNCTION_CODE_0X06          0x06
#define ILLEGAL_FUNCTION            0x01
#define ILLEGAL_DATA_ADDRESS        0x02
#define ILLEGAL_DATA_VALUE          0x03
#define FREERTOS_MESSAGE_PRIORITY   0
#define FREERTOS_QUEUE_PUT_TIMEOUT  200
#define FREERTOS_QUEUE_GET_TIMEOUT  4000
#define UART_TIMEOUT                1000

#define BYTE_SIZE                   8
#define MAX_BYTE_VALUE              0xFF
#define REGISTER_SIZE               2
#define HIGHEST_BIT_SET             0x80    // 0b10000000

#define MINIMUM_WORDS_NO            1
#define MAXIMUM_WORDS_NO            123

#define REGISTERS_NO                50

#define MODBUS_TABLE_SIZE           7
#define MODBUS_FAILURE              0
#define MODBUS_SUCCESS              1

#define INVALID_ADDRESS_INDEX       (-1)


enum MODBUS_Function_Code_03_Request
{
    SLAVE_ADDRESS_IDX = 0,
    FUNCTION_CODE_IDX,
    REG_ADD_HIGH_IDX,
    REG_ADD_LOW_IDX,
    NO_OF_PTS_HIGH_IDX,
    NO_OF_PTS_LOW_IDX,
    CRC_HIGH_IDX,
    CRC_LOW_IDx
};

enum MODBUS_Function_Code_06_Response
{
    DATA_RESPONSE_HIGH_INDEX = 2,
    DATA_RESPONSE_LOW_INDEX,
    VALUE_WRITTEN_HIGH_INDEX,
    VALUE_WRITTEN_LOW_INDEX,

};

enum MODBUS_Message_To_Send_Indexes
{
    BYTE_COUNT_IDX = 2,
    DATA_START_IDX
};

enum MODBUS_Access_Type
{
    READ,
    READ_WRITE
};

enum MODBUS_Data_Type
{
    U8 = 1,
    U16 = 2,
};


typedef uint16_t (*ModbusReadFunction)();

typedef void (*ModbusWriteFunction)(uint16_t);

typedef struct
{
    char name[20];
    uint16_t address;
    uint8_t no_of_registers;
    uint8_t data_type;
    uint8_t access_type;
    ModbusReadFunction readFunction;
    ModbusWriteFunction writeFunction;
} ModbusRegister;

extern ModbusRegister modbus_table[MODBUS_TABLE_SIZE];


extern uint8_t RxData[RX_DATA_SIZE];
extern uint8_t TxData[TX_DATA_SIZE];

int MODBUS_FindAddressIndexInTable(ModbusRegister modbus_values[], uint8_t table_size, uint16_t address);

void UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

void MODBUS_Exception(UART_HandleTypeDef *huart, uint8_t exception_code);

uint8_t MODBUS_Read_Holding_Regs(UART_HandleTypeDef *huart);

uint8_t MODBUS_Write_Single_Reg(UART_HandleTypeDef *huart);

#endif /* INC_MODBUS_SLAVE_H_ */
