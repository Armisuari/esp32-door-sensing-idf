#pragma once

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "LightSensor_OPT4003_Config.h"
#include "../include/interfaces/LightSensorInterface.h"

class LightSensor_OPT4003 : public LightSensorInterface
{
    public:
        bool init(void (*isr_handler)(void *) = nullptr);
        bool lightTriggered(const float threshold);
    private:
        esp_err_t writeRegister(uint8_t reg, uint16_t value);
        esp_err_t readRegister(uint8_t reg, uint16_t *value);
        float readLux(bool isUSON = true);

        OPT4003Config config;
};