#pragma once

class AccelerometerInterface
{
    public:
        virtual void init() = 0;
        virtual bool motionDetected() = 0;
};