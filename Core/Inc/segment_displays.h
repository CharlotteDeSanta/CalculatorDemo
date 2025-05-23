
// /**
//  * @file segment_displays.h
//  * @brief
//  * @author CharlotteDeSanta
//  * @date 25-5-24 上午12:11
//  *
//  * @copyright Copyright (c) 2025 CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

#ifndef SEGMENT_DISPLAYS_H
#define SEGMENT_DISPLAYS_H

#include "main.h"

// 数码管显示位数
#define DISPLAY_DIGITS 6
// 显示缓存
static uint8_t DisplayBuffer[DISPLAY_DIGITS] = {0};

// 初始化显示
void Display_Init(void);
// 设置显示数字
void Display_Number(float num);
// 显示扫描函数(需要在定时器中断中调用)
void Display_Scan(void);
// 清空显示
void Display_Clear(void);

#endif