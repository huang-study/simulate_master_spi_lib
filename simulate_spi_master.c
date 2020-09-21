#include "./simulate_spi_master.h"

simulate_spi_t *simulate_spi_init(simulate_spi_t *hand,
                                  uint8_t spi_mode,
                                  simulate_gpio_get_t sdi_get,
                                  simulate_gpio_set_t sdo_set,
                                  simulate_gpio_set_t sclk_set,
                                  uint32_t hold_time,
                                  uint32_t wait_time)
{
}
simulate_spi_t *simulate_spi_create(uint8_t spi_mode,
                                    simulate_gpio_get_t sdi_get,
                                    simulate_gpio_set_t sdo_set,
                                    simulate_gpio_set_t sclk_set,
                                    uint32_t hold_time,
                                    uint32_t wait_time)
{
}

void simulate_spi_deinit(simulate_spi_t *hand)
{
}
void simulate_spi_destory(simulate_spi_t *hand)
{
}

uint8_t simulate_spi_transf_byte(simulate_spi_t *hand, uint8_t dat)
{
}
uint8_t simulate_spi_recive_byte(simulate_spi_t *hand)
{
}
