#include "Int_Encoder.h"

/**
 * @description: 初始化
 * @return {*}
 */
void Int_Encoder_Init(void)
{
    /* 初始化定时器为编码器模式 */
    Dri_TIM3_Init();
    Dri_TIM4_Init();
}

/**
 * @description: 读取编码器的计数值，处理成带符号的值
 * @param {uint8_t} timx 要读取的定时器的编号
 * @return {*} 读到的值
 */
int Int_Encoder_ReadCounter(uint8_t timx)
{
    int encoder_value = 0;
    switch (timx)
    {
    case 3:
    { /* 读取定时器3的计数值 */
        encoder_value = (int16_t)TIM3->CNT;
        TIM3->CNT = 0;
        break;
    }

    case 4:
    {
        /* 读取定时器4的计数值 */
        encoder_value = (int16_t)TIM4->CNT;
        TIM4->CNT = 0;
        break;
    }
    default:
        break;
    }

    /* 由于电机是对称放置的后续读速度都要注意符号 */
    return encoder_value;
}
