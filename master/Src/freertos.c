/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usart.h"
#include "modbus_data.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for transmitToSlav */
osThreadId_t transmitToSlavHandle;
const osThreadAttr_t transmitToSlav_attributes = {
  .name = "transmitToSlav",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for transmitToGUI */
osThreadId_t transmitToGUIHandle;
const osThreadAttr_t transmitToGUI_attributes = {
  .name = "transmitToGUI",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for transmitToSlaveBinarySem */
osSemaphoreId_t transmitToSlaveBinarySemHandle;
const osSemaphoreAttr_t transmitToSlaveBinarySem_attributes = {
  .name = "transmitToSlaveBinarySem"
};
/* Definitions for transmitToGUIBinarySem */
osSemaphoreId_t transmitToGUIBinarySemHandle;
const osSemaphoreAttr_t transmitToGUIBinarySem_attributes = {
  .name = "transmitToGUIBinarySem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void TransmitToSlaveTask(void *argument);
void TransmitToGUITask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of transmitToSlaveBinarySem */
  transmitToSlaveBinarySemHandle = osSemaphoreNew(1, 0, &transmitToSlaveBinarySem_attributes);

  /* creation of transmitToGUIBinarySem */
  transmitToGUIBinarySemHandle = osSemaphoreNew(1, 0, &transmitToGUIBinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of transmitToSlav */
  transmitToSlavHandle = osThreadNew(TransmitToSlaveTask, NULL, &transmitToSlav_attributes);

  /* creation of transmitToGUI */
  transmitToGUIHandle = osThreadNew(TransmitToGUITask, NULL, &transmitToGUI_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_TransmitToSlaveTask */
/**
  * @brief  Function implementing the transmitToSlav thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_TransmitToSlaveTask */
void TransmitToSlaveTask(void *argument)
{
  /* USER CODE BEGIN TransmitToSlaveTask */
    /* Infinite loop */
    for (;;)
    {
        if (osSemaphoreAcquire(transmitToSlaveBinarySemHandle, osWaitForever) == osOK)
        {
        	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxDataGUI, RXDATA_SIZE);

        	HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxDataSlave, RXDATA_SIZE);


            HAL_UART_Transmit(&huart1, RxDataGUI, RXDATA_SIZE, UART_DATA_TIMEOUT);
        }
    }
  /* USER CODE END TransmitToSlaveTask */
}

/* USER CODE BEGIN Header_TransmitToGUITask */
/**
* @brief Function implementing the transmitToGUI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TransmitToGUITask */
void TransmitToGUITask(void *argument)
{
  /* USER CODE BEGIN TransmitToGUITask */
    /* Infinite loop */
    for (;;)
    {
        if (osSemaphoreAcquire(transmitToGUIBinarySemHandle, osWaitForever) == osOK)
        {
        	HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxDataSlave, RXDATA_SIZE);

        	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxDataGUI, RXDATA_SIZE);

        	uint8_t data_to_send_size = 5;	// Size if there is an exception
        	if (RxDataSlave[1] == 0x03)
        	{
        		data_to_send_size = RxDataSlave[2] + 5;	// Size for the function code 0x03
        	}
        	else if(RxDataSlave[1] == 0x06)
        	{
        		data_to_send_size = 8;	// Size for the function code 0x06
        	}

            HAL_UART_Transmit(&huart2, RxDataSlave, data_to_send_size, UART_DATA_TIMEOUT);
        }
    }
  /* USER CODE END TransmitToGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

