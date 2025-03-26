#pragma once

#include <sdkconfig.h>

#ifndef CONFIG_LIS2DW12_LOG_TAG
#define CONFIG_LIS2DW12_LOG_TAG                    "LIS2DW12"
#endif /* CONFIG_LIS2DW12_LOG_TAG */
#define ACCELEROMETER_TAG CONFIG_LIS2DW12_LOG_TAG

#ifndef CONFIG_LIS2DW12_ADDR_BASE
#define CONFIG_LIS2DW12_ADDR_BASE                  (0x18) // (default) the SA0/SDO pin is low
#endif /* CONFIG_LIS2DW12_ADDR_BASE */

#ifndef CONFIG_LIS2DW12_OUT_T_L
#define CONFIG_LIS2DW12_OUT_T_L                    (0x0D)   // Temperature sensor output register: lower part
#endif /* CONFIG_LIS2DW12_OUT_T_L */

#ifndef CONFIG_LIS2DW12_OUT_T_H                 
#define CONFIG_LIS2DW12_OUT_T_H                    (0x0E)   // Temperature sensor output register: higher part
#endif /* CONFIG_LIS2DW12_OUT_T_H */

#ifndef CONFIG_LIS2DW12_REG_WHO_AM_I
#define CONFIG_LIS2DW12_REG_WHO_AM_I               (0x0F)   // Device identification register
#endif /* CONFIG_LIS2DW12_REG_WHO_AM_I */

#ifndef CONFIG_LIS2DW12_REG_CTRL1
#define CONFIG_LIS2DW12_REG_CTRL1                  (0x20)
#endif /* CONFIG_LIS2DW12_REG_CTRL1 */

#ifndef CONFIG_LIS2DW12_REG_CTRL6
#define CONFIG_LIS2DW12_REG_CTRL6                  (0x25)
#endif /* CONFIG_LIS2DW12_REG_CTRL6 */

#ifndef CONFIG_LIS2DW12_REG_WAKE_UP_THS
#define CONFIG_LIS2DW12_REG_WAKE_UP_THS            (0x34)
#endif /* CONFIG_LIS2DW12_REG_WAKE_UP_THS */

#ifndef CONFIG_LIS2DW12_REG_WAKE_UP_DUR
#define CONFIG_LIS2DW12_REG_WAKE_UP_DUR            (0x35)
#endif /* CONFIG_LIS2DW12_REG_WAKE_UP_DUR */

#ifndef CONFIG_LIS2DW12_REG_CTRL4_INT1
#define CONFIG_LIS2DW12_REG_CTRL4_INT1             (0x23)
#endif /* CONFIG_LIS2DW12_REG_CTRL4_INT1 */

#ifndef CONFIG_LIS2DW12_REG_STATUS
#define CONFIG_LIS2DW12_REG_STATUS                 (0x27)
#endif /* CONFIG_LIS2DW12_REG_STATUS */

#ifndef CONFIG_LIS2DW12_REG_WAKE_UP_SRC
#define CONFIG_LIS2DW12_REG_WAKE_UP_SRC            (0x38)
#endif /* CONFIG_LIS2DW12_REG_WAKE_UP_SRC */

#ifndef CONFIG_I2C_PORT
#define CONFIG_I2C_PORT                            (I2C_NUM_0)   // I2C port number
#endif /* CONFIG_I2C_PORT */

#ifndef CONFIG_LIS2DW12_INT_PIN
#define CONFIG_LIS2DW12_INT_PIN                    (GPIO_NUM_4) // GPIO pin number for interrupt
#endif /* CONFIG_LIS2DW12_INT_PIN */

class LIS2DW12Config {
    public:
        uint8_t sa0 = 0;
        uint8_t odr = 0x50;
        gpio_num_t int_pin = CONFIG_LIS2DW12_INT_PIN;
        i2c_port_t i2c_port = CONFIG_I2C_PORT;
    
        uint8_t getDeviceAddress() const {
            return (CONFIG_LIS2DW12_ADDR_BASE | (sa0 & 0x01));
        }
};