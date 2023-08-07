#include "hal.h"
#include "ch.h"
#include "debug.h"


#define VDDA_LOW_LIM 3250
#define VDDA_HIGH_LIM 3350
#define VDDA_HYST_LIM 25

#define VDDA_FAULT_THRES 3

#define VSNS_LOW_LIM 1325
#define VSNS_HIGH_LIM 1600
#define VSNS_HYST_LIM 25

#define VSNS_FAULT_THRES 10




extern adcsample_t adc_samples[4];
#ifdef VCP_SERIAL
extern uint8_t len;
extern char buf[16];
#endif

