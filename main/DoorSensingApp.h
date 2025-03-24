#pragma once

#include "../include/interfaces/LightSensorInterface.h"
#include "../include/interfaces/AccelerometerInterface.h"

class DoorSensingApp
{
    public:
        DoorSensingApp(LightSensorInterface &lightSensor, AccelerometerInterface &accelerometer);
        void init();
        void run();

    private:
        LightSensorInterface &_lightSensor;
        AccelerometerInterface &_accelerometer;

        void checkLightSensor();
        void checkAccelerometer();
};