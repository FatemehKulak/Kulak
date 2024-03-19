#include <delay.h>

uint64_t _systick_ = 0;

void SysTick_Handler(void)
{
	_systick_++;
}

uint64_t system_clock(){
	return _systick_;
}

uint8_t delay_ms(uint64_t _ts, uint32_t _ms) {
	if ((get_timestamp() - _ts) >= _ms) {
		return 1;
	}
	return 0;
}
uint8_t delay_s(uint64_t _ts, uint32_t _s) {
	return delay_ms(_ts, _s * 1000);
}
uint64_t get_timestamp() {
	return _systick_;
}

uint8_t delay_m(uint64_t _ts, uint32_t _m) {
	if (delay_s(_ts, _m * 60)) {
		return 1;
	}
	return 0;
}

