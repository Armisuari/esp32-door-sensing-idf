// ESP32-compatible VL53LX platform header file

#ifndef VL53LX_PLATFORM_H_
#define VL53LX_PLATFORM_H_

#include <stdint.h>
#include "vl53lx_def.h"
#include "vl53lx_platform_user_data.h"

#ifdef __cplusplus
extern "C" {
#endif

VL53LX_Error VL53LX_WriteMulti(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *pdata, uint32_t count);
VL53LX_Error VL53LX_ReadMulti(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *pdata, uint32_t count);

VL53LX_Error VL53LX_WrByte(VL53LX_Dev_t *pdev, uint16_t index, uint8_t data);
VL53LX_Error VL53LX_WrWord(VL53LX_Dev_t *pdev, uint16_t index, uint16_t data);
VL53LX_Error VL53LX_WrDWord(VL53LX_Dev_t *pdev, uint16_t index, uint32_t data);

VL53LX_Error VL53LX_RdByte(VL53LX_Dev_t *pdev, uint16_t index, uint8_t *data);
VL53LX_Error VL53LX_RdWord(VL53LX_Dev_t *pdev, uint16_t index, uint16_t *data);
VL53LX_Error VL53LX_RdDWord(VL53LX_Dev_t *pdev, uint16_t index, uint32_t *data);

VL53LX_Error VL53LX_WaitMs(VL53LX_Dev_t *pdev, int32_t wait_ms);
VL53LX_Error VL53LX_WaitUs(VL53LX_Dev_t *pdev, int32_t wait_us);
VL53LX_Error VL53LX_GetTickCount(VL53LX_Dev_t *pdev, uint32_t *ptick_count_ms);

#ifdef __cplusplus
}
#endif

#endif /* VL53LX_PLATFORM_H_ */
