#include "hal.h"
#include "ch.h"
#include <hal_rtc.h>



#define ADC_GRP_VSENSE_NUM_CHANNELS 1

static adcsample_t adc_samples[4];
//static const ADCConversionGroup adc_grp_vsense;

// get vpp only
// void adc_convert_vsense(void);
