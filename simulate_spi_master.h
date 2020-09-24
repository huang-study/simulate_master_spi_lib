/*
 * @Description: Simulate master SPI lib base to GPIO
 * @Author: Huang
 * @Email: huangmailbox@126.com
 * @Version: 
 * @Date: 2020-09-20 22:30:09
 * @LastEditors: Huang
 * @LastEditTime: 2020-09-24 23:14:25
 */
#ifndef __SIMULATE_SPI_MASTER_H_
#define __SIMULATE_SPI_MASTER_H_

#include "stdint.h"

#define IS_NULL(p, ret) \
    if (NULL == p)      \
    {                   \
        return (ret);   \
    }
#define IS_ZERO(val, ret) \
    if (0 == val)         \
    {                     \
        return (ret);     \
    }

// 电平跳变的延时函数
#define SIMULATE_SPI_DELAY(time)
// 内存堆申请函数
#define SIMULATE_SPI_MALLOC(size) malloc((size))
// 内存堆释放函数
#define SIMULATE_SPI_FREE(mem) free((mem))

#define SIMULATE_SPI_MODE_0 (0X0)
#define SIMULATE_SPI_MODE_1 (0x1)
#define SIMULATE_SPI_MODE_2 (0x2)
#define SIMULATE_SPI_MODE_3 (0x3)

#ifndef GPIO_PIN_SET
#define GPIO_PIN_SET (1)
#endif
#ifndef GPIO_PIN_RESET
#define GPIO_PIN_RESET (0)
#endif

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

/**
 * @description: 初始化一个模拟 SPI 控制块
 * @param hand 模拟 SPI 控制块
 *        spi_mode SPI 工作模式
 *          SIMULATE_SPI_MODE_0
            SIMULATE_SPI_MODE_1
            SIMULATE_SPI_MODE_2
            SIMULATE_SPI_MODE_3
          sdi_get 输入引脚电平获取函数
          sdo_set 输出引脚电平设置函数
          sclk_set 时钟引脚电平设置函数
          hold_time 数据保持时间
          wait_time 等待数据生成时间
 * @return 模拟 SPI 控制块指针
 */
simulate_spi_t *simulate_spi_init(simulate_spi_t *hand,
                                  uint8_t spi_mode,
                                  simulate_gpio_get_t sdi_get,
                                  simulate_gpio_set_t sdo_set,
                                  simulate_gpio_set_t sclk_set,
                                  uint32_t hold_time,
                                  uint32_t wait_time);
/**
 * @description: 创建一个模拟 SPI 控制块(需要配置内存堆接口函数)
 * @param hand 模拟 SPI 控制块
 *        spi_mode SPI 工作模式
 *          SIMULATE_SPI_MODE_0
            SIMULATE_SPI_MODE_1
            SIMULATE_SPI_MODE_2
            SIMULATE_SPI_MODE_3
          sdi_get 输入引脚电平获取函数
          sdo_set 输出引脚电平设置函数
          sclk_set 时钟引脚电平设置函数
          hold_time 数据保持时间
          wait_time 等待数据生成时间
 * @return 模拟 SPI 控制块指针
 */
simulate_spi_t *simulate_spi_create(uint8_t spi_mode,
                                    simulate_gpio_get_t sdi_get,
                                    simulate_gpio_set_t sdo_set,
                                    simulate_gpio_set_t sclk_set,
                                    uint32_t hold_time,
                                    uint32_t wait_time);

/**
 * @description: 还原一个模拟 SPI 控制块
 * @param hand 模拟 SPI 控制块
 */
void simulate_spi_deinit(simulate_spi_t *hand);
/**
 * @description: 销毁释放一个模拟 SPI 控制块
 * @param hand 模拟 SPI 控制块
 */
void simulate_spi_destory(simulate_spi_t *hand);

/**
 * @description: 发送，读取一个字节
 * @param hand 模拟 SPI 控制块
 *        dat 发送的数据
 * @return 成功返回 0, 失败返回非 0  
 */
uint8_t simulate_spi_read_write_byte(simulate_spi_t *hand, uint8_t dat);

#endif /* __SIMULATE_SPI_MASTER_H_ */
