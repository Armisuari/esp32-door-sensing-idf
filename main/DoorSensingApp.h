#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "../include/interfaces/LightSensorInterface.h"
#include "../include/interfaces/AccelerometerInterface.h"
#include "../include/interfaces/ToFSensorInterface.h"

#define DOOR_THRESHOLD_MM 300

// States
typedef enum {
    STATE_IDLE,
    STATE_ENABLE_TOF,
    STATE_CHECK_DOOR_STATUS,
    STATE_TOF_OFF
} door_state_t;

class DoorSensingApp
{
    public:
        DoorSensingApp(LightSensorInterface &lightSensor, AccelerometerInterface &accelerometer, ToFSensorInterface &tofSensor);
        void init();
        void run();

    private:
        LightSensorInterface &_lightSensor;
        AccelerometerInterface &_accelerometer;
        ToFSensorInterface &_tofSensor;

        static bool motion_detected;
        static bool light_change_detected;

        static door_state_t current_state;

        static void IRAM_ATTR accelerometer_isr_handler(void* arg);
        static void IRAM_ATTR light_sensor_isr_handler(void* arg);

        void handleIdleState();
        void handleEnableToFState();
        void handleCheckDoorStatusState();
        void handleToFOffState();
};