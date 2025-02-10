 /*
 * HM10.h
 *
 *  Created on: Jan 16, 2025
 *      Author: markwilkinson
 */

#ifndef CUSTOMDRIVERS_BLE_HM10_H_
#define CUSTOMDRIVERS_BLE_HM10_H_

#include "stm32l4xx_hal.h"

void hm10_uart_send_tx(UART_HandleTypeDef *huart, const char *buffer, uint16_t buffer_len);
void hm10_uart_handle_tx();
void hm10_uart_handle_rx(uint8_t rx_data);

#endif /* CUSTOMDRIVERS_BLE_HM10_H_ */
