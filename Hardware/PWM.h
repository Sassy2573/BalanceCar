/*
 * @Author: Linhart && git chenyinlin0429@gmail.com
 * @Date: 2025-01-03 22:55:49
 * @LastEditors: Linhart && git chenyinlin0429@gmail.com
 * @LastEditTime: 2025-01-04 11:31:36
 * @FilePath: \BalenceCar\Hardware\PWM.h
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
void SetPWM(uint8_t channel, uint16_t duty_cycle);

#endif
