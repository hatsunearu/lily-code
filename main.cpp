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

#include "main.h"
#include "lilyadc.h"




// buffers
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
THD_WORKING_AREA(waThread2, 128);
THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("serial");

  while (true) {
    palSetPad(GPIOB, GPIOB_LED_ORANGE);

    uint32_t vsense = adc_convert_vsense();

    uint8_t len = int_to_str((uint32_t)(vsense), buf);
	  buf[len] = '\r';
    buf[len+1] = '\n';
    buf[len+2] = '\0';


    sdStart(&SD2, NULL);
    chThdSleepMilliseconds(10);
    chnWrite(&SD2, (uint8_t *)buf, len+2);
    chThdSleepMilliseconds(10);
    sdStop(&SD2);
    palClearPad(GPIOB, GPIOB_LED_ORANGE);
    chThdSleepMilliseconds(1000);
  }
}

uint8_t int_to_str(uint32_t result, char *str) {

	  uint8_t buf_ind = 0;
	  uint8_t digit = 0;
	  do { sdStart(&LPSD1, NULL);
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

RTCDateTime inittime;
RTCAlarm alarmspec;

void enter_stop() {
  // initialize time
  inittime.year = 0;
  inittime.month = 1;
  inittime.dstflag = 0;
  inittime.month = 1;
  inittime.day = 1;
  inittime.millisecond = 0;

  alarmspec.alrmr = RTC_ALRM_MSK4 | RTC_ALRM_MSK3 | RTC_ALRM_MSK2 
  | RTC_ALRM_ST(STOP_TIME_TENS) | RTC_ALRM_SU(STOP_TIME_ONES);

  rtcSetTime(&RTCD1, &inittime);
  rtcSetAlarm(&RTCD1, 0, &alarmspec);

  chSysLock();
  PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;
  PWR->CR |= PWR_CR_PDDS | PWR_CR_LPDS;
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __WFI();
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
  sdStart(&LPSD1, NULL);

  //adcStart(&ADCD1, NULL);

  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);


  while (true) {
    
    palSetPad(GPIOB, GPIOB_LED_YELLOW);
    chThdSleepMilliseconds(1000);
    palClearPad(GPIOB, GPIOB_LED_YELLOW);
    chThdSleepMilliseconds(1000);

    //enter_stop();
  }
}
