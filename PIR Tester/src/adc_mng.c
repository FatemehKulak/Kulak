
#include <adc_mng.h>

static uint8_t _adc_mng_channels_[16];
static uint8_t _adc_mng_channel_cntr_ = 0;
static uint16_t _adc_values_[16];

uint8_t adc_add_channel(uint8_t _channel) {
	_adc_mng_channels_[_adc_mng_channel_cntr_++] = _channel;
	return (_adc_mng_channel_cntr_ - 1);
}

void adc_mng_init() {
	_adc_mng_channel_cntr_ = 0;
}

uint16_t adc_mng_value(uint8_t _channel) {
	return _adc_values_[_channel];
}


void task_adc_mng() {
	static uint8_t st = 0;
	static uint32_t values[16];
	static uint8_t smp_cntr = 0;
	static uint8_t ch_cntr = 0;

	if (_adc_mng_channel_cntr_ == 0) {
		return;
	}

	if (st == 0) {

		for (ch_cntr = 0; ch_cntr < _adc_mng_channel_cntr_; ch_cntr++) {
			values[ch_cntr] = 0;
		}
		ch_cntr = 0;
		smp_cntr = 0;

		adc_set_channel(ADC1, _adc_mng_channels_[ch_cntr]);
		adc_start(ADC1);
		st = 1;
	} else if (st == 1) {

		if (adc_ready(ADC1)) {
			values[ch_cntr] += adc_value(ADC1);
			ch_cntr++;
			if (ch_cntr >= _adc_mng_channel_cntr_) {
				ch_cntr = 0;
				smp_cntr++;

				if (smp_cntr >= 50) {
					for (uint8_t s = 0; s < _adc_mng_channel_cntr_; s++) {
						values[s] /= 50;
						_adc_values_[s] = values[s];
						values[s] = 0;
					}
					smp_cntr = 0;
				}
			}
			adc_set_channel(ADC1, _adc_mng_channels_[ch_cntr]);
			adc_start(ADC1);
		}
	}
}
