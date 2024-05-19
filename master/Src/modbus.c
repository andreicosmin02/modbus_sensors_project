#include "modbus.h"
#include "gpio.h"
#include "usart.h"
#include "modbus_data.h"

uint8_t RxDataGUI[RXDATA_SIZE];
uint8_t RxDataSlave[RXDATA_SIZE];

void MODBUS_Receive_Data()
{
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxDataGUI, RXDATA_SIZE);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
    	osSemaphoreRelease(transmitToSlaveBinarySemHandle);
    }
    else if (huart->Instance == USART1)
    {
    	osSemaphoreRelease(transmitToGUIBinarySemHandle);
    }
}
