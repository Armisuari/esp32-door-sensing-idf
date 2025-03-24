#include "Accelerometer_LIS2DW12.h"
#include "esp_log.h"

esp_err_t Accelerometer_LIS2DW12::writeRegister(uint8_t reg, uint8_t value)
{
    // Write to register
    uint8_t data[2] = {reg, value};
    return i2c_master_write_to_device(CONFIG_I2C_PORT, CONFIG_LIS2DW12_ADDR, data, 2, pdMS_TO_TICKS(100));
}

esp_err_t Accelerometer_LIS2DW12::readRegister(uint8_t reg, uint8_t *value)
{
    // Read from register
    return i2c_master_write_read_device(CONFIG_I2C_PORT, CONFIG_LIS2DW12_ADDR, &reg, 1, value, 1, pdMS_TO_TICKS(100));
}

void Accelerometer_LIS2DW12::init()
{
    // Initialize sensor
    ESP_LOGI(ACCELEROMETER_TAG, "Initializing LIS2DW12");

    writeRegister(CONFIG_LIS2DW12_REG_CTRL1, 0x50);  // ODR = 100Hz, mode = low power
    writeRegister(CONFIG_LIS2DW12_REG_CTRL6, 0x10);  // Low power mode 1
    writeRegister(CONFIG_LIS2DW12_REG_WAKE_UP_THS, 0x02);  // Wake-up threshold
    writeRegister(CONFIG_LIS2DW12_REG_WAKE_UP_DUR, 0x00);  // No delay
    writeRegister(CONFIG_LIS2DW12_REG_CTRL4_INT1, 0x20);  // Enable wake-up interrupt on INT1
}

bool Accelerometer_LIS2DW12::motionDetected()
{
    // Check if motion is detected
    uint8_t src;
    if (readRegister(CONFIG_LIS2DW12_REG_WAKE_UP_SRC, &src) == ESP_OK) {
        if (src & 0x08) { // Wake-up event detected
            ESP_LOGI(ACCELEROMETER_TAG, "Motion detected (WAKE_UP_SRC: 0x%02X)", src);
            return true;
        }
    }
    return false;
}