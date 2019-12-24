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

#include <project.h>

uint8_t spi;
#define SPI_ADDRESS (&spi)
#define MAX_EEG_CHANNELS 8

#define ADS1299_PIN_PWDN		0
#define ADS1299_PIN_DRDY		0
#define ADS1299_PIN_START		0
#define ADS1299_PIN_RESET		0

void spi_put(volatile uint8_t *spi, uint8_t opcode);
void spi_selectChip(volatile uint8_t *spi, uint8_t chip_select);
void spi_unselectChip(volatile uint8_t *spi, uint8_t chip_select);
void delay_s(int delay);
void delay_ms(int delay);
void delay_us(int delay);
static inline uint8 spi_is_tx_ready(volatile uint8_t *spi);
static inline uint8 spi_is_tx_empty(volatile uint8_t *spi);
void gpio_set_gpio_pin(uint32_t pin);
void gpio_clr_gpio_pin(uint32_t pin);