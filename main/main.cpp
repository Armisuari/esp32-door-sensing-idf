#include <stdio.h>

#include "DoorSensingApp.h"
#include "LightSensor_OPT4003.h"

// Light sensor
LightSensor_OPT4003 lightSensor;

// Door sensing application
DoorSensingApp doorSensingApp(lightSensor);

extern "C" void app_main()
{
    // Initialize the door sensing application
    doorSensingApp.init();

    // Run the door sensing application
    doorSensingApp.run();
}