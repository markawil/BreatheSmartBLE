 /*
 * HM10.h
 *
 *  Created on: Jan 16, 2025
 *      Author: markwilkinson
 */

#ifndef CUSTOMDRIVERS_BLE_HM10_H_
#define CUSTOMDRIVERS_BLE_HM10_H_

#include "stm32l4xx_hal.h"

typedef enum {
	SENSOR_DATA_TEMP = 0x00,
	SENSOR_DATA_TVOC = 0x01,
	SENSOR_DATA_CO2  = 0x02,
	SENSOR_DATA_HUM  = 0x03,
	SENSOR_DATA_PRES = 0x04,
	SENSOR_DATA_BATT = 0x05
}SENSOR_DATA_TYPE;

#define MAX_BUFFER_LEN 100u

void hm10_send_sensor_data(float data, SENSOR_DATA_TYPE type, uint8_t *tx_buffer);
void hm10_uart_handle_rx(uint8_t rx_data);

#endif /* CUSTOMDRIVERS_BLE_HM10_H_ */
