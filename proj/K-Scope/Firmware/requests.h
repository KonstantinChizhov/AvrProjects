
/* This header is shared between the firmware and the host software. It
 * defines the USB request numbers (and optionally data types) used to
 * communicate between the host and the device.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

enum Requests{
	CUSTOM_RQ_GET_ADC_1_CHANEL	= 0,
	CUSTOM_RQ_GET_ADC_2_CHANEL	= 1,
	CUSTOM_RQ_GET_ADC_3_CHANEL	= 3,
	CUSTOM_RQ_GET_ADC_4_CHANEL	= 4,

	CUSTOM_RQ_GET_ADC_1_SAMPLE	= 10,
	CUSTOM_RQ_GET_BUFFER_SIZE	= 12,
	CUSTOM_RQ_GET_CPU_FREQ   	= 13,
	
    CUSTOM_RQ_WRITE_DAC			= 15,
	CUSTOM_RQ_GET_COUNTER		= 16,
	CUSTOM_RQ_SET_TIMER_PERIOD	= 17,

	CUSTOM_RQ_PING				= 20,
	CUSTOM_RQ_TRANSFER_TEST		= 21
};

#endif /* __REQUESTS_H_INCLUDED__ */
