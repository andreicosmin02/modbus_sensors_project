#ifndef INC_FREERTOSTHREADCONFIG_H_
#define INC_FREERTOSTHREADCONFIG_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"



/* Definitions for ReadHoldingRegs */
extern osThreadId_t ReadHoldingRegsHandle;
extern const osThreadAttr_t ReadHoldingRegs_attributes;

/* Definitions for WriteSingleReg */
extern osThreadId_t WriteSingleRegHandle;
extern const osThreadAttr_t WriteSingleReg_attributes;

/* Definitions for ModbusException */
extern osThreadId_t ModbusExceptionHandle;
extern const osThreadAttr_t ModbusException_attributes;

/* Definitions for ModbusDefaultFC */
extern osThreadId_t ModbusDefaultFCHandle;
extern const osThreadAttr_t ModbusDefaultFC_attributes;

/* Definitions for ModbusExceptionQueue */
extern osMessageQueueId_t ModbusExceptionQueueHandle;
extern const osMessageQueueAttr_t ModbusExceptionQueue_attributes;

/* Definitions for ReadHoldingRegsBinarySem */
extern osSemaphoreId_t ReadHoldingRegsBinarySemHandle;
extern const osSemaphoreAttr_t ReadHoldingRegsBinarySem_attributes;

/* Definitions for WriteSingleRegBinarySem */
extern osSemaphoreId_t WriteSingleRegBinarySemHandle;
extern const osSemaphoreAttr_t WriteSingleRegBinarySem_attributes;

/* Definitions for ModbusExceptionBinarySem */
extern osSemaphoreId_t ModbusExceptionBinarySemHandle;
extern const osSemaphoreAttr_t ModbusExceptionBinarySem_attributes;

/* Definitions for ModbusDefaultBinarySem */
extern osSemaphoreId_t ModbusDefaultBinarySemHandle;
extern const osSemaphoreAttr_t ModbusDefaultBinarySem_attributes;

#endif /* INC_FREERTOSTHREADCONFIG_H_ */
