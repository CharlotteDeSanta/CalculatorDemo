
// /**
//  * @file ${FILE_NAME}
//  * @brief
//  * @author CharlotteDeSanta
//  * @date ${YEAR}
//  *
//  * @copyright Copyright (c) ${YEAR} CharlotteDeSanta. All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  *

#include "segment_displays.h"

// 数码管段码表（共阴极）
static const uint8_t NumTable[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static uint8_t CurrentDigit = 0;

void Display_Init(void) {
    // 确保所有控制引脚初始状态正确
    HAL_GPIO_WritePin(GPIOA, SEG_E3_Pin, GPIO_PIN_RESET);  // 关闭显示使能
    HAL_GPIO_WritePin(GPIOA, SEG_A0_Pin|SEG_A1_Pin|SEG_A2_Pin, GPIO_PIN_RESET);  // 清除位选
    HAL_GPIO_WritePin(GPIOB, SEG_CLK_Pin|SEG_DATA_Pin, GPIO_PIN_RESET);  // 清除时钟和数据线

    Display_Clear();  // 清空显示缓存

    // 设置初始显示为0
    DisplayBuffer[DISPLAY_DIGITS-1] = NumTable[0];
}

void Display_Number(float num) {
    // 先清空显示缓存
    Display_Clear();

    // 处理负数
    if(num < 0) {
        DisplayBuffer[5] = 0x40; // 显示负号在最左边
        num = -num;
    }

    // 分离整数和小数部分
    int intPart = (int)num;
    float fracPart = num - intPart;

    // 处理整数部分
    if(intPart == 0) {
        DisplayBuffer[0] = NumTable[0];  // 至少显示一个0
    } else {
        int pos = 0;
        while(intPart > 0 && pos < DISPLAY_DIGITS) {
            DisplayBuffer[pos++] = NumTable[intPart % 10];
            intPart /= 10;
        }

        // 反转整数部分
        for(int i = 0; i < pos/2; i++) {
            uint8_t temp = DisplayBuffer[i];
            DisplayBuffer[i] = DisplayBuffer[pos-1-i];
            DisplayBuffer[pos-1-i] = temp;
        }
    }

    // 处理小数部分
    if(fracPart > 0.0001f) {  // 考虑浮点数精度误差
        // 找到整数部分最后一位的位置
        int intDigits = 0;
        while(intDigits < DISPLAY_DIGITS && DisplayBuffer[intDigits] != 0) {
            intDigits++;
        }
        if(intDigits == 0) intDigits = 1;  // 如果是0.xxx的情况

        // 添加小数点
        DisplayBuffer[intDigits-1] |= 0x80;

        // 处理小数部分（最多显示两位）
        fracPart = fracPart * 100 + 0.5f;  // 保留两位小数并四舍五入
        int decimalPart = (int)fracPart;

        // 显示小数部分
        if(decimalPart > 0 && intDigits < DISPLAY_DIGITS-1) {
            // 十位
            if(decimalPart >= 10) {
                DisplayBuffer[intDigits] = NumTable[decimalPart / 10];
                if(intDigits + 1 < DISPLAY_DIGITS) {
                    DisplayBuffer[intDigits + 1] = NumTable[decimalPart % 10];
                }
            } else {
                DisplayBuffer[intDigits] = NumTable[decimalPart];
            }
        }
    }
}

void Display_Scan(void) {
    // 关闭显示
    HAL_GPIO_WritePin(GPIOA, SEG_E3_Pin, GPIO_PIN_RESET);

    // 设置新的位选（不需要取反）
    HAL_GPIO_WritePin(GPIOA, SEG_A0_Pin, (CurrentDigit & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, SEG_A1_Pin, (CurrentDigit & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, SEG_A2_Pin, (CurrentDigit & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // 移位输出段码
    for(int i = 7; i >= 0; i--) {  // 从高位开始移位
        HAL_GPIO_WritePin(GPIOB, SEG_DATA_Pin, (DisplayBuffer[CurrentDigit] >> i) & 0x01);
        HAL_GPIO_WritePin(GPIOB, SEG_CLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, SEG_CLK_Pin, GPIO_PIN_RESET);
    }

    // 使能显示
    HAL_GPIO_WritePin(GPIOA, SEG_E3_Pin, GPIO_PIN_SET);

    // 更新位计数
    CurrentDigit = (CurrentDigit + 1) % DISPLAY_DIGITS;
}

void Display_Clear(void) {
    for(int i = 0; i < DISPLAY_DIGITS; i++) {
        DisplayBuffer[i] = 0;
    }
}