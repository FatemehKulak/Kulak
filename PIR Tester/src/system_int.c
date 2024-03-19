#include <def.h>
#include <system_init.h>

void power_selection_pin_init(){
    GPIO_InitTypeDef PSP;
	//BOARD 1
	PSP.GPIO_Speed = GPIO_Speed_50MHz;
	PSP.GPIO_Mode = GPIO_Mode_Out_PP;
	PSP.GPIO_Pin = POWER_1_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);
	//BOARD2
	PSP.GPIO_Pin = POWER_2_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD3
	PSP.GPIO_Pin = POWER_3_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD4
	PSP.GPIO_Pin = POWER_4_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD5
	PSP.GPIO_Pin = POWER_5_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD6
	PSP.GPIO_Pin = POWER_6_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD7
	PSP.GPIO_Pin = POWER_7_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//BOARD8
	PSP.GPIO_Pin = POWER_8_PIN;
	GPIO_Init(POWER_SELECTION_PORT, &PSP);

	//com
	PSP.GPIO_Pin = COM_SWITCH_PIN;
	GPIO_Init(COM_SWITCH_PORT, &PSP);
}

void mux_pin_init()
{
	GPIO_InitTypeDef MP;
	MP.GPIO_Speed = GPIO_Speed_10MHz;
	MP.GPIO_Mode = GPIO_Mode_Out_PP;
	MP.GPIO_Pin = MUX_0_PIN;
	GPIO_Init(MUX_0_PORT, &MP);

	MP.GPIO_Pin = MUX_1_PIN;
	GPIO_Init(MUX_2_PORT, &MP);

	MP.GPIO_Pin = MUX_2_PIN;
	GPIO_Init(MUX_2_PORT, &MP);

	MP.GPIO_Pin = MUX_EN_PIN;
	GPIO_Init(MUX_EN_PORT, &MP);

}

void nc_pin_init()
{
	GPIO_InitTypeDef 	NC;

	NC.GPIO_Speed = GPIO_Speed_10MHz;
	NC.GPIO_Mode = GPIO_Mode_IPU;             //pull-up input
	NC.GPIO_Pin = M_NC_1_PIN;
	GPIO_Init(M_NC_1_PORT, &NC);

	NC.GPIO_Pin = M_NC_2_PIN;
	GPIO_Init(M_NC_2_PORT, &NC);

	NC.GPIO_Pin = M_NC_3_PIN;
	GPIO_Init(M_NC_3_PORT, &NC);

	NC.GPIO_Pin = M_NC_4_PIN;
	GPIO_Init(M_NC_4_PORT, &NC);

	NC.GPIO_Pin = M_NC_5_PIN;
	GPIO_Init(M_NC_5_PORT, &NC);


	NC.GPIO_Pin = M_NC_6_PIN;
	GPIO_Init(M_NC_6_PORT, &NC);

	NC.GPIO_Pin = M_NC_7_PIN;
	GPIO_Init(M_NC_7_PORT, &NC);

	NC.GPIO_Pin = M_NC_8_PIN;
	GPIO_Init(M_NC_8_PORT, &NC);

}



void debug_init() {
    GPIO_InitTypeDef gp;
	gp.GPIO_Speed = GPIO_Speed_50MHz;
	gp.GPIO_Mode = GPIO_Mode_AF_PP;
	gp.GPIO_Pin =  TX_PIN;
	GPIO_Init(TX_PORT, &gp);
	gp.GPIO_Pin =  DBG_TX_PIN;
	GPIO_Init(DBG_TX_PORT, &gp);

	gp.GPIO_Mode = GPIO_Mode_IPU;
	gp.GPIO_Pin =  RX_PIN;
	GPIO_Init(RX_PORT, &gp);
	gp.GPIO_Pin =  DBG_RX_PIN;
	GPIO_Init(DBG_RX_PORT, &gp);
	GPIO_WriteBit(RX_PORT,RX_PIN,1);
	GPIO_WriteBit(DBG_RX_PORT,DBG_RX_PIN,1);

	USART_InitTypeDef ug;

	ug.USART_BaudRate =  230400;
	ug.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ug.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	ug.USART_Parity = USART_Parity_No;
	ug.USART_StopBits = USART_StopBits_1;
	ug.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &ug);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	ug.USART_BaudRate =125000;
	USART_Init(USART3, &ug);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
	USART_Cmd(USART1, ENABLE);

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART3_IRQn);
}

void debugc(char _c) {
	USART_SendData(USART1, _c);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
}

void debugs(char *_str) {
	for (int i = 0; _str[i] != '\0'; i++) {
		debugc(_str[i]);
	}
}

void tester_adc_init(){
	static GPIO_InitTypeDef gp;
	gp.GPIO_Mode = GPIO_Mode_AIN;
	gp.GPIO_Speed = GPIO_Speed_10MHz;

	gp.GPIO_Pin = ADC_SENSOR_PIN;
	GPIO_Init( ADC_SENSOR_PORT, &gp);
	GPIO_WriteBit( ADC_SENSOR_PORT, ADC_SENSOR_PIN, false);

	gp.GPIO_Pin = ADC_NTC_PIN	;
	GPIO_Init(ADC_NTC_PORT, &gp);
	GPIO_WriteBit(ADC_NTC_PORT, ADC_NTC_PIN	, false);

	gp.GPIO_Pin = ADC_CAP_PIN;
	GPIO_Init(ADC_CAP_PORT, &gp);
	GPIO_WriteBit(ADC_CAP_PORT, ADC_CAP_PIN, false);

	gp.GPIO_Pin = A_POWER_1_PIN;
	GPIO_Init(A_POWER_1_PORT, &gp);
	GPIO_WriteBit(A_POWER_1_PORT, A_POWER_1_PIN, false);

	gp.GPIO_Pin = A_POWER_2_PIN;
	GPIO_Init(A_POWER_2_PORT, &gp);
	GPIO_WriteBit(A_POWER_2_PORT, A_POWER_2_PIN, false);

	gp.GPIO_Pin = POWER_PIN;
	GPIO_Init(POWER_PORT, &gp);
	GPIO_WriteBit(POWER_PORT, POWER_PIN, false);

	adc_init(ADC1);
	adc_mng_init();
	sensor_adc = adc_add_channel(ADC_SENSOR_CHANNEL);
//	adc_add_channel(ADC_NTC_CHANNEL	);
	cap_adc = adc_add_channel(ADC_CAP_CHANNEL);
	a_power_1 = adc_add_channel(A_POWER_1_CHANNEL);
	a_power_2 = adc_add_channel(A_POWER_2_CHANNEL);
	pir_power = adc_add_channel(POWER_CHANNEL);
}

static uint8_t start_flag = 0;

void adc_run() {
	static uint64_t ts;
	if (start_flag == 1) {
		if (delay_ms(ts, 5)) {
			ts = get_timestamp();

			uint value_1 = ((adc_convert(adc_mng_value(0))) * 2); //just for nv5 output.
			uint value_2 = ((adc_convert(adc_mng_value(1))) * 2);
			printf("%d,%d\r\n", value_1, value_2);
		}

	}
}


void system_init() {
	SystemInit();
	SystemCoreClockUpdate();
	uint64_t tick = SystemCoreClock / 1000;
	SysTick_Config(tick);
	NVIC_EnableIRQ(15);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	AFIO->MAPR = 0;
	AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_PARTIALREMAP;
//	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE;



	power_selection_pin_init();
	mux_pin_init();
	nc_pin_init();

	debug_init();
	tester_adc_init();
	GPIO_WriteBit(MUX_EN_PORT,MUX_EN_PIN, OFF); //Enable multiplexer
	BOARD_1(OFF);
	BOARD_2(OFF);
	BOARD_3(OFF);
	BOARD_4(OFF);
	BOARD_5(OFF);
	BOARD_6(OFF);
	BOARD_7(OFF);
	BOARD_8(OFF); // Turn all of the PIRs off
	printf("**Ready to test** \r\n");

}
