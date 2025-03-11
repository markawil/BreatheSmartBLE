/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

#include "usart.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "bme280.h"
#include "ccs811.h"
#include "const.h"
#include "HM10.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

uint8_t ccs811_init_count = 0;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00B07CB4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  if (mpu6050_init() == false)
  {
	  // TODO: handle init error for mpu6050
  }

//  	while(ccs811_init() == false)
//  	{
//  		HAL_Delay(1000); // wait and keep trying to init until device ready
//  		ccs811_init_count++;
//  		if (ccs811_init_count > 5) // try 5 times in 5 seconds otherwise move on.
//  		{
//  			break;
//  		}
//  	}

  	//  if (bme280_init() == false)
  	//  {
  	//	  // TODO: handle BME280 init failure
  	//  }

  /* USER CODE END I2C1_Init 2 */

}
/* I2C3 init function */
void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x0060112F;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  ssd1306_set_LCD(LCD_NUMBER_1); // not needed, but good to be explicit about which ssd1306 we're init'ing
  ssd1306_Init();

  draw_github_intro();
  HAL_Delay(1000); // let the logo stay on the screen for a second

  ssd1306_set_LCD(LCD_NUMBER_2); // now init the 2nd LCD
  ssd1306_Init();

  draw_github_intro();
  HAL_Delay(1000); // let the logo stay on the screen for a second
  print_device_name("");

  /* USER CODE END I2C3_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspInit 0 */

  /* USER CODE END I2C3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
    PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C3 GPIO Configuration
    PA7     ------> I2C3_SCL
    PB4 (NJTRST)     ------> I2C3_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C3 clock enable */
    __HAL_RCC_I2C3_CLK_ENABLE();
  /* USER CODE BEGIN I2C3_MspInit 1 */

  /* USER CODE END I2C3_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspDeInit 0 */

  /* USER CODE END I2C3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C3_CLK_DISABLE();

    /**I2C3 GPIO Configuration
    PA7     ------> I2C3_SCL
    PB4 (NJTRST)     ------> I2C3_SDA
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);

  /* USER CODE BEGIN I2C3_MspDeInit 1 */

  /* USER CODE END I2C3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void print_temperature_data(void)
{
	ssd1306_set_LCD(LCD_NUMBER_1);

	float temp_value = mpu6050_get_temperature_data();
	// comes in as C, convert to F
	// (C × 9/5) + 32
	float temp_value_F = (temp_value * 1.8) + 32.0;
	char temp_msg[MAX_BUFFER_LEN] = "";

	snprintf(temp_msg, MAX_BUFFER_LEN, "temperature is: %.1f\r\n", temp_value_F);

	// output over the uart to serial port
	uint16_t temp_buffer_len = strlen(temp_msg);
	serial_uart_send_tx(temp_msg, temp_buffer_len);

	// output over BLE on the HM10
	hm10_uart_send_tx(temp_value_F, SENSOR_DATA_TEMP);

	// output to the OLED display
	ssd1306_Fill(Black);
	ssd1306_SetCursor(2, 0);
	char temp_F[MAX_BUFFER_LEN] = "";
	snprintf(temp_F, MAX_BUFFER_LEN, "Temp:%.1fF", temp_value_F);
	ssd1306_WriteString(temp_F, Font_11x18, White);
}

void print_device_name(const char *name)
{
	// output to the 2nd OLED display lines 1 and 2
	ssd1306_set_LCD(LCD_NUMBER_2);
	ssd1306_Fill(Black);

	if (strlen(name) > 0)
	{
		ssd1306_SetCursor(2, 0);
		ssd1306_WriteString("Connected:", Font_11x18, White);
		ssd1306_SetCursor(2, 20);
		ssd1306_WriteString((char*)name, Font_11x18, White);
	}
	else
	{
		ssd1306_SetCursor(2, 0);
		ssd1306_WriteString("Disconnected", Font_7x10, White);
	}

	ssd1306_UpdateScreen();
}

void pollBME280() {

	float humidity = 35.0;
	float pressure = 26.75;
/*
 *  use when BME280 sensor is setup and working
	struct bme280_uncomp_data bme280_data;
	bme280_load_temp_pressure_humidity(&bme28_data);

	humidity = (float)bme280_data.humidity;
	pressure = (float)bme280_data.pressure;
 *
 */

	hm10_uart_send_tx((float)humidity, SENSOR_DATA_HUM);
	hm10_uart_send_tx((float)pressure, SENSOR_DATA_PRES);
}

void pollCCS811()
{
	uint16_t co2 = 400;
	uint16_t tvoc = 25;

	/* Use when the CCS811 sensor is working right.
	if (ccs811_check_data_ready() && (ccs811_init_count < 6))
	{
		co2 = ccs811_get_CO2_PPM();
		tvoc = ccs811_get_TVOC_PPB();
		write_baseline(0x447B);
		HAL_Delay(250);
	}
	*/

	ssd1306_set_LCD(LCD_NUMBER_1);

	// output CO2 and TVOC values

	ssd1306_SetCursor(2, 20);
	char co2_msg[MAX_BUFFER_LEN] = "";
	if (co2 == 0)
	{
		snprintf(co2_msg, MAX_BUFFER_LEN, "CO2:---ppm");
	}
	else
	{
		snprintf(co2_msg, MAX_BUFFER_LEN, "CO2:%dppm", co2);
	}
	ssd1306_WriteString(co2_msg, Font_11x18, White);

	ssd1306_SetCursor(2, 40);
	char tvoc_buffer[MAX_BUFFER_LEN] = "";
	if (tvoc == 0)
	{
		snprintf(tvoc_buffer, MAX_BUFFER_LEN, "TVOC:---ppb");
	}
	else
	{
		snprintf(tvoc_buffer, MAX_BUFFER_LEN, "TVOC:%dpb", tvoc);
	}

	ssd1306_WriteString(tvoc_buffer, Font_11x18, White);

	// output over BLE on the HM10
	hm10_uart_send_tx((float)co2, SENSOR_DATA_CO2);
	hm10_uart_send_tx((float)tvoc, SENSOR_DATA_TVOC);
}

/* USER CODE END 1 */
