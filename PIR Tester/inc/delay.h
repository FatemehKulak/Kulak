
#ifndef DELAY_H_
#define DELAY_H_

#include <stdio.h>
#include <stdint.h>

uint8_t delay_ms(uint64_t _ts, uint32_t _ms);
uint8_t delay_s(uint64_t _ts, uint32_t _s);
uint8_t delay_m(uint64_t _ts, uint32_t _m);
uint64_t get_timestamp();
uint64_t system_clock();
#endif /* DELAY_H_ */
