#pragma once

class LightSensorInterface
{
    public:
        virtual bool init() = 0;
        virtual bool lightTriggered() = 0;
};