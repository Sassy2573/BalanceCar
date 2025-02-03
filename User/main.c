#include "main.h"
#include "freertos.h"
#include "task.h"
#include "Int_TB6612.h"
#include "Int_Encoder.h"
#include "Dri_USART3.h"
#include "Int_MPU6050.h"
#include "App_Task.h"

void for_delay_ms(uint32_t ms)
{
	uint32_t Delay = ms * 72000 / 9;
	do
	{
		__NOP();
	} while (Delay--);
}

int main(void)
{
	Driver_USART3_Init();

	Int_TB6612_Init();

	Int_Encoder_Init();

	Int_MPU6050_Init();

	App_Task_Init();

	// Int_TB6612_SetPWM(3600, 3600); // A左轮， B右轮

	while (1)
	{
		/* 测速编码器读取值 */
		// Serial_Printf("tim3 cnt= %d\r\n", Int_Encoder_ReadCounter(3));

		// Serial_Printf("tim4 cnt= %d\r\n", Int_Encoder_ReadCounter(4)); 
		// for_delay_ms(1000);

		/* 测速MPU6050 */
		// Int_MPU6050_Get_Gyro(&gx, &gy, &gz);
		// Int_MPU6050_Get_Accel(&ax, &ay, &az);
		// Serial_Printf("gx = %d, gy = %d, gz = %d\r\n", gx, gy, gz);
		// Serial_Printf("ax = %d, ay = %d, az = %d\r\n", ax, ay, az);
		// for_delay_ms(100);
	}
}

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}
}
