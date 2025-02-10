/*
 * HM10.c
 *
 *  Created on: Jan 16, 2025
 *      Author: markwilkinson
 */

#include "HM10.h"
#include "error.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

const char hm10_error[] = "Error! Something failed for HM10 module\r\n";
const char hm10_init_error[] = "Error! HM10 init failed.\r\n";

void hm10_uart_send_tx(UART_HandleTypeDef *huart, const char *buffer, uint16_t buffer_len)
{
	HAL_StatusTypeDef result = HAL_UART_Transmit(huart, (uint8_t *)buffer, buffer_len, 100);
	if (result != HAL_OK)
	{
		show_error(hm10_error, sizeof(hm10_error), SerialUART_Out);
	}
}

void hm10_uart_handle_tx()
{
	// clear the TX buffer after sending
}

void hm10_uart_handle_rx(uint8_t rx_data)
{
	// Temporary testing solution to verify rx and tx with the device.

	// check the incoming characters sent from
	if (rx_data == 78) // ASCII for N (No)
	{
		// turn the green LED off
		HAL_GPIO_WritePin(EXT_LED_GPIO_Port, EXT_LED_Pin, GPIO_PIN_RESET);
	}
	else if (rx_data == 89) // ASCII for Y (Yes)
	{
		// turn the green LED on
		HAL_GPIO_WritePin(EXT_LED_GPIO_Port, EXT_LED_Pin, GPIO_PIN_SET);
	}
}
