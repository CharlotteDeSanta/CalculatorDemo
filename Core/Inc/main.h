/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SEG_A0_Pin GPIO_PIN_4
#define SEG_A0_GPIO_Port GPIOA
#define SEG_A1_Pin GPIO_PIN_5
#define SEG_A1_GPIO_Port GPIOA
#define SEG_A2_Pin GPIO_PIN_6
#define SEG_A2_GPIO_Port GPIOA
#define SEG_E3_Pin GPIO_PIN_7
#define SEG_E3_GPIO_Port GPIOA
#define KEY_COL1_Pin GPIO_PIN_12
#define KEY_COL1_GPIO_Port GPIOB
#define KEY_COL2_Pin GPIO_PIN_13
#define KEY_COL2_GPIO_Port GPIOB
#define KEY_COL3_Pin GPIO_PIN_14
#define KEY_COL3_GPIO_Port GPIOB
#define KEY_COL4_Pin GPIO_PIN_15
#define KEY_COL4_GPIO_Port GPIOB
#define KEY_ROW1_Pin GPIO_PIN_4
#define KEY_ROW1_GPIO_Port GPIOB
#define KEY_ROW1_EXTI_IRQn EXTI4_IRQn
#define KEY_ROW2_Pin GPIO_PIN_5
#define KEY_ROW2_GPIO_Port GPIOB
#define KEY_ROW2_EXTI_IRQn EXTI9_5_IRQn
#define KEY_ROW3_Pin GPIO_PIN_6
#define KEY_ROW3_GPIO_Port GPIOB
#define KEY_ROW3_EXTI_IRQn EXTI9_5_IRQn
#define KEY_ROW4_Pin GPIO_PIN_7
#define KEY_ROW4_GPIO_Port GPIOB
#define KEY_ROW4_EXTI_IRQn EXTI9_5_IRQn
#define SEG_CLK_Pin GPIO_PIN_8
#define SEG_CLK_GPIO_Port GPIOB
#define SEG_DATA_Pin GPIO_PIN_9
#define SEG_DATA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
