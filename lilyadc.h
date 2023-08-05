#include "hal.h"
#include "ch.h"
#include <hal_rtc.h>



#define ADC_GRP_VSENSE_NUM_CHANNELS 1

#define VREFINT_CAL_VALUE (*(volatile uint16_t *)0x1FF80078)

static const ADCConversionGroup adc_grp_vsense;

// get vpp only
uint32_t adc_convert_vsense(void);
