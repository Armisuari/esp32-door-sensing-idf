#include "ToFSensor_VL53L3CX.h"
#include "esp_log.h"

ToFSensor_VL53L3CX::ToFSensor_VL53L3CX()
{
    dev.i2c_slave_address = cfg.i2c_addr;
    // dev.I2cHandle = nullptr;
    Dev = &dev;
}

void ToFSensor_VL53L3CX::powerOn()
{
    gpio_set_level(cfg.xshut_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
}

void ToFSensor_VL53L3CX::powerOff()
{
    gpio_set_level(cfg.xshut_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
}

bool ToFSensor_VL53L3CX::init()
{
    gpio_pad_select_gpio(cfg.xshut_pin);
    gpio_set_direction(cfg.xshut_pin, GPIO_MODE_OUTPUT);
    powerOn();

    ESP_LOGI(TOF_SENSOR_TAG, "Initializing VL53L3CX...");
    if (VL53LX_WaitDeviceBooted(Dev) != 0)
    {
        ESP_LOGE(TOF_SENSOR_TAG, "Device not booted!");
        return false;
    }

    if (VL53LX_DataInit(Dev) != 0)
    {
        ESP_LOGE(TOF_SENSOR_TAG, "Data init failed!");
        return false;
    }

    if (VL53LX_StartMeasurement(Dev) != 0)
    {
        ESP_LOGE(TOF_SENSOR_TAG, "Start measurement failed!");
        return false;
    }

    gpio_evt_queue_ = xQueueCreate(10, sizeof(uint32_t));
    if (gpio_evt_queue_ == nullptr)
    {
        ESP_LOGE(TOF_SENSOR_TAG, "Failed to create GPIO event queue");
        return false;
    }

    gpio_pad_select_gpio(cfg.int_pin);
    gpio_set_direction(cfg.int_pin, GPIO_MODE_INPUT);
    gpio_set_intr_type(cfg.int_pin, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(cfg.int_pin, gpio_isr_handler, (void *)cfg.int_pin);

    xTaskCreatePinnedToCore(isr_task_entry, "tof_isr_task", 4096, this, 10, &task_handle_, 1);

    return true;
}

void ToFSensor_VL53L3CX::handleInterrupt()
{
    VL53LX_MultiRangingData_t ranging_data;
    VL53LX_MultiRangingData_t *pRangingData = &ranging_data;

    if (VL53LX_GetMultiRangingData(Dev, pRangingData) == 0)
    {
        ESP_LOGD(TOF_SENSOR_TAG, "Objects: %d", pRangingData->NumberOfObjectsFound);
        if (pRangingData->NumberOfObjectsFound > 0)
        {
            last_distance_mm = pRangingData->RangeData[0].RangeMilliMeter;
        }
        else
        {
            last_distance_mm = -1; // No object found
        }

        for (int i = 0; i < pRangingData->NumberOfObjectsFound; i++)
        {
            ESP_LOGD(TOF_SENSOR_TAG, "Object %d: %d mm", i, pRangingData->RangeData[i].RangeMilliMeter);
        }
        VL53LX_ClearInterruptAndStartMeasurement(Dev);
    }
    else
    {
        ESP_LOGE(TOF_SENSOR_TAG, "Failed to get multi-ranging data");
    }
}

void IRAM_ATTR ToFSensor_VL53L3CX::gpio_isr_handler(void *arg)
{
    auto *self = static_cast<ToFSensor_VL53L3CX *>(arg);
    if (self && self->gpio_evt_queue_)
    {
        xQueueSendFromISR(self->gpio_evt_queue_, &self->cfg.int_pin, NULL);
    }
}

void ToFSensor_VL53L3CX::isr_task_entry(void *arg)
{
    auto *self = static_cast<ToFSensor_VL53L3CX *>(arg);
    uint32_t io_num;
    while (xQueueReceive(self->gpio_evt_queue_, &io_num, portMAX_DELAY))
    {
        self->handleInterrupt();
    }
}
