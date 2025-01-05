#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "PWM.h"
#include "Delay.h"
#include "Serial.h"

uint8_t RxData; // 串口接收数据

int main(void)
{
	Motor_GPIO_INit();	// 初始化电机
	PWM_Init();	// 初始化PWM
	Serial_Init();	// 初始化串口

	
	while(1)
	{
		// Motor_SetSpeed(1, 500);
		// Motor_SetSpeed(2, 500);
		// Delay_ms(10000);

		// Motor_SetSpeed(1, -500);
		// Motor_SetSpeed(2, -500);
		// Delay_ms(10000);

		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData(); //	获取串口接收的数据
			Serial_SendByte(RxData);	//	发送接收到的数据
			Serial_SendByte(RxData+1);
		}
		
	}
}
