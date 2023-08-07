#include "lilyadc.h"

adcsample_t adc_samples[4];
constexpr uint32_t vdda = 3300;

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


AdcGrp1Result adc_convert_grp1() {
  adcsample_t samples[2];

  palSetPad(GPIOA, GPIOA_EN_VDIV);
  chThdSleepMicroseconds(100);
  adcStart(&ADCD1, nullptr);
  chThdSleepMicroseconds(10);
  adcSTM32EnableVREF(&ADCD1);
  adcConvert(&ADCD1, &adc_grp_1, samples, 1);
  palClearPad(GPIOA, GPIOA_EN_VDIV);
  adcSTM32DisableVREF(&ADCD1);
  adcStop(&ADCD1);

  return {
    .vsns = samples[0] * vdda / 0xFFF,
    .vdda = VREFINT_CAL_VALUE * 3000 / samples[1]
  };
}

AdcGrp2Result adc_convert_grp2() {
  adcStart(&ADCD1, NULL);
  chThdSleepMicroseconds(10);
  adcConvert(&ADCD1, &adc_grp_2, adc_samples, 1);
  adcStop(&ADCD1);

  return {
    .ch1_i = vdda * adc_samples[0] / 0x7FF,
    .ch2_i = vdda * adc_samples[1] / 0x7FF,
    .ch3_i = vdda * adc_samples[2] / 0x7FF,
    .ch4_i = vdda * adc_samples[3] / 0x7FF,
  };
}