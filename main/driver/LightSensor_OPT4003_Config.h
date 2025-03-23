#pragma once

#include <sdkconfig.h>

#ifndef CONFIG_OPT4003_LOG_TAG
#define CONFIG_OPT4003_LOG_TAG                    "OPT4003"
#endif /* CONFIG_OPT4003_LOG_TAG */
#define LIGHT_SENSOR_TAG CONFIG_OPT4003_LOG_TAG

#ifndef CONFIG_OPT4003_ADDR
#define CONFIG_OPT4003_ADDR                       (0x44)
#endif /* CONFIG_OPT4003_ADDR */

#ifndef CONFIG_OPT4003_REG_RESULT
#define CONFIG_OPT4003_REG_RESULT                 (0x00)
#endif /* CONFIG_OPT4003_REG_RESULT */

#ifndef CONFIG_OPT4003_REG_CONFIG
#define CONFIG_OPT4003_REG_CONFIG                 (0x01)
#endif /* CONFIG_OPT4003_REG_CONFIG */

#ifndef CONFIG_OPT4003_CONFIG_START
#define CONFIG_OPT4003_CONFIG_START               (0xC810)  // Continuous conversion, automatic full-scale
#endif /* CONFIG_OPT4003_CONFIG_START */

#ifndef CONFIG_OPT4003_I2C_PORT
#define CONFIG_OPT4003_I2C_PORT                   (I2C_NUM_0)   // I2C port number
#endif /* CONFIG_OPT4003_I2C_PORT */