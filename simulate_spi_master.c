#include "./simulate_spi_master.h"

simulate_spi_t *simulate_spi_init(simulate_spi_t *hand,
                                  uint8_t spi_mode,
                                  simulate_gpio_get_t sdi_get,
                                  simulate_gpio_set_t sdo_set,
                                  simulate_gpio_set_t sclk_set,
                                  uint32_t hold_time,
                                  uint32_t wait_time)
{
    IS_NULL(hand, NULL);
    IS_NULL(sdi_get, NULL);
    IS_NULL(sdo_set, NULL);
    IS_NULL(sclk_set, NULL);
    if (spi_mode >= 4)
    { // 模式不对
        return NULL;
    }

    hand->spi_mode = spi_mode;
    hand->sdi_get = sdi_get;
    hand->sclk_set = sclk_set;
    hand->sdo_set = sdo_set;
    hand->hold_time = hold_time;
    hand->wait_time = wait_time;

    if (hand->spi_mode & 0x2)
    {
        hand->sclk_set(GPIO_PIN_SET);
    }
    else
    {
        hand->sclk_set(GPIO_PIN_RESET);
    }

    return hand;
}
simulate_spi_t *simulate_spi_create(uint8_t spi_mode,
                                    simulate_gpio_get_t sdi_get,
                                    simulate_gpio_set_t sdo_set,
                                    simulate_gpio_set_t sclk_set,
                                    uint32_t hold_time,
                                    uint32_t wait_time)
{
    IS_NULL(sdi_get, NULL);
    IS_NULL(sdo_set, NULL);
    IS_NULL(sclk_set, NULL);
    if (spi_mode >= 4)
    { // 模式不对
        return NULL;
    }

    simulate_spi_t *hand = SIMULATE_SPI_MALLOC(sizeof(simulate_spi_t));
    IS_NULL(hand, NULL);

    return simulate_spi_init(hand, spi_mode, sdi_get, sdo_set, sclk_set, hold_time, wait_time);
}

void simulate_spi_deinit(simulate_spi_t *hand)
{
    if (NULL == hand)
    {
        return;
    }
    hand->spi_mode = 0;
    hand->sdi_get = NULL;
    hand->sdo_set = NULL;
    hand->sclk_set = NULL;
    hand->hold_time = 0;
    hand->wait_time = 0;
}
void simulate_spi_destory(simulate_spi_t *hand)
{
    if (NULL == hand)
    {
        return;
    }
    simulate_spi_deinit(hand);
    SIMULATE_SPI_FREE(hand);
    hand = NULL;
}

uint8_t simulate_spi_read_write_byte(simulate_spi_t *hand, uint8_t dat)
{
    uint8_t i, rxdata;
    IS_NULL(hand, 0);

    if (hand->spi_mode & 0x2)
    {
        hand->sclk_set(GPIO_PIN_SET);
    }
    else
    {
        hand->sclk_set(GPIO_PIN_RESET);
    }

    if (hand->spi_mode & 0x1)
    { // CPHA == 1
        //start
        for (i = 0; i < 8; i++)
        {
            // 设置 bit
            if (dat & 0x80)
                hand->sdo_set(GPIO_PIN_SET);
            else
                hand->sdo_set(GPIO_PIN_RESET);
            SIMULATE_SPI_DELAY(hand->hold_time);

            // 电平跳转，发送 bit
            if (hand->spi_mode & 0x2)
            {
                hand->sclk_set(GPIO_PIN_RESET);
            }
            else
            {
                hand->sclk_set(GPIO_PIN_SET);
            }

            dat <<= 1;
            rxdata <<= 1;
            // 电平跳转，等待数据
            if (hand->spi_mode & 0x2)
            {
                hand->sclk_set(GPIO_PIN_SET);
            }
            else
            {
                hand->sclk_set(GPIO_PIN_RESET);
            }
            SIMULATE_SPI_DELAY(hand->wait_time);

            if (GPIO_PIN_SET == hand->sdi_get())
                rxdata |= 0x01;
            else
                rxdata &= 0xfe;
            //end sck
        }
    }
    else
    { // CPHA == 0
        //start
        for (i = 0; i < 8; i++)
        {
            rxdata <<= 1;
            // 电平跳转，等待数据
            if (hand->spi_mode & 0x2)
            {
                hand->sclk_set(GPIO_PIN_RESET);
            }
            else
            {
                hand->sclk_set(GPIO_PIN_SET);
            }
            SIMULATE_SPI_DELAY(hand->wait_time);

            if (GPIO_PIN_SET == hand->sdi_get())
                rxdata |= 0x01;
            else
                rxdata &= 0xfe;

            // 设置 bit
            if (dat & 0x80)
                hand->sdo_set(GPIO_PIN_SET);
            else
                hand->sdo_set(GPIO_PIN_RESET);
            SIMULATE_SPI_DELAY(hand->hold_time);

            // 电平跳转，发送 bit
            if (hand->spi_mode & 0x2)
            {
                hand->sclk_set(GPIO_PIN_SET);
            }
            else
            {
                hand->sclk_set(GPIO_PIN_RESET);
            }
            dat <<= 1;
            //end sck
        }
    }
    //end
    return rxdata;
}
