#pragma once

#include <sdkconfig.h>

#ifndef CONFIG_OPT4003_LOG_TAG
#define CONFIG_OPT4003_LOG_TAG                    "OPT4003"
#endif /* CONFIG_OPT4003_LOG_TAG */
#define LIGHT_SENSOR_TAG CONFIG_OPT4003_LOG_TAG

#ifndef CONFIG_OPT4003_ADDR
#define CONFIG_OPT4003_ADDR                       (0x44)
#endif /* CONFIG_OPT4003_ADDR */

#ifndef CONFIG_OPT4003_REG_RESULT_MSB_CH0
#define CONFIG_OPT4003_REG_RESULT_MSB_CH0         (0x00)
#endif /* CONFIG_OPT4003_REG_RESULT_MSB_CH0 */

#ifndef CONFIG_OPT4003_REG_RESULT_LSB_CH0
#define CONFIG_OPT4003_REG_RESULT_LSB_CH0         (0x01)
#endif /* CONFIG_OPT4003_REG_RESULT_LSB_CH0 */

#ifndef CONFIG_OPT4003_REG_CONFIG
#define CONFIG_OPT4003_REG_CONFIG                 (0x0A)
#endif /* CONFIG_OPT4003_REG_CONFIG */

#ifndef CONFIG_OPT4003_I2C_PORT
#define CONFIG_OPT4003_I2C_PORT                   (I2C_NUM_0)   // I2C port number
#endif /* CONFIG_OPT4003_I2C_PORT */

#ifndef CONFIG_OPT4003_INT_PIN
#define CONFIG_OPT4003_INT_PIN                    (GPIO_NUM_5) // GPIO pin number for interrupt
#endif /* CONFIG_OPT4003_INT_PIN */

class OPT4003Config {
    public:
        enum ConversionTime {
            CT_800US = 0b000,
            CT_1_6MS = 0b100,
            CT_3_2MS = 0b101,
            CT_6_4MS = 0b111
        };
    
        enum RangeSetting {
            RANGE_AUTO = 0b000,
            RANGE_1 = 0b001,
            RANGE_2 = 0b010,
            RANGE_3 = 0b011
        };
    
        OPT4003Config();
    
        OPT4003Config& setReset(bool enable = true);
        OPT4003Config& setContinuousMode(bool enable = true);
        OPT4003Config& setConversionTime(ConversionTime ct);
        OPT4003Config& setAutoRange();
        OPT4003Config& setRange(RangeSetting range);
    
        uint16_t build() const;
    
    private:
        uint16_t configReg;
};