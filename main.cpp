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
#include "lilyutils.h"


APIParseState apistate = Init;
#define APIBUF_LEN 6
uint8_t apibuf[APIBUF_LEN] = {0};

// /*
//  * Thread 2.
//  */
THD_WORKING_AREA(waThread2, 128);
THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("serial");

  while (true) {
    if (apistate == Init) {
      palClearPad(GPIOB, GPIOB_LED_ORANGE);
      for (int i=0; i<len; i++) {
        apibuf[i] = 0;
      }
      chnRead(&SD2, apibuf, 1);

      // sync byte found, transition
      if (apibuf[0] == 0x39) {
        apistate = Sync;
      }
    }

    else if (apistate == Sync) {
      palSetPad(GPIOB, GPIOB_LED_ORANGE);
      chnRead(&SD2, apibuf+1, 1);

      switch (apibuf[1]) {
        case 0x01: // valid addresses
        case 0x02:
        case 0x03:
        case 0x04:
          apistate = Address;
          break;
        default:
          apistate = Init;
      }
    }

    else if (apistate == Address) {
      palSetPad(GPIOB, GPIOB_LED_ORANGE);
      switch (apibuf[1]) {
        case 0x01: // payload + parity is 1
        case 0x02:
          chnRead(&SD2, apibuf+2, 1);
          service_api(apibuf, APIBUF_LEN);
          break;

        case 0x03: // payload + parity is 2
          chnRead(&SD2, apibuf+2, 2);
          service_api(apibuf, APIBUF_LEN);
          break;

        case 0x04: // payload + parity is 5
          chnRead(&SD2, apibuf+2, 5);
          service_api(apibuf, APIBUF_LEN);
          break;
      }
      apistate = Init;
    }

  }
}

void service_api(uint8_t *b, int len) {

  // invalid sync
  if (b[0] != 0x39)
    return;

  uint8_t parity = 0;
  for (int i=0; i<len; i++) {
    parity ^= b[i];
  }

  // invalid payload
  if (parity != 0)
    return;

  int outlen = 0;
  if (apibuf[1] == 0x01) {
    auto adc1 = adc_convert_grp1();
    apibuf[2] = adc1.vsns & 0xFF;
    apibuf[3] = (adc1.vsns >> 8) & 0xFF;
    apibuf[4] = adc1.vdda & 0xFF;
    apibuf[5] = (adc1.vdda >> 8) & 0xFF;
    outlen = 6;
  }
  else {
    return;
  }
  
  parity = 0;
  for (int i=0; i<outlen; i++) {
    parity ^= apibuf[i];
  }
  apibuf[outlen] = parity;
  chnWrite(&SD2, apibuf, outlen);
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

  // obtain VDDA and VSNS
  #ifdef DEBUG
  palSetPad(GPIOB, GPIOB_LED_YELLOW);
  #endif
  auto adc1 = adc_convert_grp1();
  uint32_t vsns = adc1.vsns;
  uint32_t vdda = adc1.vdda;

#ifdef VCP_SERIAL
  sdStart(&SD2, NULL);
  chThdSleepMilliseconds(10);

  VCP_PRINT("\r\nReset\r\n");
  VCP_PRINT("Startup VDDA: ");

  len = int_to_str((uint32_t)(vdda), buf);
  chnWrite(&SD2, (uint8_t *)buf, len);
  VCP_PRINT("mV\r\n");
  
  VCP_PRINT("Startup VSNS: ");

  len = int_to_str((uint32_t)(vsns), buf);
  chnWrite(&SD2, (uint8_t *)buf, len);
  VCP_PRINT("mV\r\n");
#endif


  // startup gating condition
  if (vdda <= VDDA_LOW_LIM || vdda >= VDDA_HIGH_LIM) {
    enter_stop();
  }
  if (vsns <= VSNS_LOW_LIM || vsns >= VSNS_HIGH_LIM) {
    enter_stop();
  }


  #ifdef VCP_SERIAL
  chnWrite(&SD2, (const uint8_t *)"Startup Passed\r\n", 17);
  #endif

  // enable power gating
  palSetPad(GPIOA, GPIOA_EN_4V5_REG);
  // enable pi comms
  sdStart(&LPSD1, NULL);

  // enable threads
  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  uint32_t vdda_faults = 0;
  uint32_t vsns_faults = 0;


  // main thread, check for voltages and exit
  while (true) {

    // convert voltages
    #ifdef DEBUG
    palSetPad(GPIOB, GPIOB_LED_YELLOW);
    #endif    
    auto adc1 = adc_convert_grp1();
    uint32_t vsns = adc1.vsns;
    uint32_t vdda = adc1.vdda;

    // VDDA fault
    if (vdda <= VDDA_LOW_LIM - VDDA_HYST_LIM || vdda >= VDDA_HIGH_LIM + VDDA_HYST_LIM) {
      vdda_faults++;
      #ifdef VCP_SERIAL
      VCP_PRINT("Flt VDDA\r\n");
      #endif
    }
    else {
      vdda_faults=0;
    }

    // VSNS fault
    if (vsns <= VSNS_LOW_LIM - VSNS_HYST_LIM || vsns >= VSNS_HIGH_LIM + VSNS_HYST_LIM) {
      vsns_faults++;
      #ifdef VCP_SERIAL
      VCP_PRINT("Flt VSNS\r\n");
      #endif
    }
    else {
      vsns_faults=0;
    }
    
    // fault count exceeds threshold
    if (vdda_faults >= VDDA_FAULT_THRES || vsns_faults >= VSNS_FAULT_THRES) {
      palClearPad(GPIOA, GPIOA_EN_4V5_REG);
      #ifdef VCP_SERIAL
      VCP_PRINT("Stop Flt\r\n");
      #endif
      enter_stop();
    }

    palClearPad(GPIOB, GPIOB_LED_YELLOW);
    chThdSleepMilliseconds(100);
  }
}
