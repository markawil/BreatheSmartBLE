/*
 * error.c
 *
 *  Created on: Jan 31, 2025
 *      Author: markwilkinson
 */

#include "error.h"
#include "usart.h"

extern UART_HandleTypeDef huart2;
/*
 * Customize this to output to different destinations, LCD panel, over BLE, etc.
 */

void show_error(const char *error_msg, uint16_t buffer_len, ERROR_OUTPUT output_type)
{
	if (buffer_len == 0)
	{
		return;
	}

	if (error_msg == NULL)
	{
		return;
	}

	serial_uart_send_tx(error_msg, buffer_len);
}


