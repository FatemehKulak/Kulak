

#include <adc.h>

uint8_t adc_ready(ADC_TypeDef *_adc) {
	if (_adc->SR & ADC_SR_EOC) {
		return 1;
	}
	return 0;
}

void adc_stop(ADC_TypeDef *_adc, uint8_t _ch) {
	_adc->SQR3 &= ~(_ch << 0);
	_adc->CR2 &= ~(_ch << 30);
}

void adc_start(ADC_TypeDef *_adc) {
	_adc->CR2 |= ADC_CR2_SWSTART;
}

void adc_set_channel(ADC_TypeDef *_adc, uint8_t _ch) {
	_adc->SQR3 = (_ch << 0);
}

uint16_t adc_value(ADC_TypeDef *_adc) {
	return _adc->DR & 0xFFF;
}

void adc_init(ADC_TypeDef *_adc) {
	_adc->CR2 |= ADC_CR2_ADON;
	_adc->CR2 |= ADC_CR2_EXTTRIG;
	_adc->CR2 |= ADC_CR2_EXTSEL;

	_adc->CR2 |= ADC_CR2_RSTCAL;
	_adc->CR1 |= ADC_CR1_EOCIE;


	while (_adc->CR2 & (1 << 3));
	_adc->CR2 |= ADC_CR2_CAL;
	while (_adc->CR2 & (1 << 2)); /* Wait for calibration to finish   */

}

float adc_convert(uint16_t _adc_value) {
	//return ( _adc_value );
	return ((float) _adc_value * 0.805664);
}

