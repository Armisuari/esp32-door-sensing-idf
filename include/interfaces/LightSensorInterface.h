#pragma once

class LightSensorInterface
{
    public:
        virtual bool init(void (*isr_handler)(void *)) = 0;
        virtual bool lightTriggered(const float threshold) = 0;
};