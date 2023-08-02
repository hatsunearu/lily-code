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

#define GPIOB_EN_RLY1 0U

/*
 * Thread 1.
 */
THD_WORKING_AREA(waThread1, 256);
THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palSetPad(GPIOB, GPIOB_EN_RLY1);
    chThdSleepMilliseconds(1000);
  }
}

// /*
//  * Thread 2.
//  */
// THD_WORKING_AREA(waThread2, 64);
// THD_FUNCTION(Thread2, arg) {

//   (void)arg;

//   while (true) {
//     palSetPad(GPIOB, GPIOB_EN_RLY2);
//     chThdSleepMilliseconds(500);
//     palClearPad(GPIOB, GPIOB_EN_RLY2);
//     chThdSleepMilliseconds(500);
//   }
// }




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

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop. Note that
     this tasks runs at the lowest priority level so any instruction added
     here will be executed after all other tasks have been started.*/
  while (true) {
  }
}
