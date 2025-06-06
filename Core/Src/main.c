// /**
//  * @file main.c
//  * @brief
//  * @author CharlotteDeSanta
//  * @date 25-6-6 下午11:13
//  *
//  * @copyright Copyright (c) 2025 CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

/* USER CODE BEGIN Header */
/* USER CODE END Header */
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include <math.h>
#include "segment_displays.h"
#include "keyboard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
float num1 = 0, num2 = 0;
char operator = 0;
uint8_t inputState = 0;
uint8_t decimal = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

void Calculator_KeyPress(uint8_t key);

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
  HAL_TIM_Base_Start_IT(&htim2);

  Display_Init();
  Display_Number(0);
  Keyboard_Init();
  Keyboard_SetCallback(Calculator_KeyPress);

  while (1)
  {
    // 主循环不处理键盘扫描，由定时器+中断完成
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, SEG_A0_Pin|SEG_A1_Pin|SEG_A2_Pin|SEG_E3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, KEY_COL1_Pin|KEY_COL2_Pin|KEY_COL3_Pin|KEY_COL4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, SEG_CLK_Pin|SEG_DATA_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = SEG_A0_Pin|SEG_A1_Pin|SEG_A2_Pin|SEG_E3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = KEY_COL1_Pin | KEY_COL2_Pin | KEY_COL3_Pin | KEY_COL4_Pin | SEG_CLK_Pin | SEG_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = KEY_ROW1_Pin | KEY_ROW2_Pin | KEY_ROW3_Pin | KEY_ROW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    Keyboard_NextColumn();
    Keyboard_ColumnScan();
    Display_Scan();
  }
}

void Calculator_KeyPress(uint8_t key)
{
  static uint8_t isFirstKey = 1;
  static int decimalCount = 0;

  if (isFirstKey) {
    num1 = 0;
    isFirstKey = 0;
  }

  if (key >= '0' && key <= '9')
  {
    if (inputState == 0) {
      if (!decimal) {
        num1 = num1 * 10 + (key - '0');
      } else {
        decimalCount++;
        float value = (key - '0');
        for (int i = 0; i < decimalCount; i++) {
          value /= 10.0f;
        }
        num1 = num1 + value;
      }
      Display_Number(num1);
    } else {
      if (!decimal) {
        num2 = num2 * 10 + (key - '0');
      } else {
        decimalCount++;
        float value = (key - '0');
        for (int i = 0; i < decimalCount; i++) {
          value /= 10.0f;
        }
        num2 = num2 + value;
      }
      Display_Number(num2);
    }
  }
  else if (key == '.')
  {
    if (decimal == 0) {
      decimal = 1;
      decimalCount = 0;
    }
  }
  else if (key == '+' || key == '-' || key == '*' || key == '/')
  {
    operator = key;
    inputState = 1;
    decimal = 0;
    decimalCount = 0;
  }
  else if (key == '=')
  {
    float result = 0;
    switch (operator)
    {
    case '+': result = num1 + num2;
      break;
    case '-': result = num1 - num2;
      break;
    case '*': result = num1 * num2;
      break;
    case '/': result = num2 != 0 ? num1 / num2 : 0;
      break;
    default: result = inputState == 0 ? num1 : num2;
      break;
    }
    Display_Number(result);
    num1 = result;
    num2 = 0;
    operator = 0;
    inputState = 0;
    decimal = 0;
    decimalCount = 0;
  }
}