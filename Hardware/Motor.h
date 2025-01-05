/*
 * @Author: Linhart && git chenyinlin0429@gmail.com
 * @Date: 2025-01-04 12:02:44
 * @LastEditors: Linhart && chenyinlin0429@gmail.com
 * @LastEditTime: 2025-01-04 12:16:17
 * @FilePath: \BalenceCar\Hardware\Motor.h
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void Motor_GPIO_INit(void);
void Motor_SetSpeed(uint8_t motor, int16_t speed);



#endif
