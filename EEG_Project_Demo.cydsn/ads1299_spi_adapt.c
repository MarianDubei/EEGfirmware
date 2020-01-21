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

#include <ads1299_spi_adapt.h>

uint8_t spi = 0xFF;

static inline void spi_write_single(volatile uint8_t *spi, uint8_t data)
{
	spi_put(spi,(uint16_t)data);
}

static inline void spi_read_single(volatile uint8_t *spi, uint8_t *data)
{
	*data = (uint8_t)spi_get(spi);
}

void spi_put(volatile uint8_t *spi, uint8_t opcode)
{
    spi_WriteTxData(opcode);
}

uint16_t spi_get(volatile uint8_t *spi)
{
    return spi_ReadRxData();
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




void gpio_set_gpio_pin(uint32_t pin)
{
    switch (pin)
    {
        case ADS1299_PIN_PWDN:
            PIN_ADS1299_PWDN_Write(1);
        break;
        case ADS1299_PIN_DRDY:
            PIN_ADS1299_DRDY_Write(1);
        break;
        case ADS1299_PIN_START:
            PIN_ADS1299_START_Write(1);
        break;
        case ADS1299_PIN_RESET:
            PIN_ADS1299_RESET_Write(1);
        break;
    }
}

void gpio_clr_gpio_pin(uint32_t pin)
{
    switch (pin)
        {
            case ADS1299_PIN_PWDN:
                PIN_ADS1299_PWDN_Write(0);
            break;
            case ADS1299_PIN_DRDY:
                PIN_ADS1299_DRDY_Write(0);
            break;
            case ADS1299_PIN_START:
                PIN_ADS1299_START_Write(0);
            break;
            case ADS1299_PIN_RESET:
                PIN_ADS1299_RESET_Write(0);
            break;
        }
}

void spi_read_packet(volatile uint8_t *spi,
		uint8_t *data, size_t len)
{
	unsigned int timeout = SPI_TIMEOUT;
	uint8_t val;
	size_t i=0;
	while(len) {
		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(spi)) {
			if (!timeout--) {
				return;
			}
		}
		spi_write_single(spi, DUMMY_BYTE);  
		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_ready(spi)) {
			if (!timeout--) {
				return;
			}
		}
		spi_read_single(spi,&val);
		data[i] = val;
		i++;
		len--;
	}
	return;
}