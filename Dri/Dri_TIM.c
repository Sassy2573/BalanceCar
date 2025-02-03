#include "Dri_TIM.h"

/**
 * @description: 定时器2初始化：CH3、CH4输出PWM
 * @return {*}
 */
void Dri_TIM2_Init(void)
{
    /** 使能TIM2和GPIOA时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* 开启TIM2时钟（APB1总线） */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; /* 开启GPIOA时钟（APB2总线） */

    /** 配置PA2和PA3为复用推挽输出 */
    GPIOA->CRL &= ~((0xF << (2 * 4)) | (0xF << (3 * 4))); // 清除 PA2 和 PA3 的配置
    GPIOA->CRL |= (0xB << (2 * 4)) | (0xB << (3 * 4));    // 设置 PA2 和 PA3 为复用推挽输出，50MHz

    /** 配置TIM2基本参数 */
    TIM2->CR1 &= ~TIM_CR1_CEN; /* 关闭定时器 */
    TIM2->PSC = 1 - 1;         /* 预分频值 = 0（不分频） */
    TIM2->ARR = 7200 - 1;      /* 自动重装载值（周期=ARR+1） */
    TIM2->CR1 &= ~TIM_CR1_DIR; /* 向上计数模式 */

    /** 配置TIM2通道3和通道4为PWM输出模式 */
    TIM2->CCR3 = 0;                 /* 通道3占空比 */
    TIM2->CCR4 = 0;                 /* 通道4占空比 */
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S; /* 通道3配置为输出 */
    TIM2->CCMR2 &= ~TIM_CCMR2_CC4S; /* 通道4配置为输出 */

    TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2; /* 通道3 PWM模式1 */
    TIM2->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2; /* 通道4 PWM模式1 */

    TIM2->CCER &= ~TIM_CCER_CC3P; /* 通道3输出极性 */
    TIM2->CCER &= ~TIM_CCER_CC4P; /* 通道4输出极性 */

    TIM2->CCER |= TIM_CCER_CC3E; /* 使能通道3输出 */
    TIM2->CCER |= TIM_CCER_CC4E; /* 使能通道4输出 */

    TIM2->CR1 |= TIM_CR1_CEN; /* 开启定时器 */
}

/**
 * @description: 初始化为编码器模式（A电机）
 * @return {*}
 */
void Dri_TIM3_Init(void)
{
    /* 时钟、GPIO、定时器通用、 编码器模式 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 清空 PA6、PA7 配置 */
    GPIOA->CRL &= ~GPIO_CRL_MODE6; // 清空 PA6 模式位
    GPIOA->CRL &= ~GPIO_CRL_CNF6;  // 清空 PA6 配置位
    GPIOA->CRL |= GPIO_CRL_CNF6_0; // 设置 PA6 为浮空输入

    GPIOA->CRL &= ~GPIO_CRL_MODE7; // 清空 PA7 模式位
    GPIOA->CRL &= ~GPIO_CRL_CNF7;  // 清空 PA7 配置位
    GPIOA->CRL |= GPIO_CRL_CNF7_0; // 设置 PA7 为浮空输入

    /* 定时器基本通用配置 */
    TIM3->PSC = 1 - 1;
    TIM3->ARR = 65536 - 1;

    /* 配置编码器模式 */
    /* 配置输入通道映射IC1-TI1 IC2-TI2*/
    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;

    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S_1;
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;

    /* 配置不反向 */
    TIM3->CCER &= ~TIM_CCER_CC1P;
    TIM3->CCER &= ~TIM_CCER_CC2P;

    /* 两路信号都计数，编码器模式3, SMS=011 */
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;
    TIM3->SMCR |= TIM_SMCR_SMS_1;
    TIM3->SMCR |= TIM_SMCR_SMS_0;

    /* 启动定时器 */
    TIM3->CR1 |= TIM_CR1_CEN;
}


/**
 * @description: 初始化为编码器模式（B电机）
 * @return {*}
 */
void Dri_TIM4_Init(void)
{
    /* 时钟、GPIO、定时器通用、 编码器模式 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 清空 PB6、PB7 配置 */
    GPIOB->CRL &= ~GPIO_CRL_MODE6; // 清空 PB6 模式位
    GPIOB->CRL &= ~GPIO_CRL_CNF6;  // 清空 PB6 配置位
    GPIOB->CRL |= GPIO_CRL_CNF6_0; // 设置 PB6 为浮空输入

    GPIOB->CRL &= ~GPIO_CRL_MODE7; // 清空 PB7 模式位
    GPIOB->CRL &= ~GPIO_CRL_CNF7;  // 清空 PB7 配置位
    GPIOB->CRL |= GPIO_CRL_CNF7_0; // 设置 PB7 为浮空输入

    /* 定时器基本通用配置 */
    TIM4->PSC = 1 - 1;
    TIM4->ARR = 65536 - 1;

    /* 配置编码器模式 */
    /* 配置输入通道映射 IC1-TI1 IC2-TI2 */
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_0;

    /* 配置不反向 */
    TIM4->CCER &= ~TIM_CCER_CC1P; 
    TIM4->CCER &= ~TIM_CCER_CC2P;

    /* 两路信号都计数，编码器模式3, SMS=011 */
    TIM4->SMCR &= ~TIM_SMCR_SMS_2;
    TIM4->SMCR |= TIM_SMCR_SMS_1;
    TIM4->SMCR |= TIM_SMCR_SMS_0;

    /* 启动定时器 */
    TIM4->CR1 |= TIM_CR1_CEN;
}
