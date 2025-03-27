#pragma once

#include <sdkconfig.h>

#ifndef CONFIG_VL53L3CX_LOG_TAG
#define CONFIG_VL53L3CX_LOG_TAG                         "VL53L3CX"
#endif /* CONFIG_VL53L3CX_LOG_TAG */
#define TOF_SENSOR_TAG CONFIG_VL53L3CX_LOG_TAG

#ifndef CONFIG_VL53L3CX_ADDR_BASE
#define CONFIG_VL53L3CX_ADDR_BASE                       (0x52)
#endif /* CONFIG_VL53L3CX_ADDR_BASE */

#ifndef CONFIG_VL53L3CX_XSHUT_PIN
#define CONFIG_VL53L3CX_XSHUT_PIN                       ((gpio_num_t)18)
#endif /* CONFIG_VL53L3CX_XSHUT_PIN */

#ifndef CONFIG_VL53L3CX_INT_PIN
#define CONFIG_VL53L3CX_INT_PIN                         ((gpio_num_t)19)
#endif /* CONFIG_VL53L3CX_INT_PIN */

#ifndef CONFIG_VL53L3CX_I2C_PORT
#define CONFIG_VL53L3CX_I2C_PORT                        (I2C_NUM_0)   // I2C port number
#endif /* CONFIG_OPT4003_I2C_PORT */

class VL53L3CXConfig {
    public:
        uint8_t i2c_addr;
        gpio_num_t xshut_pin;
        gpio_num_t int_pin;
    
        VL53L3CXConfig(uint8_t addr = CONFIG_VL53L3CX_ADDR_BASE, gpio_num_t xshut = CONFIG_VL53L3CX_XSHUT_PIN, gpio_num_t interrupt = CONFIG_VL53L3CX_INT_PIN)
            : i2c_addr(addr), xshut_pin(xshut), int_pin(interrupt) {}
};