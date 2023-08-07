#include "hal.h"
#include "ch.h"
#include "debug.h"
#include <hal_rtc.h>

#define ADC_UPDATE_VDDA

#define VREFINT_CAL_VALUE (*(volatile uint16_t *)0x1FF80078)
#define VTSCAL2_CAL_VALUE (*(volatile uint16_t *)0x1FF8007E)

// 0xFFFF * 1.61mV/ (C)
#define TS_CNT_SLOPE 6593



// get vpp only
void adc_convert_grp1(void);

uint32_t adc_grp1_vsns(void);

uint32_t adc_grp1_vdda(void);

void adc_convert_grp2(void);

uint32_t adc_grp2_ichan(int channel);
