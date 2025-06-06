// /**
//  * @file keyboard.c
//  * @brief
//  * @author CharlotteDeSanta
//  * @date 25-6-6 上午10:48
//  *
//  * @copyright Copyright (c) 2025 CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

#include "keyboard.h"

static KeyCallback keyCallback = NULL;

void Keyboard_Init(void) {
    // GPIO初始化已在main.c中完成
}

void Keyboard_SetCallback(KeyCallback callback) {
    keyCallback = callback;
}

void Keyboard_Scan(void)
{
    static uint8_t col = 0;
    static const uint16_t colPins[4] = {KEY_COL1_Pin, KEY_COL2_Pin, KEY_COL3_Pin, KEY_COL4_Pin};
    static const uint16_t rowPins[4] = {KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin, KEY_ROW4_Pin};
    static const uint8_t KeyMap[4][4] = {
        {'1', '2', '3', '+'},
        {'4', '5', '6', '-'},
        {'7', '8', '9', '*'},
        {'.', '0', '=', '/'}
    };

    // 所有列先置高电平
    for (int i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(GPIOB, colPins[i], GPIO_PIN_SET);
    }

    // 当前列拉低
    HAL_GPIO_WritePin(GPIOB, colPins[col], GPIO_PIN_RESET);

    // 扫描行
    for (int row = 0; row < 4; row++)
    {
        if (HAL_GPIO_ReadPin(GPIOB, rowPins[row]) == GPIO_PIN_RESET)
        {
            static uint32_t lastTick = 0;
            if (HAL_GetTick() - lastTick > 150)
            {
                // 简单消抖
                lastTick = HAL_GetTick();
                if (keyCallback) keyCallback(KeyMap[row][col]);
            }
        }
    }

    // 下一列
    col = (col + 1) % 4;
}