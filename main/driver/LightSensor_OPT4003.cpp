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

bool LightSensor_OPT4003::init(void (*isr_handler)(void *))
{
    // Initialize sensor
    config = OPT4003Config()
                .setReset()
                .setContinuousMode()
                .setAutoRange()
                .setConversionTime(OPT4003Config::CT_1_6MS);

    // Write configuration register to set continuous mode
    esp_err_t ret = writeRegister(CONFIG_OPT4003_REG_CONFIG, config.build());
    if (ret != ESP_OK)
    {
        ESP_LOGE(LIGHT_SENSOR_TAG, "Failed to write configuration register");
        return false;
    }

    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << CONFIG_OPT4003_INT_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(CONFIG_OPT4003_INT_PIN, isr_handler, NULL);

    return true;
}

bool LightSensor_OPT4003::lightTriggered(const float threshold)
{
    // Check if light is triggered
    if (readLux() > threshold)
    {
        return true;
    }

    return false;
}

float LightSensor_OPT4003::readLux(bool isUSON)
{
    uint16_t result_msb;
    uint8_t result_lsb;

    // Read RESULT_MSB_CH0 (0x00) – 16 bits
    esp_err_t ret = readRegister(CONFIG_OPT4003_REG_RESULT_MSB_CH0, &result_msb);
    if (ret != ESP_OK) {
        return -1.0f;
    }

    // Read RESULT_LSB_CH0 (0x01) – 8 bits (we read it as 16-bit, but only use low byte)
    uint16_t temp;
    ret = readRegister(CONFIG_OPT4003_REG_RESULT_LSB_CH0, &temp);
    if (ret != ESP_OK) {
        return -1.0f;
    }
    result_lsb = static_cast<uint8_t>(temp & 0xFF);

    uint8_t exponent = (result_msb >> 12) & 0x0F;
    uint32_t mantissa = ((result_msb & 0x0FFF) << 8) | result_lsb;
    uint32_t adc_codes = mantissa * (1 << exponent);

    // Convert to lux using variant-specific scale factor
    const float scale = isUSON ? 0.000535f : 0.000585f;

    return adc_codes * scale;
}

OPT4003Config::OPT4003Config() : configReg(0) {}

OPT4003Config& OPT4003Config::setReset(bool enable) {
    if (enable)
        configReg |= (1 << 15);
    else
        configReg &= ~(1 << 15);
    return *this;
}

OPT4003Config& OPT4003Config::setContinuousMode(bool enable) {
    if (enable)
        configReg |= (1 << 6);
    else
        configReg &= ~(1 << 6);
    return *this;
}

OPT4003Config& OPT4003Config::setConversionTime(ConversionTime ct) {
    configReg &= ~(0b111 << 7); // Clear bits 9:7
    configReg |= (ct << 7);     // Set new CT value
    return *this;
}

OPT4003Config& OPT4003Config::setAutoRange() {
    return setRange(RANGE_AUTO);
}

OPT4003Config& OPT4003Config::setRange(RangeSetting range) {
    configReg &= ~(0b111 << 11);     // Clear bits 13:11
    configReg |= (range << 11);      // Set new range
    return *this;
}

uint16_t OPT4003Config::build() const {
    return configReg;
}
