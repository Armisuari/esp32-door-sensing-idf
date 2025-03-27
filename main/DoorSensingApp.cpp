#include "DoorSensingApp.h"
#include "esp_log.h"    // ESP_LOGI
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DoorSensingApp";

DoorSensingApp::DoorSensingApp(LightSensorInterface &lightSensor, AccelerometerInterface &accelerometer, ToFSensorInterface &tofSensor) :
_lightSensor(lightSensor), _accelerometer(accelerometer), _tofSensor(tofSensor)
{
}

void DoorSensingApp::init()
{
    // Initialize the light sensor
    if (!_lightSensor.init())
    {
        ESP_LOGE(TAG, "Failed to initialize light sensor");
    }

    // Initialize the accelerometer
    _accelerometer.init();

    // Initialize the ToF sensor
    _tofSensor.powerOn();
    if (!_tofSensor.init())
    {
        ESP_LOGE(TAG, "Failed to initialize ToF sensor");
    }
}

void DoorSensingApp::run()
{
    // Run the door sensing application
    while (true)
    {
        checkLightSensor();
        checkAccelerometer();
        checkToFSensor();

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void DoorSensingApp::checkLightSensor()
{
    if (_lightSensor.lightTriggered(100))
    {
        ESP_LOGI(TAG, "Light is triggered");
    }
    else
    {
        ESP_LOGI(TAG, "Light is not triggered");
    }
}

void DoorSensingApp::checkAccelerometer()
{
    if (_accelerometer.motionDetected())
    {
        ESP_LOGI(TAG, "Motion is detected");
    }
    else
    {
        ESP_LOGI(TAG, "Motion is not detected");
    }
}

void DoorSensingApp::checkToFSensor()
{
    if (_tofSensor.getDistance() < 100)
    {
        ESP_LOGI(TAG, "Object is detected");
    }
    else
    {
        ESP_LOGI(TAG, "Object is not detected");
    }
}