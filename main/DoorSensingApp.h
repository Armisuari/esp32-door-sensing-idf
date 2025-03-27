#pragma once

#include "../include/interfaces/LightSensorInterface.h"
#include "../include/interfaces/AccelerometerInterface.h"
#include "../include/interfaces/ToFSensorInterface.h"

class DoorSensingApp
{
    public:
        DoorSensingApp(LightSensorInterface &lightSensor, AccelerometerInterface &accelerometer, ToFSensorInterface &tofSensor);
        void init();
        void run();

    private:
        LightSensorInterface &_lightSensor;
        AccelerometerInterface &_accelerometer;
        ToFSensorInterface &_tofSensor;

        void checkLightSensor();
        void checkAccelerometer();
        void checkToFSensor();
};