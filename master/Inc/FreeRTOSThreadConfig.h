#ifndef INC_FREERTOSTHREADCONFIG_H_
#define INC_FREERTOSTHREADCONFIG_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"



/* Definitions for transmitToSlav */
extern osThreadId_t transmitToSlavHandle;
extern const osThreadAttr_t transmitToSlav_attributes;

/* Definitions for transmitToGUI */
extern osThreadId_t transmitToGUIHandle;
extern const osThreadAttr_t transmitToGUI_attributes;

/* Definitions for transmitToSlaveBinarySem */
extern osSemaphoreId_t transmitToSlaveBinarySemHandle;
extern const osSemaphoreAttr_t transmitToSlaveBinarySem_attributes;

/* Definitions for transmitToGUIBinarySem */
extern osSemaphoreId_t transmitToGUIBinarySemHandle;
extern const osSemaphoreAttr_t transmitToGUIBinarySem_attributes;


#endif /* INC_FREERTOSTHREADCONFIG_H_ */
