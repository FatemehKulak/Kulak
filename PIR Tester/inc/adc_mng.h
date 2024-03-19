
#ifndef ADC_MNG_H_
#define ADC_MNG_H_

#include <def.h>
#include <adc.h>

uint8_t adc_add_channel(uint8_t _channel) ;
void adc_mng_init() ;
uint16_t adc_mng_value(uint8_t _channel) ;
void task_adc_mng() ;


#endif /* ADC_MNG_H_ */
