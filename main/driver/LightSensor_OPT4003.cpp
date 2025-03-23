#include "LightSensor_OPT4003.h"
#include "esp_log.h"

esp_err_t LightSensor_OPT4003::writeRegister(uint8_t reg, uint16_t value)
{
    // Write to register
    uint8_t data[3];
    data[0] = reg;
    data[1] = (value >> 8) & 0xFF;
    data[2] = value & 0xFF;
    return i2c_master_write_to_device(CONFIG_OPT4003_I2C_PORT, CONFIG_OPT4003_ADDR, data, 3, pdMS_TO_TICKS(100));
}

esp_err_t LightSensor_OPT4003::readRegister(uint8_t reg, uint16_t *value)
{
    // Read from register
    uint8_t data[2];
    esp_err_t ret = i2c_master_write_read_device(CONFIG_OPT4003_I2C_PORT, CONFIG_OPT4003_ADDR, &reg, 1, data, 2, pdMS_TO_TICKS(100));
    if (ret == ESP_OK)
    {
        *value = (data[0] << 8) | data[1];
    }
    return ret;
}

bool LightSensor_OPT4003::init()
{
    // Initialize sensor
    esp_err_t ret = writeRegister(CONFIG_OPT4003_REG_CONFIG, CONFIG_OPT4003_CONFIG_START);
    if (ret != ESP_OK)
    {
        ESP_LOGE(LIGHT_SENSOR_TAG, "Failed to write configuration register");
        return false;
    }

    return true;
}

bool LightSensor_OPT4003::lightTriggered()
{
    // Check if light is triggered
    uint16_t value;
    esp_err_t ret = readRegister(CONFIG_OPT4003_REG_RESULT, &value);
    if (ret != ESP_OK)
    {
        ESP_LOGE(LIGHT_SENSOR_TAG, "Failed to read result register");
        return false;
    }

    return (value & 0x8000) != 0; // placeholder
}
