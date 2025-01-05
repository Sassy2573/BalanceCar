/*
 * @Author: Linhart && chenyinlin0429@gmail.com
 * @Date: 2025-01-03 22:55:37
 * @LastEditors: Linhart && chenyinlin0429@gmail.com
 * @LastEditTime: 2025-01-04 13:01:33
 * @FilePath: \BalenceCar\Hardware\PWM.c
 * @Description: 
 * 
 * Copyright (c) 2025 by chenyinlin0429@gmail.com, All Rights Reserved. 
 */
#include "stm32f10x.h"
#include "PWM.h"


/* 初始化PWM信号 */
void RCC_Configuration(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // 使能 TIM2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能 GPIOA 时钟
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  // 设置 PA2 和 PA3 为复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM_Configuration(void)
{
    TIM_InternalClockConfig(TIM2);  // 设置 TIM2 时钟源为内部时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Period = 1000;  // 设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = 0; // 设置时钟预分频数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 设置计数器模式为向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  // 初始化 TIM2

    TIM_OCStructInit(&TIM_OCInitStructure);  // 初始化 TIM_OCInitStructure 结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  // 配置为 PWM 模式 1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;  // 设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // 输出极性高

    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  // 初始化外设 TIM2 OC1
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能 TIM2 在 CCR1 上的预装载寄存器

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);  // 初始化外设 TIM2 OC2
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能 TIM2 在 CCR2 上的预装载寄存器

    TIM_ARRPreloadConfig(TIM2, ENABLE);  // 使能 TIM2 在 ARR 上的预装载寄存器
    TIM_CtrlPWMOutputs(TIM2, ENABLE); // 使能 TIM1 定时器的PWM输出控制(高级定时器专属)

    TIM_Cmd(TIM2, ENABLE);  // 使能 TIM2
}

void PWM_Init(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    TIM_Configuration();
}

void SetPWM(uint8_t channel, uint16_t duty_cycle)
{
    if (duty_cycle > 1000)
    {
        duty_cycle = 1000;
    }
    if (channel ==1)
    {
        TIM_SetCompare3(TIM2, duty_cycle);
    } else if (channel == 2)
    {
        TIM_SetCompare4(TIM2, duty_cycle);
    }
    
}
