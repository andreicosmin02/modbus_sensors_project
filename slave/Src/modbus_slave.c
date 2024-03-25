#include "modbus_slave.h"

uint8_t RxData[RX_DATA_SIZE];
uint8_t TxData[TX_DATA_SIZE];

ModbusRegister modbus_table[MODBUS_TABLE_SIZE] =
        {
                {"StareLed1", 5000, 1, U8,  READ_WRITE, (ModbusReadFunction) ReadStareLed1, (ModbusWriteFunction) WriteStareLed1},
                {"StareLed2", 5002, 1, U8,  READ_WRITE, (ModbusReadFunction) ReadStareLed2, (ModbusWriteFunction) WriteStareLed2},
                {"StareLed3", 5003, 1, U8,  READ_WRITE, (ModbusReadFunction) ReadStareLed3, (ModbusWriteFunction) WriteStareLed3},
                {"StareLed4", 5004, 1, U8,  READ_WRITE, (ModbusReadFunction) ReadStareLed4, (ModbusWriteFunction) WriteStareLed4},
                {"StareLed5", 5005, 1, U8,  READ_WRITE, (ModbusReadFunction) ReadStareLed5, (ModbusWriteFunction) WriteStareLed5},

                {"DHT11",     7000, 1, U16, READ, (ModbusReadFunction) ReadDHT11, NULL},
                {"HCSR04",    8000, 1, U16, READ, (ModbusReadFunction) ReadHCSR04, NULL}
        };

int MODBUS_FindAddressIndexInTable(ModbusRegister modbus_values[], uint8_t table_size, uint16_t address)
{
    for (uint8_t i = 0; i < table_size; i++)
    {
        if (modbus_values[i].address == address)
        {
            return i;
        }
    }

    return INVALID_ADDRESS_INDEX;
}


void UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    uint16_t crc = crc16(data, size);
    data[size] = crc & MAX_BYTE_VALUE;  // crc low
    data[size + 1] = (crc >> BYTE_SIZE) & MAX_BYTE_VALUE;   // crc high

    HAL_UART_Transmit(huart, data, size + REGISTER_SIZE, UART_TIMEOUT);
}

void MODBUS_Exception(UART_HandleTypeDef *huart, uint8_t exception_code)
{
    TxData[SLAVE_ADDRESS_IDX] = RxData[SLAVE_ADDRESS_IDX];
    TxData[FUNCTION_CODE_IDX] = RxData[FUNCTION_CODE_IDX] | HIGHEST_BIT_SET;
    TxData[BYTE_COUNT_IDX] = exception_code;
    UART_Send_Data(huart, TxData, 3);
}

uint8_t MODBUS_Read_Holding_Regs(UART_HandleTypeDef *huart)
{
    uint8_t exception_code;
    uint16_t start_addr = (RxData[REG_ADD_HIGH_IDX] << BYTE_SIZE) | RxData[REG_ADD_LOW_IDX];

    uint16_t num_regs = (RxData[NO_OF_PTS_HIGH_IDX] << BYTE_SIZE) | RxData[NO_OF_PTS_LOW_IDX];
    if ((num_regs < MINIMUM_WORDS_NO) || (num_regs > MAXIMUM_WORDS_NO))
    {
        exception_code = ILLEGAL_DATA_VALUE;
        osMessageQueuePut(ModbusExceptionQueueHandle, &exception_code, FREERTOS_MESSAGE_PRIORITY,
                          FREERTOS_QUEUE_PUT_TIMEOUT);
        osSemaphoreRelease(ModbusExceptionBinarySemHandle);
        return MODBUS_FAILURE;
    }


    TxData[SLAVE_ADDRESS_IDX] = SLAVE_ADDRESS;
    TxData[FUNCTION_CODE_IDX] = RxData[FUNCTION_CODE_IDX];
    TxData[BYTE_COUNT_IDX] = num_regs * REGISTER_SIZE;

    int indx = DATA_START_IDX;
    for (uint16_t i = 0; i < num_regs; i++)
    {
        int address_index = MODBUS_FindAddressIndexInTable(modbus_table, MODBUS_TABLE_SIZE, start_addr);
        if (address_index == INVALID_ADDRESS_INDEX)
        {
            exception_code = ILLEGAL_DATA_ADDRESS;
            osMessageQueuePut(ModbusExceptionQueueHandle, &exception_code, FREERTOS_MESSAGE_PRIORITY,
                              FREERTOS_QUEUE_PUT_TIMEOUT);
            osSemaphoreRelease(ModbusExceptionBinarySemHandle);
            return MODBUS_FAILURE;
        }

        uint16_t modbus_value = (uint16_t) modbus_table[address_index].readFunction();
        if (modbus_value == READ_HOLDING_REGS_ERROR)
        {
        	exception_code = ILLEGAL_DATA_VALUE;
        	osMessageQueuePut(ModbusExceptionQueueHandle, &exception_code, FREERTOS_MESSAGE_PRIORITY,
        	                              FREERTOS_QUEUE_PUT_TIMEOUT);
        	osSemaphoreRelease(ModbusExceptionBinarySemHandle);
        	return MODBUS_FAILURE;
        }

        TxData[indx++] = modbus_value >> BYTE_SIZE & MAX_BYTE_VALUE;
        TxData[indx++] = modbus_value & MAX_BYTE_VALUE;

        start_addr++;
    }

    UART_Send_Data(huart, TxData, indx);
    return MODBUS_SUCCESS;
}

uint8_t MODBUS_Write_Single_Reg(UART_HandleTypeDef *huart)
{
    uint8_t exception_code;
    uint16_t start_addr = ((RxData[REG_ADD_HIGH_IDX] << BYTE_SIZE) | RxData[REG_ADD_LOW_IDX]);

    int address_index = MODBUS_FindAddressIndexInTable(modbus_table, MODBUS_TABLE_SIZE, start_addr);
    if (address_index == INVALID_ADDRESS_INDEX)
    {
        exception_code = ILLEGAL_DATA_ADDRESS;
        osMessageQueuePut(ModbusExceptionQueueHandle, &exception_code, FREERTOS_MESSAGE_PRIORITY,
                          FREERTOS_QUEUE_PUT_TIMEOUT);
        osSemaphoreRelease(ModbusExceptionBinarySemHandle);
        return MODBUS_FAILURE;
    }
    if (modbus_table[address_index].access_type == READ)
    {
        exception_code = ILLEGAL_DATA_ADDRESS;
        osMessageQueuePut(ModbusExceptionQueueHandle, &exception_code, FREERTOS_MESSAGE_PRIORITY,
                          FREERTOS_QUEUE_PUT_TIMEOUT);
        osSemaphoreRelease(ModbusExceptionBinarySemHandle);
        return MODBUS_FAILURE;
    }

    uint16_t value_to_write = (RxData[VALUE_WRITTEN_HIGH_INDEX] << BYTE_SIZE | RxData[VALUE_WRITTEN_LOW_INDEX]);
    modbus_table[address_index].writeFunction(value_to_write);

    TxData[SLAVE_ADDRESS_IDX] = SLAVE_ADDRESS;
    TxData[FUNCTION_CODE_IDX] = RxData[FUNCTION_CODE_IDX];
    TxData[DATA_RESPONSE_HIGH_INDEX] = RxData[REG_ADD_HIGH_IDX];
    TxData[DATA_RESPONSE_LOW_INDEX] = RxData[REG_ADD_LOW_IDX];
    TxData[VALUE_WRITTEN_HIGH_INDEX] = RxData[NO_OF_PTS_HIGH_IDX];
    TxData[VALUE_WRITTEN_LOW_INDEX] = RxData[NO_OF_PTS_LOW_IDX];

    UART_Send_Data(huart, TxData, CRC_HIGH_IDX);
    return MODBUS_SUCCESS;
}

