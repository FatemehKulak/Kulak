#ifndef ADC_H_
#define ADC_H_

#include <def.h>

uint8_t adc_ready(ADC_TypeDef *_adc) ;
void adc_stop(ADC_TypeDef *_adc, uint8_t _ch) ;
void adc_start(ADC_TypeDef *_adc) ;
void adc_set_channel(ADC_TypeDef *_adc, uint8_t _ch) ;
uint16_t adc_value(ADC_TypeDef *_adc) ;
void adc_init(ADC_TypeDef *_adc) ;
float adc_convert(uint16_t _adc_value) ;

#endif /* ADC_H_ */
