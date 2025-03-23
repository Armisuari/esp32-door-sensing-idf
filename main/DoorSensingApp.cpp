#include "DoorSensingApp.h"
#include "esp_log.h"    // ESP_LOGI
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DoorSensingApp";

DoorSensingApp::DoorSensingApp(LightSensorInterface &lightSensor) : _lightSensor(lightSensor)
{
}

void DoorSensingApp::init()
{
    // Initialize the light sensor
    if (!_lightSensor.init())
    {
        ESP_LOGE(TAG, "Failed to initialize light sensor");
    }
}

void DoorSensingApp::run()
{
    // Run the door sensing application
    while (true)
    {
        // Check if light is triggered
        if (_lightSensor.lightTriggered())
        {
            ESP_LOGI(TAG, "Light is triggered");
        }
        else
        {
            ESP_LOGI(TAG, "Light is not triggered");
        }

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}