#include "DoorSensingApp.h"
#include "esp_log.h"    // ESP_LOGI
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DoorSensingApp";

bool DoorSensingApp::motion_detected = false;
bool DoorSensingApp::light_change_detected = false;
door_state_t DoorSensingApp::current_state = STATE_IDLE;

DoorSensingApp::DoorSensingApp(LightSensorInterface &lightSensor, AccelerometerInterface &accelerometer, ToFSensorInterface &tofSensor) :
_lightSensor(lightSensor), _accelerometer(accelerometer), _tofSensor(tofSensor)
{
}

void DoorSensingApp::init()
{
    // Initialize the light sensor
    if (!_lightSensor.init(DoorSensingApp::light_sensor_isr_handler))
    {
        ESP_LOGE(TAG, "Failed to initialize light sensor");
    }

    // Initialize the accelerometer
    _accelerometer.init(DoorSensingApp::accelerometer_isr_handler);

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
        switch (current_state)
        {
            case STATE_IDLE:
                handleIdleState();
                break;
            case STATE_ENABLE_TOF:
                handleEnableToFState();
                break;
            case STATE_CHECK_DOOR_STATUS:
                handleCheckDoorStatusState();
                break;
            case STATE_TOF_OFF:
                handleToFOffState();
                break;
            default:
                break;
        }

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void IRAM_ATTR DoorSensingApp::light_sensor_isr_handler(void* arg)
{
    auto *self = static_cast<DoorSensingApp *>(arg);
    if (self->_lightSensor.lightTriggered(100))
    {
        ESP_LOGI(TAG, "Light is triggered");
        light_change_detected = true;
    }
    else
    {
        ESP_LOGI(TAG, "Light is not triggered");
        light_change_detected = false;
    }
}

void IRAM_ATTR DoorSensingApp::accelerometer_isr_handler(void* arg)
{
    auto *self = static_cast<DoorSensingApp *>(arg);
    if (self->_accelerometer.motionDetected())
    {
        ESP_LOGI(TAG, "Motion is detected");
        motion_detected = true;
    }
    else
    {
        ESP_LOGI(TAG, "Motion is not detected");
        motion_detected = false;
    }
}

void DoorSensingApp::handleIdleState()
{
    // Check if the door is opened
    if (motion_detected || light_change_detected)
    {
        ESP_LOGI(TAG, "Interrupt detected, enabling ToF...");
        motion_detected = false;
        light_change_detected = false;
        current_state = STATE_ENABLE_TOF;
    }
}

void DoorSensingApp::handleEnableToFState()
{
    // Power on the ToF sensor
    _tofSensor.powerOn();

    // Initialize the ToF sensor
    if (!_tofSensor.init())
    {
        ESP_LOGE(TAG, "Failed to initialize ToF sensor");
    }

    current_state = STATE_CHECK_DOOR_STATUS;
}

void DoorSensingApp::handleCheckDoorStatusState()
{
    int distance_mm = _tofSensor.getDistance();

    if (distance_mm < DOOR_THRESHOLD_MM) {
        ESP_LOGI(TAG, "Distance: %d mm -> Door Closed", distance_mm);
    } else {
        ESP_LOGI(TAG, "Distance: %d mm -> Door Open", distance_mm);
    }

    current_state = STATE_TOF_OFF;
}

void DoorSensingApp::handleToFOffState()
{
    // Power off the ToF sensor
    _tofSensor.powerOff();

    current_state = STATE_IDLE;
}