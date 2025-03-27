#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_task_wdt.h"
#include "esp_sleep.h"

#include "DoorSensingApp.h"
#include "LightSensor_OPT4003.h"
#include "Accelerometer_LIS2DW12.h"
#include "ToFSensor_VL53L3CX.h"

static const char *TAG = "main";

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_TIMEOUT_MS 1000

esp_err_t i2c_master_init(void)
{
    i2c_config_t conf;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

LightSensor_OPT4003 lightSensor;
Accelerometer_LIS2DW12 accelerometer;
ToFSensor_VL53L3CX tofSensor;

// Door sensing application
DoorSensingApp doorSensingApp(lightSensor, accelerometer, tofSensor);

extern "C" void app_main()
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized");

    // Initialize the door sensing application
    doorSensingApp.init();

    // Run the door sensing application
    doorSensingApp.run();
}