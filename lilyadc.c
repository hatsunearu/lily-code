#include "lilyadc.h"

// // sample channel 0 only for sensing vsense
static const ADCConversionGroup adc_grp_vsense = {
  FALSE,
  ADC_GRP_VSENSE_NUM_CHANNELS,
  NULL,
  NULL, // adcerrorcallback,
  // CFGR1
  ADC_CFGR1_RES_12BIT | ADC_CFGR1_CONT,
  // TR
  ADC_TR(0,0),
  // SMPR
  ADC_SMPR_SMP_1P5,
  // CHSELR
  ADC_CHSELR_CHSEL0
};


void adc_convert_vsense() {

    palSetPad(GPIOA, GPIOA_EN_VDIV);
    palSetPad(GPIOB, GPIOB_LED_ORANGE);
    chThdSleepMilliseconds(10);
    adcStart(&ADCD1, NULL);
    adcConvert(&ADCD1, &adc_grp_vsense, adc_samples, 1);
    palClearPad(GPIOA, GPIOA_EN_VDIV);
    palClearPad(GPIOB, GPIOB_LED_ORANGE);
    adcStop(&ADCD1);
}
