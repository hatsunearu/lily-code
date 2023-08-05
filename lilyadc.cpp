#include "lilyadc.h"

adcsample_t adc_samples[4];

// // sample channel 0 only for sensing vsense
static const ADCConversionGroup adc_grp_vsense = {
  FALSE,
  3,
  NULL,
  NULL, // adcerrorcallback,
  // CFGR1
  ADC_CFGR1_RES_12BIT | ADC_CFGR1_CONT,
  // TR
  ADC_TR(0,0),
  // SMPR
  ADC_SMPR_SMP_1P5,
  // CHSELR
  ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL17 | ADC_CHSELR_CHSEL18
};


uint32_t adc_convert_vsense() {
    palSetPad(GPIOA, GPIOA_EN_VDIV);
    chThdSleepMilliseconds(10);
    adcStart(&ADCD1, NULL);
    adcSTM32EnableVREF(&ADCD1);
    adcConvert(&ADCD1, &adc_grp_vsense, adc_samples, 3);
    palClearPad(GPIOA, GPIOA_EN_VDIV);
    adcSTM32DisableVREF(&ADCD1);
    adcStop(&ADCD1);

    return adc_samples[0] * 3300 / 0xFFF;
}
