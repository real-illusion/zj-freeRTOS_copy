/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/**
 * @brief UART1 DMA发送函数
 * @param pData 待发送数据指针
 * @param Size 发送数据长度（字节数）
 * @return HAL_OK 成功, HAL_ERROR 失败, HAL_BUSY 忙线（前一个DMA传输未完成）
 */
HAL_StatusTypeDef UART1_DMA_Transmit(uint8_t *pData, uint16_t Size);

/**
 * @brief UART1 DMA发送等待完成（阻塞方式）
 * @param timeout_ms 超时时间（毫秒）
 * @return 1表示成功，0表示超时
 */
uint8_t UART1_DMA_Wait_Complete(uint32_t timeout_ms);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

