#pragma once

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/interfaces/AccelerometerInterface.h"
#include "Accelerometer_LIS2DW12_Config.h"

class Accelerometer_LIS2DW12 : public AccelerometerInterface
{
    public:
        void init();
        bool motionDetected();
    private:
        esp_err_t writeRegister(uint8_t reg, uint8_t value);
        esp_err_t readRegister(uint8_t reg, uint8_t *value, size_t len);
        void checkDeviceID();
        void configureSensor();

        LIS2DW12Config config_;
};