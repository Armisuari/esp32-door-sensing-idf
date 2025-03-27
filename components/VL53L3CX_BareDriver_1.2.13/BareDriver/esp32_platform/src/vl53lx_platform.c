// ESP32-compatible VL53LX platform implementation for basic usage

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "driver/i2c.h"
#include "vl53lx_platform.h"

#define I2C_PORT I2C_NUM_0
#define I2C_TIMEOUT_MS 1000

VL53LX_Error VL53LX_WriteMulti(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *pdata, uint32_t count) {
    uint8_t buffer[258];
    buffer[0] = index >> 8;
    buffer[1] = index & 0xFF;
    memcpy(&buffer[2], pdata, count);

    esp_err_t ret = i2c_master_write_to_device(I2C_PORT, pdev->i2c_slave_address, buffer, count + 2, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    return (ret == ESP_OK) ? VL53LX_ERROR_NONE : VL53LX_ERROR_CONTROL_INTERFACE;
}

VL53LX_Error VL53LX_ReadMulti(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *pdata, uint32_t count) {
    uint8_t reg[2] = { index >> 8, index & 0xFF };
    esp_err_t ret = i2c_master_write_read_device(I2C_PORT, pdev->i2c_slave_address, reg, 2, pdata, count, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    return (ret == ESP_OK) ? VL53LX_ERROR_NONE : VL53LX_ERROR_CONTROL_INTERFACE;
}

VL53LX_Error VL53LX_WrByte(VL53LX_Dev_t *pdev, uint16_t index, uint8_t data) {
    return VL53LX_WriteMulti(pdev, index, &data, 1);
}

VL53LX_Error VL53LX_WrWord(VL53LX_Dev_t *pdev, uint16_t index, uint16_t data) {
    uint8_t buffer[2] = { (uint8_t)(data >> 8), (uint8_t)(data & 0xFF) };
    return VL53LX_WriteMulti(pdev, index, buffer, 2);
}

VL53LX_Error VL53LX_WrDWord(VL53LX_Dev_t *pdev, uint16_t index, uint32_t data) {
    uint8_t buffer[4] = {
        (uint8_t)(data >> 24),
        (uint8_t)((data >> 16) & 0xFF),
        (uint8_t)((data >> 8) & 0xFF),
        (uint8_t)(data & 0xFF)
    };
    return VL53LX_WriteMulti(pdev, index, buffer, 4);
}

VL53LX_Error VL53LX_RdByte(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *data) {
    return VL53LX_ReadMulti(pdev, index, data, 1);
}

VL53LX_Error VL53LX_RdWord(VL53LX_Dev_t *pdev, uint16_t index, uint16_t *data) {
    uint8_t buffer[2];
    VL53LX_Error status = VL53LX_ReadMulti(pdev, index, buffer, 2);
    *data = ((uint16_t)buffer[0] << 8) | buffer[1];
    return status;
}

VL53LX_Error VL53LX_RdDWord(VL53LX_Dev_t *pdev, uint16_t index, uint32_t *data) {
    uint8_t buffer[4];
    VL53LX_Error status = VL53LX_ReadMulti(pdev, index, buffer, 4);
    *data = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) |
            ((uint32_t)buffer[2] << 8) | buffer[3];
    return status;
}

VL53LX_Error VL53LX_WaitMs(VL53LX_Dev_t *pdev, int32_t wait_ms) {
    (void)pdev;
    vTaskDelay(pdMS_TO_TICKS(wait_ms));
    return VL53LX_ERROR_NONE;
}

VL53LX_Error VL53LX_WaitUs(VL53LX_Dev_t *pdev, int32_t wait_us) {
    (void)pdev;
    ets_delay_us(wait_us);
    return VL53LX_ERROR_NONE;
}

VL53LX_Error VL53LX_GetTickCount(VL53LX_Dev_t *pdev, uint32_t *ptick_count_ms) {
    (void)pdev;
    *ptick_count_ms = (uint32_t)(esp_timer_get_time() / 1000);
    return VL53LX_ERROR_NONE;
}
