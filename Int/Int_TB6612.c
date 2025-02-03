#include "Int_TB6612.h"


/**
 * @description: TB6612初始化
 * @return {*}
 */
void Int_TB6612_Init(void)
{
    /* 初始化定时器 */
    Dri_TIM2_Init();

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; /* 开启GPIOB时钟 */
    /* 初始化GPIO */
    GPIOB->CRH |= (GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 |GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);
}

/**
 * @description: 控制电机A的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorA(uint8_t direct)
{
    if (direct == Go)
    {
        /* 正转 AIN1 = 0， AIN2 = 1 */
        AIN1_L;
        AIN2_H;
    }
    else if (direct == Back)
    {
        /* 反转 AIN1 = 1， AIN2 = 0 */
        AIN1_H;
        AIN2_L;
    }
    else if (direct == Stop)
    {
        /* 停止 AIN1 = 1， AIN2 = 1 */
        AIN1_H;
        AIN2_H; 
    }
}

/**
 * @description: 控制电机B的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorB(uint8_t direct)
{
    if (direct == Go)
    {
        /* 正转 BIN1 = 0， BIN2 = 1 */
        BIN1_L;
        BIN2_H;
    }
    else if (direct == Back)
    {
        /* 反转 BIN1 = 1， BIN2 = 0 */
        BIN1_H;
        BIN2_L;
    }
    else if (direct == Stop)
    {
        /* 停止 BIN1 = 1， BIN2 = 1 */
        BIN1_H;
        BIN2_H; 
    }
}

/**
 * @description: 根据带符号的PWM值，设置转速和方向
 * @param {int} pwma A电机
 * @param {int} pwmb B电机
 * @return {*}
 */
void Int_TB6612_SetPWM(int pwma, int pwmb)
{
    if (pwma > 0)
    {
        /* 正转 */
        Int_TB6612_MotorA(Go);
    }
    else if (pwma < 0)
    {
        /* 反转 */
        Int_TB6612_MotorA(Back);
    }else
    {
        /* 停止 */
        Int_TB6612_MotorA(Stop);
    }
    TIM2->CCR3 = abs(pwma); // 左电机

    if (pwmb > 0)
    {
        /* 正转 */
        Int_TB6612_MotorB(Go);
    }
    else if (pwmb < 0)
    {
        /* 反转 */
        Int_TB6612_MotorB(Back);
    }else
    {
        /* 停止 */
        Int_TB6612_MotorB(Stop);
    }
    TIM2->CCR4 = abs(pwmb); // 右电机

    
}
