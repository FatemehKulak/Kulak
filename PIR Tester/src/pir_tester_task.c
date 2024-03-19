#include <stddef.h>
#include <stdint.h>
#include <def.h>
#include <system_init.h>
#include <delay.h>
#include <stdio.h>
#include <pir_tester_task.h>
#include <fifo.h>

uint16_t ncPinArray[] = {M_NC_1_PIN, M_NC_2_PIN, M_NC_3_PIN, M_NC_4_PIN, M_NC_5_PIN, M_NC_6_PIN, M_NC_7_PIN, M_NC_8_PIN};
GPIO_TypeDef * ncPortArray[] = {M_NC_1_PORT, M_NC_2_PORT, M_NC_3_PORT, M_NC_4_PORT, M_NC_5_PORT, M_NC_6_PORT, M_NC_7_PORT, M_NC_8_PORT};

static uint16_t PirHealth[PIR_MAX];
static uint8_t last_board = 0;
static uint8_t RXBuffer[20];
static uint8_t RXBufCntr = 0;
static uint8_t board_st = 0;
static uint64_t RXBufTs = 0;
static uint64_t RXUSART3Ts = 0;
static uint16_t CMD;


extern cp_cmd_enu cmd_aval();

static bool_enu connection_flag = false;


char DBGBUF[100];
uint8_t DBGBUF_CNTR = 0;
bool_enu DBGBUF_RDY = false;


static bool_enu req_start = false , req_stop = false;

static uint8_t command_fifo_id = 0;
uint8_t command_fifo[11];
const char CMD_TITLE[][20] = {
		"CPC_ON",
		"CPC_OFF",
		"NONE"
};



void USART3_IRQHandler(void)
{
//	uint16_t data = USART_ReceiveData(USART1);
//	printf("%d\r\n",data);
//	USART_SendData(USART1,data);
//	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	uint16_t data = USART_ReceiveData(USART3);
	if (delay_ms(RXBufTs,100))
	{
		RXBufCntr = 0;
	}
	RXBuffer[RXBufCntr++] = data;
	RXBufTs = system_clock();
}

void USART1_IRQHandler(void)
{
	char c = USART1->DR;
//	USART_SendData(USART3, c);
//	while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
	if (delay_ms(RXUSART3Ts,100))
		DBGBUF_CNTR = 0;
	if (c == '\n' || c == '\r') {
		DBGBUF_RDY = true;
		DBGBUF_CNTR = 0;
	} else {
		DBGBUF[DBGBUF_CNTR++] = c;
		DBGBUF[DBGBUF_CNTR] = '\0';
	}
	if (DBGBUF_CNTR >= 99) {
		DBGBUF_CNTR = 0;
	}
	RXUSART3Ts = system_clock();
	USART1->SR;
}

bool_enu requested_start() {
	if (req_start) {
		req_start = false;
		return true;
	}
	return false;
}

bool_enu requested_stop() {
	if (req_stop) {
		req_stop = false;
		return true;
	}
	return false;
}


void uart_transmit(uint16_t *_buf, uint16_t _len) {
	for (int i = 0; i < _len; i++) {
		USART_SendData(USART3,_buf[i]);
		while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
	}
}

void  board_switch(uint8_t N, bool_enu state) {

	if(state == ON){
		board_st = N;
		if (N == 1) {
			BOARD_1(ON);
			MUX_0(OFF);
			MUX_1(OFF);
			MUX_2(OFF); //000
		} else if (N == 2) {
			BOARD_2(ON);
			MUX_0(ON);
			MUX_1(OFF);
			MUX_2(OFF); //001
		} else if (N == 3) {
			BOARD_3(ON);
			MUX_0(OFF);
			MUX_1(ON);
			MUX_2(OFF); //010
		} else if (N == 4) {
			BOARD_4(ON);
			MUX_0(ON);
			MUX_1(ON);
			MUX_2(OFF); //011
		} else if (N == 5){
			BOARD_5(ON);
			MUX_0(OFF);
			MUX_1(OFF);
			MUX_2(ON); //100
		} else if (N == 6) {
			BOARD_6(ON);
			MUX_0(ON);
			MUX_1(OFF);
			MUX_2(ON); //101
		}	else if (N == 7) {
			BOARD_7(ON);
			MUX_0(OFF);
			MUX_1(ON);
			MUX_2(ON); //110
		} else if(N == 8) {
			BOARD_8(ON);
			MUX_0(ON);
			MUX_1(ON);
			MUX_2(ON); //111
		}
	} else {
		board_st = 0;
		if (N == 1){
			BOARD_1(OFF);
		} else if(N == 2) {
			BOARD_2(OFF);
		} else if (N == 3) {
			BOARD_3(OFF);
		} else if (N == 4){
			BOARD_4(OFF);
		} else if(N == 5) {
			BOARD_5(OFF);
		} else if(N == 6) {
			BOARD_6(OFF);
		} else if (N == 7) {
			BOARD_7(OFF);
		} else if (N == 8) {
			BOARD_8(OFF);
		}
	}
}

uint8_t board_state() {
	return board_st;
}


void tester_task(){

	static uint16_t CmdBuf[10];
	CmdBuf[0] = 0xAA;
	CmdBuf[2] = 0x55;
	TASK_MAKE(false);
	if (st == 0) {
		st = 1;
	} else if (st == 1) {
		if (delay_ms(ts, 50) && (cmd_aval() !=  CMD_NONE)) {
			CMD = cmd_aval();
//			printf("CMD = %s\r\n",CMD_TITLE[CMD]);
			CmdBuf[1] = CMD;
			uart_transmit(CmdBuf,3);
			TASK_STEP(2);
		}
	} else if(st == 2) {
		if( RXBufCntr >= PIR_MAX && delay_ms(RXBufTs, 5)) {
			RXBufCntr = 0;
			TASK_STEP(0);
			int i;
			if(RXBuffer[PIR_START_1] == 0xAA && RXBuffer[PIR_START_2] == 0xAA && RXBuffer[PIR_STOP_1] == 0x55 && RXBuffer[PIR_STOP_2] == 0x55)
			{
				for(i = 0; i < PIR_MAX;i++){
					PirHealth[i] = 0;
				}
				PirHealth[LAST_CMD] |= RXBuffer[LAST_CMD_2];
				PirHealth[LAST_CMD] |= (RXBuffer[LAST_CMD_1] << 8);
//				printf("LAST CMD =  %s\r\n",CMD_TITLE[PirHealth[LAST_CMD]]);
				if (PirHealth[LAST_CMD] == NONE){
					PirHealth[PIR_KEYS_STATE] |= RXBuffer[PIR_KEYS_STATE_2];
					PirHealth[PIR_KEYS_STATE] |= (RXBuffer[PIR_KEYS_STATE_1] << 8) ;
					printf("PIR KEY STATE IS %d\r\n",PirHealth[PIR_KEYS_STATE]);
					if(RXBuffer[PIR_TEMP_READY_2] == 1){
						PirHealth[PIR_TEMP_READY] = 1;
						PirHealth[PIR_NTC] |= RXBuffer[PIR_NTC_2];
						PirHealth[PIR_NTC] |= (RXBuffer[PIR_NTC_1] << 8) ;
						printf("NTC VALUE IS: %d\r\n",PirHealth[PIR_NTC]);
					} else {
						printf("NTC IS NOT READY\r\n");
					}
					if(RXBuffer[PIR_SENSOR_READY_2] == 1){
						PirHealth[PIR_SENSOR_READY] = 1;
						PirHealth[PIR_SENSOR] |= RXBuffer[PIR_SENSOR_2];
						PirHealth[PIR_SENSOR] |= (RXBuffer[PIR_SENSOR_1] << 8);
						printf("SENSOR ADC VALUE IS %d\r\n",(uint16_t)( PirHealth[PIR_SENSOR] * 1.22));
					}
					if (RXBuffer[ALARM_ERR_2] == 1) {
						printf("ALARM-ERR\r\n");
					}
						/* else {
					printf("SENSOR IS NOT READY\r\n");
					}*/
				}
				if(CMD == PirHealth[LAST_CMD]){
					 fifo_pop(command_fifo_id);
//					 printf("%s CMD POPED.\r\n",CMD_TITLE[PirHealth[LAST_CMD]]);
				}
			}
			if (delay_s(ts, 5)) {
				st = 0;
				printf("CONNECTION WITH PIR FAILD\r\n");
				fifo_pop(command_fifo_id);
			}
		}
	}
}




cp_cmd_enu cmd_aval() {
	uint8_t *ptr;
	if (!fifo_empty(command_fifo_id)) {
		ptr = fifo_pop_stay(command_fifo_id);
		return *ptr;
	}
	return CMD_NONE;
}

void cmd_req(cp_cmd_enu _cmd) {
//	printf("cmd %s req\r\n", CMD_TITLE[_cmd]);
	fifo_push(command_fifo_id, &_cmd);
}

void task_dbg_mng() {
	char str[20];
	memset(str,'\0',20);
	int i;
	if (DBGBUF_RDY) {
		DBGBUF_RDY = false;
		for (i = 1;i <= 8;i++) {
			sprintf(str,"board %d on",i);
			if (strcmp(DBGBUF, str) == 0) {
				if(i == last_board){
					printf("BOARD %d ON ALREADY\r\n",board_state());
				} else {
					if(board_state() != 0) {
						printf("BOARD %d OFF AUTOMATICALLY\r\n",board_state());
						board_switch(board_state(),OFF);
					}
					board_switch(i, ON);
					printf("BOARD %d ON\r\n",board_state());
					last_board = board_state();
				}
			return;
			}
		}
		for (i = 1;i <= 8;i++){
			sprintf(str,"board %d off",i);
			if (strcmp(DBGBUF, str) == 0) {
				board_switch(i, OFF);
				printf("BOARD %d OFF\r\n",i);
				return;
			}
		}
		for (i = 1;i <= 8;i++){
			sprintf(str,"run %d",i);
			if (strcmp(DBGBUF, str) == 0) {
				if (i == board_state()){
					DBGBUF_CNTR = 0;
					req_start = true;
				} else {
					printf("BOARD %d IS OFF\r\n",i);
				}
				return;
			}
		}
		for (i = 1;i <= 8;i++){
			sprintf(str,"stop %d",i);
			if (strcmp(DBGBUF, str) == 0) {
				printf("TEST STOPPED BY CMD");
				DBGBUF_CNTR = 0;
				req_stop = 0;
				return;
			}
		}
		DBGBUF_CNTR = 0;
	}
}
void task_test() {
	TASK_MAKE(false);
	static int N = 0;
	static long unsigned int adc_temp[5] = {0,0,0,0,0};
	uint16_t current_value;
	if (requested_stop()) {
		st = 0;
		fifo_clear(command_fifo_id);
		connection_flag = false;
	}
	if (st != 0) {
		requested_start();
	}
	if (st == 0) {
		if (requested_start()) {
			printf("TEST-RUNNING\r\n");
			TASK_STEP(1);
			fifo_clear(command_fifo_id);
		}
	} else if (st == 1){
		adc_temp[PIR_POWER_ADC] += adc_mng_value(pir_power);
		adc_temp[POWER_1_ADC] += adc_mng_value(a_power_2);
		adc_temp[POWER_2_ADC] += adc_mng_value(a_power_1);
		N++;
		if (delay_s(ts, 3)) {
			adc_temp[PIR_POWER_ADC] /= N;
			adc_temp[POWER_1_ADC] /= N;
			adc_temp[POWER_2_ADC] /= N;
			N = 0;
			current_value = ((uint16_t)(adc_temp[POWER_2_ADC] * 0.805) - (uint16_t)(adc_temp[POWER_1_ADC] * 0.805));
			printf("CURRENT: %d\r\n",current_value);
//			printf("v1: %d\r\n", (uint16_t)(adc_temp[POWER_1_ADC] * 0.805));
//			printf("v2: %d\r\n", (uint16_t)(adc_temp[POWER_2_ADC] * 0.805));
			printf("POWER 5V: %d\r\n", (uint16_t)(adc_temp[PIR_POWER_ADC] * 0.805 * 2));
			adc_temp[PIR_POWER_ADC] = 0;
			adc_temp[POWER_1_ADC] = 0;
			adc_temp[POWER_2_ADC] = 0;
			TASK_STEP(2);
		}
	} else if (st == 2) {
		if (delay_s(ts, 3)) {
		cmd_req(CPC_OFF);
		TASK_STEP(3);
		}
	} else if (st == 3) {
		if (delay_s(ts, 1)  && fifo_empty(command_fifo_id)) {
			GPIO_WriteBit(COM_SWITCH_PORT,COM_SWITCH_PIN, ON);
			TASK_STEP(4);
		}
		if (delay_s(ts, 3)) {
			printf("err SENDING CPC_OFF\r\n");
			printf("CONNECTION WITH PIR FAILD\r\n");
			st = 0;
		}
	} else if (st == 4){
		if(delay_s(ts, 1)){
			if(GPIO_ReadInputDataBit(ncPortArray[board_state()-1],ncPinArray[board_state()-1]) == ON){
				printf("err CPC OFF\r\n");
				GPIO_WriteBit(COM_SWITCH_PORT,COM_SWITCH_PIN, OFF);
			}
			TASK_STEP(5);
		}
	} else if(st == 5){
		if(delay_s(ts, 1)){
			if(GPIO_ReadInputDataBit(ncPortArray[board_state()-1],ncPinArray[board_state()-1]) == ON){
				printf("err CPC OFF\r\n");
			}
			TASK_STEP(6);
			cmd_req(CPC_ON);
		}
	} else if(st == 5){
		if (delay_s(ts, 1)  && fifo_empty(command_fifo_id)) {
			GPIO_WriteBit(COM_SWITCH_PORT,COM_SWITCH_PIN, ON);
			TASK_STEP(6);
		}
		if (delay_s(ts, 3)) {
			printf("err SENDING CPC_ON\r\n");
			printf("CONNECTION WITH PIR FAILD\r\n");
			st = 0;
		}
	} else if(st == 6) {
		if(delay_s(ts, 1)){
			if(GPIO_ReadInputDataBit(ncPortArray[board_state()-1],ncPinArray[board_state()-1]) == OFF){
				printf("err CPC ON\r\n");
			}
			GPIO_WriteBit(COM_SWITCH_PORT,COM_SWITCH_PIN, OFF);
			TASK_STEP(7);
		}
	} else if(st == 7){
		if(delay_s(ts,1)){
			if(GPIO_ReadInputDataBit(ncPortArray[board_state()-1],ncPinArray[board_state()-1]) == ON){
				printf("err CPC ON\r\n");
			}
			TASK_STEP(8);
			printf("CPC-FIN\r\n");
		}
	}  else if (st == 8){
		adc_temp[PIR_SENSOR_ADC] += adc_mng_value(sensor_adc);
		adc_temp[PIR_CAP_ADC] += adc_mng_value(cap_adc);
		N++;
//		printf("CAP VOLTAGE: %d\r\n", (uint16_t)(adc_temp[PIR_CAP_ADC] * 0.805));
		if(delay_s(ts,15)){
			adc_temp[PIR_SENSOR_ADC] /= N;
			adc_temp[PIR_CAP_ADC] /= N;
			N = 0;
			printf("SENSOR VOLTAGE: %d\r\n", (uint16_t)(adc_temp[PIR_SENSOR_ADC] * 0.805));
			printf("CAP VOLTAGE: %d\r\n", (uint16_t)(adc_temp[PIR_CAP_ADC] * 0.805));
			cmd_req(NONE);
			st++;
		}
	} else if (st == 9){
		if(delay_s(ts,10)){
			printf("err RESIVIENG DATA FORM PIR\r\n");
			st = 0;
		}
		if(delay_s(ts,2) && fifo_empty(command_fifo_id)){
			if(PirHealth[PIR_SENSOR_READY] && PirHealth[PIR_TEMP_READY]){
				TASK_STEP(10);
			} else {
				printf("WAIT FOR READLY\r\n");
				st = 8;
			}
		}
	} else if (st == 10) {

			printf("TEST-FIN\r\n");
			TASK_STEP(0);

	}
}

void task_init(){
	command_fifo_id = fifo_req(command_fifo, 1, 10, "Command Fifo");
	fifo_clear(command_fifo_id);

}


