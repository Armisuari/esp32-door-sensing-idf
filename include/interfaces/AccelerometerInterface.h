#pragma once

class AccelerometerInterface
{
    public:
        virtual void init(void (*isr_handler)(void *)) = 0;
        virtual bool motionDetected() = 0;
};