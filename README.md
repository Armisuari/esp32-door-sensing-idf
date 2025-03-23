# _ESP32 DOOR SENSING_

## Hardware Design

![alt text](ESP32-door-sensing-Hardware-Design.jpg)

[OPT4003DNPRQ1] https://www.ti.com/lit/gpn/OPT4003-Q1

## Firmware Architecture

![alt text](ESP32-door-sensing-Firmware-Architecture.jpg)

## Firmware Design

```C++

// Light Sensor
bool init();
bool lightTriggered();

// ToF Sensor
void powerOn();
void powerOff();
bool init();
int getDistance();

// Accelerometer
void init();
bool motionDetected();

```

## Program State Chart

![alt text](ESP32-door-sensing-State-Chart.jpg)
