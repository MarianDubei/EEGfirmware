/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */

#include "ads1299_spi_adapt.h"

uint8_t spi = 0xFF;

void spi_put(volatile uint8_t *spi, uint8_t opcode)
{
    spi_WriteTxData(opcode);
}

void spi_selectChip(volatile uint8_t *spi, uint8_t chip_select)
{   
}

void spi_unselectChip(volatile uint8_t *spi, uint8_t chip_select)
{   
}

void delay_s(int delay)
{
    CyDelay(1000 * delay);
}

void delay_ms(int delay)
{
    CyDelay(delay);
}

void delay_us(int delay)
{
    CyDelayUs(delay);
}

static inline uint8 spi_is_tx_ready(volatile uint8_t *spi)
{
	return spi_ReadTxStatus();
}

static inline uint8 spi_is_tx_empty(volatile uint8_t *spi)
{
	return spi_ReadTxStatus();
}

void gpio_set_gpio_pin(uint32_t pin)
{
}

void gpio_clr_gpio_pin(uint32_t pin)
{

}
