#pragma once

class ToFSensorInterface
{
    public:
        virtual void powerOn() = 0;
        virtual void powerOff() = 0;
        virtual bool init() = 0;
        virtual int getDistance() = 0;
};