#include "lilyutils.h"


#ifdef VCP_SERIAL
uint8_t len = 0;
char buf[16] = {0};
#endif

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

RTCDateTime inittime;
RTCAlarm alarmspec;

void enter_stop() {
  #ifdef VCP_SERIAL
  while (!oqIsEmptyI(&SD2.oqueue)) chThdSleepMilliseconds(1);
  sdStop(&SD2);
  #endif

  palClearPad(GPIOB, GPIOB_EN_RLY1);
  palClearPad(GPIOB, GPIOB_EN_RLY2);
  palClearPad(GPIOB, GPIOB_EN_RLY3);
  palClearPad(GPIOB, GPIOB_EN_RLY4);

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

  palClearPad(GPIOB, GPIOB_LED_YELLOW);
  __WFI();
}