#ifndef _Mutex_H
#define _Mutex_H
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
extern QueueHandle_t lvgl_mutex;
#define mutex_lock(mutex) xSemaphoreTake(mutex,portMAX_DELAY) 
#define mutex_unlock(mutex) xSemaphoreGive(mutex)
#endif