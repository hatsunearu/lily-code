#include "lilyadc.h"

adcsample_t adc_samples[4];
uint32_t vdda = 3300;

// vsense and vref conversion
static const ADCConversionGroup adc_grp_1 = {
  FALSE,
  2,
  NULL,
  NULL, // adcerrorcallback,
  // CFGR1
  ADC_CFGR1_RES_12BIT | ADC_CFGR1_CONT,
  // TR
  ADC_TR(0,0),
  // SMPR
  ADC_SMPR_SMP_1P5,
  // CHSELR
  ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL17
};

static const ADCConversionGroup adc_grp_2 = {
  FALSE,
  4,
  NULL,
  NULL, // adcerrorcallback,
  // CFGR1
  ADC_CFGR1_RES_12BIT | ADC_CFGR1_CONT,
  // TR
  ADC_TR(0,0),
  // SMPR
  ADC_SMPR_SMP_1P5,
  // CHSELR
  ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL2 | ADC_CHSELR_CHSEL5 | ADC_CHSELR_CHSEL6
};


void adc_convert_grp1() {
  palSetPad(GPIOA, GPIOA_EN_VDIV);
  chThdSleepMicroseconds(100);
  adcStart(&ADCD1, NULL);
  chThdSleepMicroseconds(10);
  adcSTM32EnableVREF(&ADCD1);
  adcConvert(&ADCD1, &adc_grp_1, adc_samples, 1);
  palClearPad(GPIOA, GPIOA_EN_VDIV);
  adcSTM32DisableVREF(&ADCD1);
  adcStop(&ADCD1);
#ifdef ADC_UPDATE_VDDA
  vdda = adc_grp1_vdda();
#endif
}

uint32_t adc_grp1_vsns() {
  return adc_samples[0] * vdda / 0xFFF;
}

uint32_t adc_grp1_vdda() {
  return VREFINT_CAL_VALUE * 3000 / adc_samples[1];
}

// EN_4V5_REG must be enabled
void adc_convert_grp2() {
  adcStart(&ADCD1, NULL);
  chThdSleepMicroseconds(10);
  adcConvert(&ADCD1, &adc_grp_2, adc_samples, 1);
  adcStop(&ADCD1);
}

uint32_t adc_grp2_ichan(int channel) {
  return vdda * adc_samples[channel] / 0x7FF;
}