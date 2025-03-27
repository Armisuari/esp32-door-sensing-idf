#pragma once

#include <inttypes.h>

class ToFSensorInterface
{
    public:
        virtual void powerOn() = 0;
        virtual void powerOff() = 0;
        virtual bool init() = 0;
        virtual uint16_t getDistance() = 0;
};