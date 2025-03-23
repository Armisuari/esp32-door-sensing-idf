#pragma once

#include "../include/interfaces/LightSensorInterface.h"

class DoorSensingApp
{
    public:
        DoorSensingApp(LightSensorInterface &lightSensor);
        void init();
        void run();

    private:
        LightSensorInterface &_lightSensor;
};