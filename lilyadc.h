#include "hal.h"
#include "ch.h"
#include "debug.h"
#include <hal_rtc.h>

#define ADC_UPDATE_VDDA

#define VREFINT_CAL_VALUE (*(volatile uint16_t *)0x1FF80078)
#define VTSCAL2_CAL_VALUE (*(volatile uint16_t *)0x1FF8007E)

// 0xFFFF * 1.61mV/ (C)
#define TS_CNT_SLOPE 6593

struct AdcGrp1Result {
    uint32_t vsns;
    uint32_t vdda;
};

AdcGrp1Result adc_convert_grp1();

void adc_convert_grp2();

uint32_t adc_grp2_ichan(int channel);
