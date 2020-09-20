/*
 * @Description: Simulate master SPI lib base to GPIO
 * @Author: Huang
 * @Email: huangmailbox@126.com
 * @Version: 
 * @Date: 2020-09-20 22:30:09
 * @LastEditors: Huang
 * @LastEditTime: 2020-09-20 23:48:33
 */
#ifndef __SIMULATE_SPI_MASTER_H_
#define __SIMULATE_SPI_MASTER_H_

#include "stdint.h"

// 电平跳变的延时函数
#define SIMULATE_SPI_DELAY(time)

// 由于需要通过通过上拉获取输入，建议引脚设为开漏上拉

// CPOL:时钟极性: SCLK 空闲电平
//     CPOL=0，表示当 SCLK=0 时处于空闲态，所以有效状态就是SCLK处于高电平时
//     CPOL=1，表示当 SCLK=1 时处于空闲态，所以有效状态就是SCLK处于低电平时
// CPHA:时钟相位: 数据采样在第几个边沿
//     CPHA=0，表示数据采样是在第1个边沿，数据发送在第2个边沿
//     CPHA=1，表示数据采样是在第2个边沿，数据发送在第1个边沿

// Mode0：CPOL=0，CPHA=0
// Mode1：CPOL=0，CPHA=1
// Mode2：CPOL=1，CPHA=0
// Mode3：CPOL=1，CPHA=1

#define SIMULATE_SPI_MODE_0 (0X0)
#define SIMULATE_SPI_MODE_0 (0x1)
#define SIMULATE_SPI_MODE_0 (0x2)
#define SIMULATE_SPI_MODE_0 (0x3)

typedef void (*simulate_gpio_set_t)(uint8_t *);
typedef uint8_t (*simulate_gpio_get_t)(void);

typedef struct
{
    uint8_t spi_mode;

    // 数据输入输出引脚
    simulate_gpio_get_t sdi_get;
    simulate_gpio_set_t sdo_set;

    // 设置控制引脚(主机模式必须，片选信号用户自行操作)
    simulate_gpio_set_t sclk_set;

    // 电平保持时间保持时间 , 基于 SIMULATE_SPI_DELAY 的延时函数
    // 如果为 0 或 SIMULATE_SPI_DELAY 宏无参数，则无传输延时
    uint32_t hold_time;
    // 等待数据生成时间 , 基于 SIMULATE_SPI_DELAY 的延时函数
    // 如果为 0 或 SIMULATE_SPI_DELAY 宏无参数，则无传输延时
    uint32_t wait_time;
} simulate_spi_t;

simulate_spi_t *simulate_spi_init(simulate_spi_t *hand,
                                  uint8_t spi_mode,
                                  simulate_gpio_get_t sdi_get,
                                  simulate_gpio_set_t sdo_set,
                                  simulate_gpio_set_t sclk_set,
                                  uint32_t hold_time,
                                  uint32_t wait_time);
simulate_spi_t *simulate_spi_create(uint8_t spi_mode,
                                    simulate_gpio_get_t sdi_get,
                                    simulate_gpio_set_t sdo_set,
                                    simulate_gpio_set_t sclk_set,
                                    uint32_t hold_time,
                                    uint32_t wait_time);

void simulate_spi_deinit(simulate_spi_t *hand);
void simulate_spi_destory(simulate_spi_t *hand);

void simulate_spi_transfer_byte(simulate_spi_t *hand, uint8_t dat);
uint8_t simulate_spi_recive_byte(simulate_spi_t *hand, uint8_t dat);

#endif /* __SIMULATE_SPI_MASTER_H_ */
