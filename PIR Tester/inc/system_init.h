#ifndef SYSTEM_INIT_
#define SYSTEM_INIT_

#include <stdio.h>
#include <stdint.h>

//uint ntc_channel;
//uint sensor_channel;
//uint power_adc;
uint a_power_1;
uint a_power_2;
uint pir_power;
uint sensor_adc;
uint cap_adc;
void tester_adc_init();
void system_init();
void debugc(char _c);
#endif /* SYSTEM_INIT_ */
