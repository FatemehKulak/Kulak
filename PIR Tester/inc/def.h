

#ifndef DEF_H_
#define DEF_H_

#include "stm32f10x.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <delay.h>

#include <adc.h>
#include <adc_mng.h>

typedef enum {
	false = 0,
	ret_false = false,
	FALSE = false,

	true = 1,
	ret_true = true,
	TRUE = true,
} bool_enu;

#define ON 1
#define OFF 0
//dbg usart
#define DBG_USART					USART1

#define DBG_TX_PORT					GPIOA
#define DBG_TX_PIN					GPIO_Pin_9

#define DBG_RX_PORT					GPIOA
#define DBG_RX_PIN					GPIO_Pin_10

#define	USART3_TEMP					USART2//USART3

#define USART_IRQn					USART2_IRQn//USART3_IRQn
#define TX_PORT						GPIOC
#define TX_PIN						GPIO_Pin_10

#define RX_PORT						GPIOC
#define RX_PIN						GPIO_Pin_11

//adc

#define ADC							ADC1

#define ADC_SENSOR_PORT				GPIOC
#define ADC_SENSOR_PIN				GPIO_Pin_1
#define ADC_SENSOR_CHANNEL			ADC_Channel_11

#define ADC_NTC_PORT				GPIOC
#define ADC_NTC_PIN					GPIO_Pin_2
#define ADC_NTC_CHANNEL				ADC_Channel_12

#define ADC_CAP_PORT				GPIOC
#define ADC_CAP_PIN				    GPIO_Pin_3
#define ADC_CAP_CHANNEL			    ADC_Channel_13


#define A_POWER_1_PORT				GPIOC
#define A_POWER_1_PIN				GPIO_Pin_4
#define A_POWER_1_CHANNEL			ADC_Channel_14

#define A_POWER_2_PORT				GPIOC
#define A_POWER_2_PIN				GPIO_Pin_5
#define A_POWER_2_CHANNEL           ADC_Channel_15

#define POWER_PORT					GPIOC
#define POWER_PIN					GPIO_Pin_0
#define POWER_CHANNEL				ADC_Channel_10

#define COM_SWITCH_PORT				GPIOB
#define COM_SWITCH_PIN				GPIO_Pin_7


//gpio power selection pin
#define POWER_SELECTION_PORT		GPIOA
#define POWER_1_PIN					GPIO_Pin_7
#define POWER_2_PIN					GPIO_Pin_6
#define POWER_3_PIN					GPIO_Pin_5
#define POWER_4_PIN					GPIO_Pin_4
#define POWER_5_PIN					GPIO_Pin_3
#define POWER_6_PIN					GPIO_Pin_2
#define POWER_7_PIN					GPIO_Pin_1
#define POWER_8_PIN					GPIO_Pin_0

#define BOARD_1(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_7, ON);
#define BOARD_2(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_6, ON);
#define BOARD_3(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_5, ON);
#define BOARD_4(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_4, ON);
#define BOARD_5(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_3, ON);
#define BOARD_6(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_2, ON);
#define BOARD_7(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_1, ON);
#define BOARD_8(ON)					GPIO_WriteBit(POWER_SELECTION_PORT,GPIO_Pin_0, ON);


//gpio mux pin
#define MUX_0_PORT					GPIOB
#define MUX_0_PIN					GPIO_Pin_6
#define MUX_0(ON)					GPIO_WriteBit(MUX_0_PORT,MUX_0_PIN, ON);

#define MUX_1_PORT					GPIOB
#define MUX_1_PIN  					GPIO_Pin_5
#define MUX_1(ON)					GPIO_WriteBit(MUX_1_PORT,MUX_1_PIN, ON);

#define MUX_2_PORT					GPIOD
#define MUX_2(ON)					GPIO_WriteBit(MUX_2_PORT,MUX_2_PIN, ON);
#define MUX_2_PIN					GPIO_Pin_2


#define MUX_EN_PORT					GPIOC
#define MUX_EN_PIN					GPIO_Pin_12

//nc pin
#define	M_NC_1_PIN					GPIO_Pin_9
#define	M_NC_1_PORT					GPIOC

#define	M_NC_2_PIN					GPIO_Pin_8
#define	M_NC_2_PORT					GPIOC

#define	M_NC_3_PIN					GPIO_Pin_7
#define	M_NC_3_PORT					GPIOC

#define	M_NC_4_PIN					GPIO_Pin_6
#define	M_NC_4_PORT					GPIOC

#define	M_NC_5_PIN					GPIO_Pin_15
#define	M_NC_5_PORT					GPIOB

#define	M_NC_6_PIN					GPIO_Pin_14
#define	M_NC_6_PORT					GPIOB

#define	M_NC_7_PIN					GPIO_Pin_13
#define	M_NC_7_PORT					GPIOB

#define	M_NC_8_PIN					GPIO_Pin_12
#define	M_NC_8_PORT					GPIOB



#define TASK_MAKE(DBG) \
	static uint8_t st = 0, lst = 0;	\
	static uint64_t ts;	\
	if (st != lst) {	\
		if (DBG) {	\
			printf("STEP-%d\r\n", st);	\
		}	\
		lst = st;	\
		ts = get_timestamp();	\
	}

#define TASK_STEP(STEP) \
		st = STEP;


#endif /* DEF_H_ */
