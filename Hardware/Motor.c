/*
 * @Author: Linhart && git chenyinlin0429@gmail.com
 * @Date: 2025-01-04 12:02:37
 * @LastEditors: Linhart && chenyinlin0429@gmail.com
 * @LastEditTime: 2025-01-04 13:19:58
 * @FilePath: \BalenceCar\Hardware\Motor.c
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#include "Motor.h"
#include "PWM.h"

void Motor_GPIO_INit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 使能 GPIOB 时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_14;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_14);
}

void Motor_SetSpeed(uint8_t motor, int16_t speed)
{
    if (motor == 1)
    {
        if (speed > 0)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_15);
            GPIO_ResetBits(GPIOB, GPIO_Pin_14);
            SetPWM(1, speed);
        } else if (speed < 0)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_15);
            GPIO_SetBits(GPIOB, GPIO_Pin_14);
            SetPWM(1, -speed);
        } else 
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
            SetPWM(2, 0);
        }
    } else if (motor == 2)
    {
        if (speed > 0)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_12);
            GPIO_ResetBits(GPIOB, GPIO_Pin_13);
            SetPWM(2, speed);
        } else if (speed < 0)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_12);
            GPIO_SetBits(GPIOB, GPIO_Pin_13);
            SetPWM(2, -speed);
        } else 
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
            SetPWM(2, 0);
        }
    }
}
