/* Includes */
#include <stddef.h>
#include "stm32f10x.h"
#include <def.h>
#include <system_init.h>
#include <delay.h>
#include <pir_tester_task.h>
#include <fifo.h>


int main(void) {

	system_init();
	task_init();

	while (1) {
		task_adc_mng();
		task_dbg_mng();
		task_test();
		tester_task();
	}
}
