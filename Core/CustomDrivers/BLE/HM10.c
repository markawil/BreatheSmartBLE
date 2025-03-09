/*
 * HM10.c
 *
 *  Created on: Jan 16, 2025
 *      Author: markwilkinson
 */

#include "HM10.h"
#include "error.h"
#include "gpio.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern UART_HandleTypeDef huart1;

void parse_OK_cmd(uint8_t next_byte);

const char hm10_error[] = "Error! Something failed for HM10 module\r\n";
const char hm10_init_error[] = "Error! HM10 init failed.\r\n";
static bool incoming_device_name = false;
static bool incoming_ok_cmd = false;
static uint8_t rx_data_s; // holds last character to be received
static uint8_t rx_name_buffer_s[100];
static uint8_t name_buffer_counter = 0;

/*
 * typedef enum {
	SENSOR_DATA_TEMP = 0x00,
	SENSOR_DATA_TVOC = 0x01,
	SENSOR_DATA_CO2  = 0x02,
	SENSOR_DATA_HUM  = 0x03,
	SENSOR_DATA_PRES = 0x04,
	SENSOR_DATA_BATT = 0x05
}SENSOR_DATA_TYPE;
 */


void hm10_send_sensor_data(float data, SENSOR_DATA_TYPE type, uint8_t *tx_buffer)
{
	switch(type)
	{
	case SENSOR_DATA_TEMP:
		snprintf((char *)tx_buffer, 20, "00:%.1f", data);
		break;
	case SENSOR_DATA_TVOC:
		snprintf((char *)tx_buffer, 20, "01:%.1f", data);
		break;
	case SENSOR_DATA_CO2:
		snprintf((char *)tx_buffer, 20, "02:%.1f", data);
		break;
	case SENSOR_DATA_HUM:
		snprintf((char *)tx_buffer, 20, "03:%.1f", data);
		break;
	case SENSOR_DATA_PRES:
		snprintf((char *)tx_buffer, 20, "04:%.1f", data);
		break;
	case SENSOR_DATA_BATT:
		snprintf((char *)tx_buffer, 20, "05:%.1f", data);
		break;
	default:
		return;
	}

	// Transmit the formatted string over the HM10
	HAL_StatusTypeDef result = HAL_UART_Transmit(&huart1, tx_buffer, 20, 100);
	if (result != HAL_OK)
	{
		show_error(hm10_error, sizeof(hm10_error), SerialUART_Out);
	}

	// clear the buffer for next transfer
	memset(tx_buffer, '\0', (size_t)MAX_BUFFER_LEN);
}

void hm10_uart_handle_rx(uint8_t rx_data)
{
	if (rx_data_s == 79 && rx_data == 75) // received the OK response
	{
		incoming_ok_cmd = true;
	}
	else if (incoming_ok_cmd) // parse the next character for OK command
	{
		parse_OK_cmd(rx_data);
	}
	// Check if incoming characters are saying YES or NO to turn the LED on or off
	else if (rx_data_s == 78 && rx_data == 79) // ASCII for N and O
	{
		// turn the green LED off
		HAL_GPIO_WritePin(EXT_LED_GPIO_Port, EXT_LED_Pin, GPIO_PIN_RESET);
	}
	else if (rx_data_s == 89 && rx_data == 69) // ASCII for Y and E
	{
		// turn the green LED on
		HAL_GPIO_WritePin(EXT_LED_GPIO_Port, EXT_LED_Pin, GPIO_PIN_SET);
	}
	// otherwise check if device is sending it's name
	else if (rx_data_s == 36 && rx_data == 36) // $$ code for start of sending device name
	{
		incoming_device_name = true;
	}
	// received end of the device name(!!) or 20 chars, print out the name
	else if ((rx_data_s == 33 && rx_data == 33) || name_buffer_counter == 21)
	{
		// print the device name out over i2c to the display
		print_device_name((char *)rx_name_buffer_s);

		// reset the name flag, counter and buffer
		incoming_device_name = false;
		name_buffer_counter = 0;
		memset(rx_name_buffer_s, '\0', (size_t)100);
	}
	else if (incoming_device_name && rx_data != 33) // ignore the ! after the name ends
	{
		rx_name_buffer_s[name_buffer_counter++] = rx_data; // store the next character for the name
	}

	rx_data_s = rx_data; // store last character
}

void parse_OK_cmd(uint8_t next_byte)
{
	if (rx_data_s == 76 && next_byte == 79) // LO for LOST indicating BLE client disconnected
	{
		// clear the connection and device name from LCD2
		incoming_ok_cmd = false;
		print_device_name("");
		incoming_device_name = false;
		name_buffer_counter = 0;
		memset(rx_name_buffer_s, '\0', (size_t)100);
	}
	else if (rx_data_s == 67 && next_byte == 79) // CO for CONN indicating the BLE client connected
	{
		incoming_ok_cmd = false;
	}
	// add more commands when they're needed
}
