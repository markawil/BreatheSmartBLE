/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

#include "HM10.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void serial_uart_handle_tx();
void serial_uart_handle_rx();
void hm10_uart_handle_rx_2();

#define MAX_BUFFER_LEN 100u

// receive buffers
uint8_t uart1_rx_data;
uint8_t uart2_rx_data;

// data buffers for serial port uart2
uint8_t uart2_tx_buffer[MAX_BUFFER_LEN];
uint8_t uart2_rx_buffer[MAX_BUFFER_LEN];
uint32_t counter_s = 0; // count how many bytes are received

const char* ok_response = "OK+";
static bool ok_received = false;

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  HAL_Delay(20); // delay needed to get UART working properly

  HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  HAL_Delay(20); // delay needed to get UART working properly

  HAL_UART_Transmit_IT(&huart2, uart2_tx_buffer, MAX_BUFFER_LEN);
  HAL_UART_Receive_IT(&huart2, &uart2_rx_data, 1);

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart2.Instance)
	{
		serial_uart_handle_tx();
	}
	else if (huart->Instance == huart1.Instance)
	{
		hm10_uart_handle_tx();
	}
}

// called from HAL for any UART callback setup with Interrupts.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// if from the HCO5 or HM10 Bluetooth module
	if (huart->Instance == huart1.Instance)
	{
//		for (uint8_t i=0; i<20; i++)
//		{
//			if (uart1_rx_data[i] != '\0')
//			{
//				hm10_uart_handle_rx(uart1_rx_data[i]);
//			}
//			else
//			{
//				break;
//			}
//		}

		hm10_uart_handle_rx(uart1_rx_data);
		HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);
	}
	else if (huart->Instance == huart2.Instance)
	{
		serial_uart_handle_rx();
		HAL_UART_Receive_IT(&huart2, &uart2_rx_data, 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	char *error_msg = "UART error occurred.";

	uint32_t error = huart->ErrorCode;

	if (huart->Instance == huart1.Instance)
	{
		if(error & HAL_UART_ERROR_FE)
		{
			//frame error detected
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE); //clear frame error flag

		}
		else if(error & HAL_UART_ERROR_ORE)
		{
			//overrun error
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE); //clear overrun flag
			__HAL_UART_FLUSH_DRREGISTER(huart); //flush data
		}
		else if(error & HAL_UART_ERROR_PE)
		{
			// parity error occurred
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE); //clear frame error flag
		}

		error_msg = "HM10 UART error occurred.";
		serial_uart_send_tx(error_msg, sizeof(error_msg));
	}
	else if (huart->Instance == huart2.Instance)
	{
		error_msg = "Serial port UART error occurred";
		// probably output over BLE or just output to terminal.
	}
}

const char* hello_cmd = "hello";

/*!
 * \brief    Sends content in the buffer over uart tx
 * \param[in] tx_buff - Buffer with the message we want to send.
 * \param[in] buffer_len - Maximum length of the buffer we want to send.
 */
void serial_uart_send_tx(const char *buffer, uint16_t buffer_len)
{
	strcpy((char *)uart2_tx_buffer, buffer);
	HAL_UART_Transmit(&huart2, uart2_tx_buffer, buffer_len, 100);
}

/*!
 * \brief    Readies the uart tx buffer
 * \param[in] huart - pointer to huart handle.
 */
void serial_uart_handle_tx()
{
	memset(uart2_tx_buffer, '\0', (size_t)MAX_BUFFER_LEN); //empty the transmit data buffer to be ready for new data.
}

/*!
 * \brief    Handles uart rx by taking in next character, processing it, then sending out over tx
 * \param[in] huart - pointer to huart handle.
 */
void serial_uart_handle_rx()
{
	// if we didn't receive the carriage return increment the rx_buffer and add the next character
	if (uart2_rx_data != '\r')
	{
		// if there is data coming into the rx_data pointer that isn't the "Enter" character then add it to the buffer.
		uart2_rx_buffer[counter_s++] = uart2_rx_data;
		// Get ready for new data in rx_data_s pointer.
		HAL_UART_Receive_IT(&huart2, &uart2_rx_data, 1);
		return;
	}

	// otherwise we did get a carriage return, check against our expected string
	if (strcmp(hello_cmd, (char*)uart2_rx_buffer) == 0) // string compare the command against what we received from the sender.
	{
		const char *response = "Hello to you too!\r\n";
		strcpy((char*)uart2_tx_buffer, response); // move response into tx buffer
	}
	else
	{
		// otherwise put an error in the tx buffer.
		const char *error = "Uh oh, didn't get the right word...\r\n";
		strcpy((char*)uart2_tx_buffer, error);
	}

	// now transmit out the answer over tx.
	HAL_UART_Transmit(&huart2, uart2_tx_buffer, (uint16_t)MAX_BUFFER_LEN, 100);
	// clear both buffers
	memset(uart2_tx_buffer, '\0', (size_t)MAX_BUFFER_LEN);
	memset(uart2_rx_buffer, '\0', (size_t)MAX_BUFFER_LEN);
	counter_s = 0; // reset the counter to be ready for new data.
}

void hm10_uart_handle_rx_2()
{
	if (strcmp(ok_response, (char*)uart1_rx_data) == 0)
	{
		ok_received = true;
	}
	else
	{
		ok_received = false;
	}

	memset(uart1_rx_data, '\0', (size_t)3);
}

/* USER CODE END 1 */
