#ifndef _FIFO_H_
#define _FIFO_H_
#include <stdint.h>
#include <def.h>
#include <string.h>

char *fifo_title(uint8_t _fifo_id);
uint8_t fifo_req(uint8_t *_ptr, uint16_t _size, uint16_t _max_elem_aval, const char *_f_title_);
uint16_t fifo_size(uint8_t _fifo_id);
uint8_t fifo_push(uint8_t _fifo_id, uint8_t *_data);
uint8_t fifo_full(uint8_t _fifo_id);
uint8_t fifo_empty(uint8_t _fifo_id);
uint8_t *fifo_pop(uint8_t _fifo_id);
uint8_t *fifo_pop_stay(uint8_t _fifo_id);
void fifo_free_elem(uint8_t _fifo_id);
void fifo_clear(uint8_t _fifo_id);
uint8_t fifo_aval();
void fifo_set_front_rear(uint8_t _fifo_id, uint16_t _front, uint16_t _rear);

uint16_t fifo_get_rear(uint8_t _fifo_id);
uint16_t fifo_get_front(uint8_t _fifo_id);
#endif
