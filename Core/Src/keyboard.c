// /**
//  * @file keyboard.c
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
#include "keyboard.h"

static KeyCallback keyCallback = NULL;
static uint8_t currentCol = 0;

static const uint16_t colPins[4] = {KEY_COL1_Pin, KEY_COL2_Pin, KEY_COL3_Pin, KEY_COL4_Pin};
static const uint16_t rowPins[4] = {KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin, KEY_ROW4_Pin};
static const uint8_t KeyMap[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'.', '0', '=', '/'}
};

void Keyboard_Init(void)
{
    // GPIO初始化已在main.c中完成
}

void Keyboard_SetCallback(KeyCallback callback)
{
    keyCallback = callback;
}

void Keyboard_ColumnScan(void)
{
    // 所有列拉高
    for (int i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(GPIOB, colPins[i], GPIO_PIN_SET);
    }
    // 当前列拉低
    HAL_GPIO_WritePin(GPIOB, colPins[currentCol], GPIO_PIN_RESET);
}

void Keyboard_NextColumn(void)
{
    currentCol = (currentCol + 1) % 4;
}

uint8_t Keyboard_GetKey(void)
{
    static uint8_t lastKey = 0;
    static uint32_t lastTick = 0;

    for (int row = 0; row < 4; row++)
    {
        if (HAL_GPIO_ReadPin(GPIOB, rowPins[row]) == GPIO_PIN_RESET)
        {
            uint8_t key = KeyMap[row][currentCol];
            uint32_t now = HAL_GetTick();
            if (key != lastKey || (now - lastTick > 250))
            {
                lastKey = key;
                lastTick = now;
                return key;
            }
            else
            {
                return 0; // 消抖：忽略重复
            }
        }
    }
    return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin >= GPIO_PIN_4 && GPIO_Pin <= GPIO_PIN_7)
    {
        uint8_t key = Keyboard_GetKey();
        if (key && keyCallback) {
            keyCallback(key);
        }
    }
}