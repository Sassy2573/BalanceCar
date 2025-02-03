#ifndef __DRI_USART_H
#define __DRI_USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h> // 可变参数

extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

void Driver_USART3_Init(void);

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);

void Serial_SendPacket(void);

#endif
