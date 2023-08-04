/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "hal.h"
#include "ch.h"
#include <string.h>

#define ADC_GRP_VSENSE_NUM_CHANNELS 1


uint8_t int_to_str(uint32_t, char *);

// sample channel 0 only for sensing vsense
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


// buffers
static adcsample_t adc_samples[5];
char buf[16] = {0};



// /*
//  * Thread 1.
//  */
// THD_WORKING_AREA(waThread1, 128);
// THD_FUNCTION(Thread1, arg) {

//   (void)arg;
//   chRegSetThreadName("blinker");

//   while (true) {


//     chThdSleepMilliseconds(1000);
//   }
// }




// /*
//  * Thread 2.
//  */
THD_WORKING_AREA(waThread2, 64);
THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("serial");

  while (true) {
    palSetPad(GPIOA, GPIOA_EN_VDIV);
    palSetPad(GPIOB, GPIOB_LED_ORANGE);
    chThdSleepMilliseconds(10);
    adcStart(&ADCD1, NULL);
    adcConvert(&ADCD1, &adc_grp_vsense, adc_samples, 1);
    palClearPad(GPIOA, GPIOA_EN_VDIV);
    palClearPad(GPIOB, GPIOB_LED_ORANGE);
    adcStop(&ADCD1);

    adc_samples[0] = 0x7FF;
    uint8_t len = int_to_str((uint32_t)(adc_samples[0] * 3330 / 0xFFF), buf);
	  // buf[len] = '\r';
    // buf[len+1] = '\n';
    // buf[len+2] = '\0';
    strcat(buf, "\r\n");

    chnWrite(&SD2, (uint8_t *)buf, 16);
    chThdSleepMilliseconds(1000);
  }
}

uint8_t int_to_str(uint32_t result, char *str) {

	  uint8_t buf_ind = 0;
	  uint8_t digit = 0;
	  do {
		  digit = result % 10;
		  result /= 10;

		  str[buf_ind] = digit + '0';
		  buf_ind++;
	  } while (result > 0);
	  str[buf_ind] = 0;

	  // reverse string
	  uint8_t temp;
	  for (int i=0; i<(buf_ind+1)/2; i++) {
		  temp = str[i];
		  str[i] = str[buf_ind-1-i];
		  str[buf_ind-1-i] = temp;
	  }

    return buf_ind;

}




/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  sdStart(&SD2, NULL);
  //adcStart(&ADCD1, NULL);

  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);


  while (true) {
    palSetPad(GPIOB, GPIOB_LED_YELLOW);
    chThdSleepMilliseconds(10);
    palClearPad(GPIOB, GPIOB_LED_YELLOW);
    chThdSleepMilliseconds(990);
  }
}
