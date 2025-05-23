
// /**
//  * @file keyboard.c
//  * @brief
//  * @author CharlotteDeSanta
//  * @date 25-5-24 上午12:11
//  *
//  * @copyright Copyright (c) 2025 CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

#include "keyboard.h"

static KeyCallback keyCallback = NULL;
static uint32_t lastPressTime = 0;  // 上次按键时间
static uint8_t lastKey = 0;         // 上次按键值
static const uint32_t DEBOUNCE_TIME = 100;    // 消抖时间(ms)
static const uint32_t REPEAT_TIME = 300;      // 重复按键延时(ms)
static uint8_t keyPressed = 0;      // 按键按下标志

// 键盘映射表
static const uint8_t KeyMap[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'.', '0', '=', '/'}
};

void Keyboard_Init(void) {
    // GPIO初始化已在main.c中完成
}

void Keyboard_SetCallback(KeyCallback callback) {
    keyCallback = callback;
}

static uint8_t GetPressedKey(void) {
    static const uint16_t colPins[4] = {KEY_COL1_Pin, KEY_COL2_Pin, KEY_COL3_Pin, KEY_COL4_Pin};
    static const uint16_t rowPins[4] = {KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin, KEY_ROW4_Pin};

    // 将所有列设置为高电平
    for(int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(GPIOB, colPins[i], GPIO_PIN_SET);
    }

    // 扫描所有列
    for(int col = 0; col < 4; col++) {
        // 当前列设置为低电平
        HAL_GPIO_WritePin(GPIOB, colPins[col], GPIO_PIN_RESET);

        // 短暂延时确保信号稳定
        for(volatile int i = 0; i < 10; i++);

        // 检查每一行
        for(int row = 0; row < 4; row++) {
            if(HAL_GPIO_ReadPin(GPIOB, rowPins[row]) == GPIO_PIN_RESET) {
                // 恢复当前列为高电平
                HAL_GPIO_WritePin(GPIOB, colPins[col], GPIO_PIN_SET);
                return KeyMap[row][col];
            }
        }

        // 恢复当前列为高电平
        HAL_GPIO_WritePin(GPIOB, colPins[col], GPIO_PIN_SET);
    }

    return 0;  // 没有按键按下
}

void Keyboard_Scan(void) {
    uint32_t currentTime = HAL_GetTick();
    uint8_t key = GetPressedKey();

    // 有按键按下
    if(key != 0) {
        if(!keyPressed) {  // 新的按键按下
            if(currentTime - lastPressTime >= DEBOUNCE_TIME) {
                keyPressed = 1;
                lastKey = key;
                lastPressTime = currentTime;
                if(keyCallback) keyCallback(key);
            }
        } else {  // 按键持续按下
            if(key == lastKey && currentTime - lastPressTime >= REPEAT_TIME) {
                lastPressTime = currentTime;
                if(keyCallback) keyCallback(key);
            }
        }
    } else {  // 没有按键按下
        keyPressed = 0;
        lastKey = 0;
    }
}

// 中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin >= GPIO_PIN_4 && GPIO_Pin <= GPIO_PIN_7) {
        // 在中断中只触发一次扫描，不直接调用回调
        Keyboard_Scan();
    }
}