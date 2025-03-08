/*
 * ccs811.c
 *
 *  Created on: Oct 30, 2024
 *      Author: markwilkinson
 *
 *      Following example code from:
 *      https://github.com/DFRobot/DFRobot_CCS811
 */

#include "ccs811.h"
#include "stm32l4xx_hal.h"
#include "i2c.h"
#include "ccs811_reg_map.h"

extern I2C_HandleTypeDef hi2c1;

void set_measurement_mode(CCS811_MODE mode);
void set_temp_humidity(float temperature, float humidity);

// private function prototypes
bool write_CCS811_register(uint8_t reg_add, uint8_t *reg_val, uint16_t size);
bool read_CCS811_register(uint8_t reg_add, uint8_t *data, uint16_t data_len);

/*
 *  Initialize the CCS811 over i2c
 *  Returns true or false based on initialization success.
 */
bool ccs811_init(void)
{
	ccs811_reset();
	HAL_Delay(100);
	uint8_t id=0;

	// check the Register HW ID by making a call, ensuring i2C bus is connected.
	if (read_CCS811_register(CCS811_REG_HW_ID, &id, 1))
	{
		return false;
	}

	// confirm the id is the HW ID
	if (id != CCS811_HW_ID)
	{
		return false;
	}

	// write NULL into the bootloader
	(void)write_CCS811_register(CCS811_BOOTLOADER_APP_START, NULL, 0);

	// set the measurement cycle rate to 1 second readings
	set_measurement_mode(CCS811_MODE_1SEC);

	// set the temperature and humidity from the temp/hum sensor
	// (TODO) Should pull these values and not input base values
	set_temp_humidity(25, 50);

	// made it here, so we're good
	return true;
}

void ccs811_reset()
{
	uint8_t reset_values[4] = {0x11, 0xE5, 0x72, 0x8A};
	(void)write_CCS811_register(CCS811_REG_SW_RESET, reset_values, 4);
}

bool ccs811_check_data_ready()
{
	bool isReady = false;
	uint8_t status = 0;
	(void)read_CCS811_register(CCS811_REG_STATUS, &status, 1);
	if((status >> 3) & 0x01)
	{
		isReady = true;
	}
	return isReady;
}

void set_measurement_mode(CCS811_MODE mode)
{
	uint8_t measurement = 0;
	measurement = (mode << 4);
	(void)write_CCS811_register(CCS811_REG_MEAS_MODE, &measurement, 1);
}

void set_temp_humidity(float temperature, float humidity)
{
	int rounded_temp = 0;
	if(temperature > 0)
	{
		rounded_temp = (int)(temperature + 0.5);
	}
	else if (temperature < 0)
	{
		rounded_temp = (int)(temperature - 0.5);
	}

	int rounded_hum = (int)(humidity + 0.5);

	uint8_t env_data[4];
	env_data[0] = rounded_hum << 1;  // shift the binary number to left by 1. This is stored as a 7-bit value
	env_data[1] = 0;  // most significant fractional bit. Using 0 here - gives us accuracy of +/-1%. Current firmware (2016) only supports fractional increments of 0.5
	env_data[2] = rounded_temp << 1;
	env_data[3] = 0;

	(void)write_CCS811_register(CCS811_REG_ENV_DATA, env_data, 4);
}

uint16_t ccs811_get_CO2_PPM()
{
	uint8_t buffer[8];
	(void)read_CCS811_register(CCS811_REG_ALG_RESULT_DATA, buffer, 8);
	uint16_t co2_reading = (((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1]);
	return co2_reading;
}

uint16_t ccs811_get_TVOC_PPB()
{
	uint8_t buffer[8];
	(void)read_CCS811_register(CCS811_REG_ALG_RESULT_DATA, buffer, 8);
	uint16_t tovc_reading = (((uint16_t)buffer[2] << 8) | (uint16_t)buffer[3]);
	return tovc_reading;
}

uint16_t readBaseLine()
{
	uint8_t buffer[2];
	(void)read_CCS811_register(CCS811_REG_BASELINE, buffer, 2);
	uint16_t baseline = (uint16_t)buffer[0] << 8 | (uint16_t)buffer[1];
	return baseline;
}

void write_baseline(uint16_t baseline)
{
	uint8_t buffer[2];
	buffer[0] = baseline >> 8;
	buffer[1] = baseline;
	(void)write_CCS811_register(CCS811_REG_BASELINE, buffer, 2);
}

/*!
 * \brief     Reads from a register address passed into the function.
 * \param[in] reg_add - Address of the register we want to read.
 * \param[in] data - data to read value into.
 * \param[in] data_len - length of the data to read.
 * \return    reg_val - Value of the register we read from.
 */
bool read_CCS811_register(uint8_t reg_add, uint8_t *data, uint16_t data_len)
{
	HAL_StatusTypeDef i2c_rx_okay = HAL_I2C_Mem_Read(&hi2c1, CCS811_I2C_ADDRESS1, reg_add, 1, data, data_len, CCS811_I2C_MAX_TIMEOUT);
	if (i2c_rx_okay != HAL_OK)
	{
		// handle the error
	}

	return i2c_rx_okay == HAL_OK;
}

/*!
 * \brief     Writes a register value to a given register address.
 * \param[in] reg_add - Address of the register we want to write to.
 * \param[in] reg_val - Value of the register we want to write.
 * \param[in] size - 16bit size of the value to write.
 * \return    True is the register write was successful, false if not.
 */
bool write_CCS811_register(uint8_t reg_add, uint8_t *reg_val, uint16_t size)
{
	HAL_StatusTypeDef i2c_tx_okay = HAL_I2C_Mem_Write(&hi2c1, CCS811_I2C_ADDRESS1, reg_add, 1, reg_val, size, CCS811_I2C_MAX_TIMEOUT);
	if (i2c_tx_okay != HAL_OK)
	{
		// handle the error
	}

	return i2c_tx_okay == HAL_OK;
}
