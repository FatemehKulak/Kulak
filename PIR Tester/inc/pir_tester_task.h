/*
 * pir_tester_task.h
 *
 *  Created on: Jul 6, 2022
 *      Author: Tester2
 */

#ifndef PIR_TESTER_TASK_H_
#define PIR_TESTER_TASK_H_
#include <stdio.h>
typedef enum
{
	CPC_ON,
	CPC_OFF,
	NONE,
	CMD_NONE
}cp_cmd_enu;

typedef enum
{
	PIR_START_1,
	PIR_START_2,
	PIR_NTC_1,
	PIR_NTC_2,
	PIR_SENSOR_1,
	PIR_SENSOR_2,
	PIR_KEYS_STATE_1,
	PIR_KEYS_STATE_2,
	PIR_SENSOR_READY_1,
	PIR_SENSOR_READY_2,
	PIR_TEMP_READY_1,
	PIR_TEMP_READY_2,
	LAST_CMD_1,
	LAST_CMD_2,
	ALARM_ERR_1,
	ALARM_ERR_2,
	PIR_STOP_1,
	PIR_STOP_2,
	PIR_MAX
}pir_buf_enu;

typedef enum
{
	PIR_NTC,
	PIR_SENSOR,
	PIR_KEYS_STATE,
	PIR_SENSOR_READY,
	PIR_TEMP_READY,
	LAST_CMD,
	PIR_STATE_MAX
}pir_enu;
typedef enum {
		PIR_POWER_ADC,
		POWER_1_ADC,
		POWER_2_ADC,
		PIR_SENSOR_ADC,
		PIR_CAP_ADC,
		ADC_MAX
}adc_enu;

typedef struct {
	uint16_t SENSOR_OK, NTC_GND, CAP_OK;
} QCStatus_st;

void uart_transmit(uint16_t *_buf, uint16_t _len);


void tester_task();
void task_test();
void  board_switch(uint8_t N, bool_enu state);
uint8_t board_state();
bool_enu requested_start();
bool_enu requested_stop();
void task_init();
void task_dbg_mng();

#endif /* PIR_TESTER_TASK_H_ */
