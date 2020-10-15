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

#ifndef _ADS1299_SPI_ADAPT_H_
#define _ADS1299_SPI_ADAPT_H_
#endif

/* [] END OF FILE */

#include <project.h>
#include <comms.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t spi;
#define SPI_ADDRESS (&spi)
#define MAX_EEG_CHANNELS 8
#define UC3 1u

#define ADS1299_PIN_PWDN		1
#define ADS1299_PIN_DRDY		2
#define ADS1299_PIN_START		3
#define ADS1299_PIN_RESET		4
#define ADS1299_PIN_CS          5
#define ADS1299_PIN_CLKSEL      6
#define SPI_TIMEOUT             15000
#define ERR_TIMEOUT             -3
#define DSP_PREBUFFER_NB_SAMPLES    256	

cy_en_scb_spi_status_t spi_put(volatile uint8_t *spi, uint8_t opcode);
cy_en_scb_spi_status_t spi_get(volatile uint8_t *spi, uint8_t rxBuffer[]);
void spi_selectChip(volatile uint8_t *spi, uint8_t chip_select);
void spi_unselectChip(volatile uint8_t *spi, uint8_t chip_select);
void delay_s(int delay);
void delay_ms(int delay);
void delay_us(int delay);
void gpio_set_gpio_pin(uint32_t pin);
void gpio_clr_gpio_pin(uint32_t pin);
uint32_t spi_write_packet(uint8_t txBuffer[], uint8_t packet_size);
uint32_t spi_read_packet(uint8_t *data, uint8_t packet_size);
void HandleError(void);

/*static void spi_write_single(volatile uint8_t *spi, uint8_t data)
{
	spi_put(spi,(uint16_t)data);
}

static void spi_read_single(volatile uint8_t *spi, uint8_t *data)
{
	*data = (uint8_t)spi_get(spi);
}*/

static inline uint8 spi_is_tx_ready(volatile uint8_t *spi){
	return SPI_GetTxFifoStatus() != 0;
}
static inline uint8 spi_is_rx_ready(volatile uint8_t *spi){
	return SPI_GetRxFifoStatus() != 0;
}
static inline uint8 spi_is_tx_empty(volatile uint8_t *spi)
{
	return SPI_IsTxComplete(); // spi_STS_SPI_IDLE
}

static inline uint8 spi_data_ready(volatile uint8_t *spi)
{
	if (Cy_GPIO_Read(PIN_ADS1299_DRDY_0_PORT, PIN_ADS1299_DRDY_0_NUM) == 0) {
        //UART_PutString("drdy0");
        return 1;
    } else {
        //UART_PutString("1");
        return 0;
    }
}

/*void UART_PutNum(int number, int size) 
{
    
    char buffer[sizeof(int)*size];
    itoa(number, buffer, 16);   
    UART_PutString(buffer);
}*/