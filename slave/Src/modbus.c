#include "modbus.h"
#include "timer.h"


void MODBUS_Receive_Data(UART_HandleTypeDef *huart)
{
	timer_init();
    HAL_UARTEx_ReceiveToIdle_IT(huart, RxData, RX_DATA_SIZE);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (RxData[SLAVE_ADDRESS_IDX] == SLAVE_ADDRESS)
    {

        switch (RxData[FUNCTION_CODE_IDX])
        {
            case FUNCTION_CODE_OX03:
            	osSemaphoreRelease(ReadHoldingRegsBinarySemHandle);
                break;
            case FUNCTION_CODE_0X06:
                osSemaphoreRelease(WriteSingleRegBinarySemHandle);
                break;
            default:
            	osSemaphoreRelease(ModbusDefaultBinarySemHandle);
        }
    }

    HAL_UARTEx_ReceiveToIdle_IT(huart, RxData, RX_DATA_SIZE);
}
