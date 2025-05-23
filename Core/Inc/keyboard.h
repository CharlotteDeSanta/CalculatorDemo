
// /**
//  * @file keyboard.h
//  * @brief
//  * @author CharlotteDeSanta
//  * @date 25-5-24 上午12:11
//  *
//  * @copyright Copyright (c) 2025 CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "main.h"

// 按键回调函数类型
typedef void (*KeyCallback)(uint8_t key);

// 初始化键盘
void Keyboard_Init(void);
// 设置按键回调
void Keyboard_SetCallback(KeyCallback callback);
// 键盘扫描(在中断中调用)
void Keyboard_Scan(void);

#endif