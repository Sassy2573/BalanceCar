#include "Int_MPU6050.h"

/**
 * @description: 读取指定寄存器一个字节数据
 * @param {uint8_t} reg_addr
 * @param {uint8_t} *receive_byte
 * @return {*}
 */
uint8_t Int_MPU6050_ReadByte(uint8_t reg_addr, uint8_t *receive_byte)
{
    /* 1.起始信号 */
    Driver_I2C1_Start();
    /* 2.AD+W  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x00);
    /* 3.发送 寄存器的地址 */
    Driver_I2C_SendByte(reg_addr);
    /* 4.起始信号 */
    Driver_I2C1_Start();
    /* 5.AD+R  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x01);
    /* 6.先准备NACK和停止信号 */
    Driver_I2C1_NAck();
    Driver_I2C1_Stop();
    /* 7.读取一字节数据 */
    *receive_byte = Driver_I2C_ReadByte();

    return 0;
}

/**
 * @description: 读取指定寄存器多个字节数据
 * @param {uint8_t} reg_addr
 * @param {uint8_t} *receive_buff
 * @param {uint8_t} size
 * @return {*}
 */
uint8_t Int_MPU6050_ReadBytes(uint8_t reg_addr, uint8_t *receive_buff, uint8_t size)
{
    /* 1.起始信号 */
    Driver_I2C1_Start();
    /* 2.AD+W  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x00);
    /* 3.发送 寄存器的地址 */
    Driver_I2C_SendByte(reg_addr);
    /* 4.起始信号 */
    Driver_I2C1_Start();
    /* 5.AD+R  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x01);

    /* 6.连续读取多个字节 */
    for (uint8_t i = 0; i < size; i++)
    {
        if (i < size - 1)
        {
            /* 不是最后一个字节，提前准备ack */
            Driver_I2C1_Ack();
        }
        else
        {
            /* 是最后一个字节，先准备NACK和停止信号 */
            Driver_I2C1_NAck();
            Driver_I2C1_Stop();
        }

        receive_buff[i] = Driver_I2C_ReadByte();
    }

    return 0;
}

/**
 * @description: 向MPU6050写入一个字节
 * @param {uint8_t} reg_addr
 * @param {uint8_t} write_byte
 * @return {*}
 */
uint8_t Int_MPU6050_WriteByte(uint8_t reg_addr, uint8_t write_byte)
{
    /* 1.起始信号 */
    Driver_I2C1_Start();
    /* 2.AD+W  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x00);
    /* 3.发送 寄存器的地址 */
    Driver_I2C_SendByte(reg_addr);
    /* 4.写入一字节 */
    Driver_I2C_SendByte(write_byte);
    /* 5.停止信号 */
    Driver_I2C1_Stop();

    return 0;
}

/**
 * @description: 向MPU6050写入多个字节
 * @param {uint8_t} reg_addr
 * @param {uint8_t} *write_bytes
 * @param {uint8_t} size
 * @return {*}
 */
uint8_t Int_MPU6050_WriteBytes(uint8_t reg_addr, uint8_t *write_bytes, uint8_t size)
{
    /* 1.起始信号 */
    Driver_I2C1_Start();
    /* 2.AD+W  */
    Driver_I2C_SendAddr(MPU_IIC_ADDR << 1 | 0x00);
    /* 3.发送 寄存器的地址 */
    Driver_I2C_SendByte(reg_addr);
    /* 4.写入多个字节 */
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_I2C_SendByte(write_bytes[i]);
    }
    /* 5.停止信号 */
    Driver_I2C1_Stop();

    return 0;
}

/**
 * @description: 根据采样率设置低通滤波器
 * @param {uint16_t} rate
 * @return {*}
 */
void Int_MPU6050_Set_DLPF_CFG(uint16_t rate)
{
    /* 采样定理： 采样率 >= 2*带宽 才不会失真 ===》 带宽 <= 采样率/2 */
    uint8_t cfg = 0;
    rate = rate / 2;
    if (rate > 188)
    {
        cfg = 1;
    }
    else if (rate > 98)
    {
        cfg = 2;
    }
    else if (rate > 42)
    {
        cfg = 3;
    }
    else if (rate > 20)
    {
        cfg = 4;
    }
    else if (rate > 10)
    {
        cfg = 5;
    }
    else
    {
        cfg = 6;
    }
    Int_MPU6050_WriteByte(MPU_CFG_REG, cfg << 0);
}

/**
 * @description: 设置陀螺仪的采样率
 * @param {uint16_t} rate
 * @return {*}
 */
void Int_MPU6050_SetGyroRate(uint16_t rate)
{
    /* 采样率=输出频率/(1+分频值) ===》 分频值 = (输出频率/采样率) -1 */
    uint8_t sample_div = 0;
    /* 1. 采样率限幅:产品说明书12页，输出频率最小值=4，最大是8k，我们期望最大是1k */
    if (rate < 4)
    {
        rate = 4;
    }
    else if (rate > 1000)
    {
        rate = 1000;
    }

    /* 2. 根据期望的采样率，计算出分频值 */
    sample_div = 1000 / rate - 1;

    /* 3. 将分频值设置到寄存器中 */
    Int_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, sample_div);

    /* 4. 根据采样率去设置低通滤波器 */
    Int_MPU6050_Set_DLPF_CFG(rate);
}

/**
 * @description: 初始化
 * @return {*}
 */
void Int_MPU6050_Init(void)
{
    uint8_t dev_id = 0;
    /* 1 初始化IIC */
    Driver_I2C1_Init();

    /* 2 复位 -》延迟一会 -》 唤醒 */
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x80);
    for_delay_ms(300);
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x00);

    /* 3 陀螺仪量程 +-2000°/s, fsr=3 */
    Int_MPU6050_WriteByte(MPU_GYRO_CFG_REG, 3 << 3);

    /* 4 加速度量程 */
    Int_MPU6050_WriteByte(MPU_ACCEL_CFG_REG, 0 << 3);

    /* 5 其他功能设置（可选）：FIFO、第二IIC、中断 */
    Int_MPU6050_WriteByte(MPU_INT_EN_REG, 0x00);    // 关闭所有中断
    Int_MPU6050_WriteByte(MPU_USER_CTRL_REG, 0x00); // 关闭第二IIC
    Int_MPU6050_WriteByte(MPU_FIFO_EN_REG, 0x00);   // 关闭FIFO

    /* 6 系统时钟源、陀螺仪采样率、低通滤波的设置 */
    /* 配置时钟源之前，确认正常工作，读一下id */
    Int_MPU6050_ReadByte(MPU_DEVICE_ID_REG, &dev_id);
    if (dev_id == MPU_IIC_ADDR)
    {
        /* 6.1 设置时钟源：陀螺仪X轴的时钟，精度更高 */
        Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x01);
        /* 6.2 设置陀螺仪采样率、低通滤波 */
        Int_MPU6050_SetGyroRate(100);
        /* 6.3 让两个传感器退出待机模式，进入正常工作状态*/
        Int_MPU6050_WriteByte(MPU_PWR_MGMT2_REG,0x00);
    }
}


/**
 * @description: 读取陀螺仪角速度的数据，拼接两字节数据且转成带符号的类型
 * @param {short} *gx
 * @param {short} *gy
 * @param {short} *gz
 * @return {*}
 */
void Int_MPU6050_Get_Gyro(short *gx,short *gy,short *gz)
{
    uint8_t buff[6];
    Int_MPU6050_ReadBytes(MPU_GYRO_XOUTH_REG,buff,6);

    /* 
        buff[0]:角速度X轴高8位
        buff[1]:角速度X轴低8位
        buff[2]:角速度Y轴高8位
        buff[3]:角速度Y轴低8位
        buff[4]:角速度Z轴高8位
        buff[5]:角速度Z轴低8位
     */
    *gx = ((short)buff[0] << 8)|buff[1];
    *gy = ((short)buff[2] << 8)|buff[3];
    *gz = ((short)buff[4] << 8)|buff[5];

}

/**
 * @description: 读取加速度计的加速度数据，拼接两字节数据且转成带符号的类型
 * @param {short} *ax
 * @param {short} *ay
 * @param {short} *az
 * @return {*}
 */
void Int_MPU6050_Get_Accel(short *ax,short *ay,short *az)
{
    uint8_t buff[6];
    Int_MPU6050_ReadBytes(MPU_ACCEL_XOUTH_REG,buff,6);

    /* 
        buff[0]:加速度X轴高8位
        buff[1]:加速度X轴低8位
        buff[2]:加速度Y轴高8位
        buff[3]:加速度Y轴低8位
        buff[4]:加速度Z轴高8位
        buff[5]:加速度Z轴低8位
     */
    *ax = ((short)buff[0] << 8)|buff[1];
    *ay = ((short)buff[2] << 8)|buff[3];
    *az = ((short)buff[4] << 8)|buff[5];

}
