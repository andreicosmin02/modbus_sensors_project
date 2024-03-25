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

#include <usart.h>
#include "FreeRTOSThreadConfig.h"
#include "modbus_slave.h"

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
/* Definitions for ReadHoldingRegs */
osThreadId_t ReadHoldingRegsHandle;
const osThreadAttr_t ReadHoldingRegs_attributes = {
  .name = "ReadHoldingRegs",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for WriteSingleReg */
osThreadId_t WriteSingleRegHandle;
const osThreadAttr_t WriteSingleReg_attributes = {
  .name = "WriteSingleReg",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ModbusException */
osThreadId_t ModbusExceptionHandle;
const osThreadAttr_t ModbusException_attributes = {
  .name = "ModbusException",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ModbusDefaultFC */
osThreadId_t ModbusDefaultFCHandle;
const osThreadAttr_t ModbusDefaultFC_attributes = {
  .name = "ModbusDefaultFC",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ModbusExceptionQueue */
osMessageQueueId_t ModbusExceptionQueueHandle;
const osMessageQueueAttr_t ModbusExceptionQueue_attributes = {
  .name = "ModbusExceptionQueue"
};
/* Definitions for ReadHoldingRegsBinarySem */
osSemaphoreId_t ReadHoldingRegsBinarySemHandle;
const osSemaphoreAttr_t ReadHoldingRegsBinarySem_attributes = {
  .name = "ReadHoldingRegsBinarySem"
};
/* Definitions for WriteSingleRegBinarySem */
osSemaphoreId_t WriteSingleRegBinarySemHandle;
const osSemaphoreAttr_t WriteSingleRegBinarySem_attributes = {
  .name = "WriteSingleRegBinarySem"
};
/* Definitions for ModbusExceptionBinarySem */
osSemaphoreId_t ModbusExceptionBinarySemHandle;
const osSemaphoreAttr_t ModbusExceptionBinarySem_attributes = {
  .name = "ModbusExceptionBinarySem"
};
/* Definitions for ModbusDefaultBinarySem */
osSemaphoreId_t ModbusDefaultBinarySemHandle;
const osSemaphoreAttr_t ModbusDefaultBinarySem_attributes = {
  .name = "ModbusDefaultBinarySem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ReadHoldingRegsTask(void *argument);
void WriteSingleRegTask(void *argument);
void ModbusExceptionTask(void *argument);
void ModbusDefaultFunctionCodeTask(void *argument);

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
  /* creation of ReadHoldingRegsBinarySem */
  ReadHoldingRegsBinarySemHandle = osSemaphoreNew(1, 0, &ReadHoldingRegsBinarySem_attributes);

  /* creation of WriteSingleRegBinarySem */
  WriteSingleRegBinarySemHandle = osSemaphoreNew(1, 0, &WriteSingleRegBinarySem_attributes);

  /* creation of ModbusExceptionBinarySem */
  ModbusExceptionBinarySemHandle = osSemaphoreNew(1, 0, &ModbusExceptionBinarySem_attributes);

  /* creation of ModbusDefaultBinarySem */
  ModbusDefaultBinarySemHandle = osSemaphoreNew(1, 0, &ModbusDefaultBinarySem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ModbusExceptionQueue */
  ModbusExceptionQueueHandle = osMessageQueueNew (8, sizeof(uint8_t), &ModbusExceptionQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ReadHoldingRegs */
  ReadHoldingRegsHandle = osThreadNew(ReadHoldingRegsTask, NULL, &ReadHoldingRegs_attributes);

  /* creation of WriteSingleReg */
  WriteSingleRegHandle = osThreadNew(WriteSingleRegTask, NULL, &WriteSingleReg_attributes);

  /* creation of ModbusException */
  ModbusExceptionHandle = osThreadNew(ModbusExceptionTask, NULL, &ModbusException_attributes);

  /* creation of ModbusDefaultFC */
  ModbusDefaultFCHandle = osThreadNew(ModbusDefaultFunctionCodeTask, NULL, &ModbusDefaultFC_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ReadHoldingRegsTask */
/**
  * @brief  Function implementing the ReadHoldingRegs thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ReadHoldingRegsTask */
void ReadHoldingRegsTask(void *argument)
{
  /* USER CODE BEGIN ReadHoldingRegsTask */
    /* Infinite loop */
    for (;;)
    {
        if (osSemaphoreAcquire(ReadHoldingRegsBinarySemHandle, osWaitForever) == osOK)
        {
            MODBUS_Read_Holding_Regs(&huart2);
        }
    }
  /* USER CODE END ReadHoldingRegsTask */
}

/* USER CODE BEGIN Header_WriteSingleRegTask */
/**
* @brief Function implementing the WriteSingleReg thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WriteSingleRegTask */
void WriteSingleRegTask(void *argument)
{
  /* USER CODE BEGIN WriteSingleRegTask */
    /* Infinite loop */
    for (;;)
    {
        if (osSemaphoreAcquire(WriteSingleRegBinarySemHandle, osWaitForever) == osOK)
        {
            MODBUS_Write_Single_Reg(&huart2);
        }

    }
  /* USER CODE END WriteSingleRegTask */
}

/* USER CODE BEGIN Header_ModbusExceptionTask */
/**
* @brief Function implementing the ModbusException thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ModbusExceptionTask */
void ModbusExceptionTask(void *argument)
{
  /* USER CODE BEGIN ModbusExceptionTask */

    uint8_t exception_code;

    /* Infinite loop */
    for (;;)
    {
        if (osSemaphoreAcquire(ModbusExceptionBinarySemHandle, osWaitForever) == osOK)
        {
            if (osMessageQueueGet(ModbusExceptionQueueHandle, &exception_code, NULL, FREERTOS_QUEUE_GET_TIMEOUT) == osOK)
            {
                MODBUS_Exception(&huart2, exception_code);
            }

        }
    }
  /* USER CODE END ModbusExceptionTask */
}

/* USER CODE BEGIN Header_ModbusDefaultFunctionCodeTask */
/**
* @brief Function implementing the ModbusDefaultFC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ModbusDefaultFunctionCodeTask */
void ModbusDefaultFunctionCodeTask(void *argument)
{
  /* USER CODE BEGIN ModbusDefaultFunctionCodeTask */
  /* Infinite loop */
  for(;;)
  {
      if (osSemaphoreAcquire(ModbusDefaultBinarySemHandle, 2000) == osOK)
      {
          uint8_t illegal_function_value = ILLEGAL_FUNCTION;
          osMessageQueuePut(ModbusExceptionQueueHandle, &illegal_function_value, FREERTOS_MESSAGE_PRIORITY,
                            FREERTOS_QUEUE_PUT_TIMEOUT);
          osSemaphoreRelease(ModbusExceptionBinarySemHandle);
      }
  }
  /* USER CODE END ModbusDefaultFunctionCodeTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

