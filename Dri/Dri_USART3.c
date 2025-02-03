#include "Dri_USART3.h"

char Serial_RxPacket[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t Serial_RxFlag;					//定义接收数据包标志位

/**
 * @description: 初始化USART3用于串口
 * @return {*}
 */
void Driver_USART3_Init(void)
{
    /* 1. 开启 USART3 和 GPIOB 时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  // 使能 USART3 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;    // 使能 GPIOB 时钟

    /* 2. 配置 GPIOB10 (TX) 为复用推挽输出 */
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10); // 清除原有设置
    GPIOB->CRH |= (GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0); // 50MHz 输出速度
    GPIOB->CRH |= GPIO_CRH_CNF10_1; // 复用推挽模式

    /* 3. 配置 GPIOB11 (RX) 为上拉输入 */
    GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11); // 清除原有设置
    GPIOB->CRH |= GPIO_CRH_CNF11_0;  // 输入模式，带上拉
    GPIOB->ODR |= GPIO_ODR_ODR11;    // 使能上拉

    /* 4. 配置 USART3 */
    USART3->BRR = 36000000 / 38400;   // 计算波特率
    USART3->CR1 &= ~USART_CR1_M;      // 8位数据位
    USART3->CR1 &= ~USART_CR1_PCE;    // 无校验
    USART3->CR2 &= ~USART_CR2_STOP;   // 1位停止位
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE; // 使能发送和接收

    /* 5. 使能 USART3 接收中断 */
    USART3->CR1 |= USART_CR1_RXNEIE;  // 使能接收中断

    /* 6. 配置 NVIC */
    NVIC_SetPriorityGrouping(2);
    NVIC_SetPriority(USART3_IRQn, 1);  // 设置中断优先级
    NVIC_EnableIRQ(USART3_IRQn);       // 使能 USART3 中断

    /* 7. 使能 USART3 */
    USART3->CR1 |= USART_CR1_UE;  // 使能串口

}

/**
 * @description: 串口发送一个字节
 * @param {uint8_t} Byte
 * @return {*}
 */
void Serial_SendByte(uint8_t Byte)
{   
    USART3->DR = Byte;
    while (!(USART3->SR & USART_SR_TXE)); // 等待发送完成(TXE = 1)
}

/**
 * @description: 串口发送一个数组
 * @param {uint8_t} *Array
 * @param {uint16_t} Length
 * @return {*}
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    for (uint16_t i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }    
}

/**
 * @description: 串口发送一个字符串
 * @param {char} *String
 * @return {*}
 */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
 * @description: 次方函数（内部使用）
 * @param {uint32_t} X
 * @param {uint32_t} Y
 * @return {*}
 */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}


/**
 * @description: 串口发送数字
 * @param {uint32_t} Number 要发送的数字，范围：0~4294967295
 * @param {uint8_t} Length  要发送数字的长度，范围：0~10
 * @return {*}
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}


/**
 * @description: 使用printf需要重定向的底层函数
 * @param {int} ch
 * @param {FILE} *f
 * @return {*}
 */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

/**
 * @description: 自己封装的prinf函数
 * @param {char} *format
 * @return {*}
 */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}

/**
 * @description: 获取串口接收数据包标志位
 * @return {*} 接受到数据包后标志位置1
 */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}


/**
 * @description: USART中断函数
 * @return {*}
 */
void USART3_IRQHandler(void)
{
    static uint8_t RxState = 0;  // 状态机状态
    static uint8_t pRxPacket = 0; // 当前接收数据位置

    if (USART3->SR & USART_SR_RXNE)  // 判断 USART3 是否触发接收中断
    {
        uint8_t RxData = USART3->DR; // 读取数据寄存器，清除 RXNE 标志位

        /* 状态机处理数据包 */
        
        if (RxState == 0)  // 处理包头
        {
            if (RxData == '@' && Serial_RxFlag == 0)  // 检测包头，确保上个数据包已处理完
            {
                RxState = 1;  // 进入下一状态
                pRxPacket = 0; // 位置归零
            }
        }
        else if (RxState == 1)  // 处理数据包内容
        {
            if (RxData == '\r')  // 判断是否收到第一个包尾
            {
                RxState = 2;
            }
            else  // 正常数据存入缓冲区
            {
                Serial_RxPacket[pRxPacket] = RxData;
                pRxPacket++;
            }
        }
        else if (RxState == 2)  // 处理第二个包尾
        {
            if (RxData == '\n')  // 如果收到第二个包尾
            {
                RxState = 0;  // 归零状态
                Serial_RxPacket[pRxPacket] = '\0';  // 添加字符串结束符
                Serial_RxFlag = 1;  // 标记数据包接收完成
            }
        }
    }
}
