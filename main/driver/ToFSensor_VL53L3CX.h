#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "vl53lx_api.h"
#include "vl53lx_platform.h"

#include <../include/interfaces/ToFSensorInterface.h>
#include "ToFSensor_VL53L3CX_Config.h"

class ToFSensor_VL53L3CX : public ToFSensorInterface
{
    public:
        ToFSensor_VL53L3CX();
        void powerOn();
        void powerOff();
        bool init();
        void handleInterrupt();
        uint16_t getDistance() { return last_distance_mm; }
        
    private:
        static void IRAM_ATTR gpio_isr_handler(void* arg);
        static void isr_task_entry(void* arg);   // Task wrapper

        VL53LX_Dev_t dev;
        VL53LX_DEV Dev;
        VL53L3CXConfig cfg;

        QueueHandle_t gpio_evt_queue_ = nullptr;
        TaskHandle_t task_handle_ = nullptr;
        uint16_t last_distance_mm = -1;
};