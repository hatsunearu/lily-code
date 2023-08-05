#include "hal.h"
#include "ch.h"
#include <hal_rtc.h>

#define VREFINT_CAL_VALUE (*(volatile uint16_t *)0x1FF80078)

// get vpp only
uint32_t adc_convert_vsense(void);
