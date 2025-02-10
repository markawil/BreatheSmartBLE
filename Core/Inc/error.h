/*
 * error.h
 *
 *  Created on: Jan 31, 2025
 *      Author: markwilkinson
 */

#ifndef SRC_ERROR_H_
#define SRC_ERROR_H_

#include "stm32l4xx_hal.h"

typedef enum {
    SerialUART_Out = 0x00, // outputting the error over serial uart
    BLE_Out = 0x01,  // outputting the error to BLE client
	LCD_Out = 0x02, // outputting the error to an LCD display
} ERROR_OUTPUT;


void show_error(const char *error_msg, uint16_t buffer_len, ERROR_OUTPUT output_type);

#endif /* SRC_ERROR_H_ */
