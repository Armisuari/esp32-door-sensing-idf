#include "Accelerometer_LIS2DW12.h"
#include "esp_log.h"

esp_err_t Accelerometer_LIS2DW12::writeRegister(uint8_t reg, uint8_t value)
{
    // Write to register
    uint8_t data[2] = {reg, value};
    return i2c_master_write_to_device(CONFIG_I2C_PORT, config_.getDeviceAddress(), data, 2, pdMS_TO_TICKS(100));
}

esp_err_t Accelerometer_LIS2DW12::readRegister(uint8_t reg, uint8_t *value, size_t len)
{
    // Read from register
    return i2c_master_write_read_device(CONFIG_I2C_PORT, config_.getDeviceAddress(), &reg, 1, value, len, pdMS_TO_TICKS(100));
}

void Accelerometer_LIS2DW12::init(void (*isr_handler)(void *))
{
    // Initialize sensor
    ESP_LOGI(ACCELEROMETER_TAG, "Initializing LIS2DW12");
    config_.sa0 = 0;

    checkDeviceID();
    configureSensor();

    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << CONFIG_LIS2DW12_INT_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(CONFIG_LIS2DW12_INT_PIN , isr_handler, NULL);
}

bool Accelerometer_LIS2DW12::motionDetected()
{
    uint8_t wake_up_src = 0;
    if (readRegister(CONFIG_LIS2DW12_REG_WAKE_UP_SRC, &wake_up_src, 1) == ESP_OK)
    {
        return wake_up_src & 0x08; // Check if WU_IA is set
    }
    ESP_LOGW(ACCELEROMETER_TAG, "Failed to read wake-up source register");
    return false;
}

void Accelerometer_LIS2DW12::checkDeviceID()
{
    uint8_t who_am_i = 0;
    if (readRegister(CONFIG_LIS2DW12_REG_WHO_AM_I, &who_am_i, 1) == ESP_OK)
    {
        if (who_am_i != 0x44)
        {
            ESP_LOGE(ACCELEROMETER_TAG, "Invalid device ID: 0x%02X", who_am_i);
        }
    }
    else
    {
        ESP_LOGE(ACCELEROMETER_TAG, "Failed to read WHO_AM_I register");
    }
}

void Accelerometer_LIS2DW12::configureSensor()
{
    ESP_LOGI(ACCELEROMETER_TAG, "Configuring LIS2DW12");

    if (writeRegister(CONFIG_LIS2DW12_REG_CTRL1, config_.odr | 0x00 | 0x02) != ESP_OK ||
        writeRegister(CONFIG_LIS2DW12_REG_WAKE_UP_DUR, 0x00) != ESP_OK ||
        writeRegister(CONFIG_LIS2DW12_REG_WAKE_UP_THS, 0x00) != ESP_OK ||
        writeRegister(CONFIG_LIS2DW12_REG_CTRL4_INT1, 0x08) != ESP_OK ||
        writeRegister(CONFIG_LIS2DW12_REG_CTRL6, 0x10) != ESP_OK)
    {
        ESP_LOGE(ACCELEROMETER_TAG, "Failed to configure LIS2DW12");
    }
}