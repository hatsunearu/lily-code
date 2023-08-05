#include "hal.h"
#include "ch.h"
#include <hal_rtc.h>

#include "lilyadc.h"

#define STOP_TIME 10


#define STOP_TIME_TENS (STOP_TIME / 10)
#define STOP_TIME_ONES (STOP_TIME % 10)



uint8_t int_to_str(uint32_t, char *);
void enter_stop(void);