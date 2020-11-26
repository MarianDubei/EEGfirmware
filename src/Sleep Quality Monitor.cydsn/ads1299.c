/** 
 *  \file ads1299.c
 *  \brief Some useful functions for using the ADS1299 EEG analog front-end from Texas Instruments.
 *	\details The ADS1299 is an SPI slave device. Communication with it requires at least one SPI master module on the host.
 *	\author Graham Kelly
 *	\version 1.0
 *	\date August 2014
 *
 *  All Atmel Software Framework libraries used herein are copyright Atmel and
 *  subject to their appropriate licenses, which allow free redistribution with
 *  some restrictions. These restrictions are listed in their appropriate files.
 *
 *  \page License
 *
 *  Brainboard firmware code is placed under the MIT license
 *  Copyright (c) 2014 Graham Kelly
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ads1299.h"


/**************************************************************************************************************************************************
*               Function Definitions                                                                                                              *
**************************************************************************************************************************************************/

/* SYSTEM CONTROL FUNCTIONS **********************************************************************************************************************/

ads1299_error_t	ads1299_device_init(uint8_t chip_select, uint8_t init_regs)
{
	#if UC3
        
    gpio_set_gpio_pin(ADS1299_PIN_CLKSEL);
    
	/* Power cycle ADS1299 */
	//gpio_clr_gpio_pin(ADS1299_PIN_PWDN);
	//CyDelay(1000);
	gpio_set_gpio_pin(ADS1299_PIN_PWDN);
    //CyDelay(1);
    
    gpio_set_gpio_pin(ADS1299_PIN_RESET);
	CyDelay(1000);
    gpio_clr_gpio_pin(ADS1299_PIN_RESET);
	CyDelay(1000);
	gpio_set_gpio_pin(ADS1299_PIN_RESET);
    CyDelay(10);
    //gpio_clr_gpio_pin(ADS1299_PIN_RESET);
	//CyDelay(1);
    
	/* Allow oscillator warm-up */
	CyDelay(1000);
	
    spi_selectChip(SPI_ADDRESS, chip_select);
    CyDelay(1000);
	spi_unselectChip(SPI_ADDRESS, chip_select);
    CyDelay(500);
    
	/* Tell chip to exit continuous data mode */
	ads1299_send_byte(chip_select, ADS1299_OPC_SDATAC);
    CyDelay(10);
    ads1299_send_byte(chip_select, ADS1299_OPC_STOP);
    //ads1299_hard_stop_conversion();
    CyDelay(10);
    /* Stop taking conversions; apparently not done automatically */
 	    
    
	if (init_regs) 
	{
        /* Write to GPIO register, set all pins to driven-low output */
		ads1299_wreg(chip_select, ADS1299_REGADDR_GPIO, ADS1299_REG_GPIO_GPIOC4_OUTPUT |
		ADS1299_REG_GPIO_GPIOD4_LOW    |
		ADS1299_REG_GPIO_GPIOC3_OUTPUT |
		ADS1299_REG_GPIO_GPIOD3_LOW    |
		ADS1299_REG_GPIO_GPIOC2_OUTPUT |
		ADS1299_REG_GPIO_GPIOD2_LOW    |
		ADS1299_REG_GPIO_GPIOC1_OUTPUT |
		ADS1299_REG_GPIO_GPIOD1_LOW    );
			
		/* Write to CONFIG1, set data rate to 250 Hz */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CONFIG1, ADS1299_REG_CONFIG1_RESERVED_VALUE |
		ADS1299_REG_CONFIG1_FMOD_DIV_BY_1024);
		/* Write to CONFIG2 register, generate test signal internally */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CONFIG2, ADS1299_REG_CONFIG2_RESERVED_VALUE |
		ADS1299_REG_CONFIG2_CAL_INT        |
		ADS1299_REG_CONFIG2_CAL_PULSE_FCLK_DIV_2_21);
			
		/* Write to CONFIG3, enable internal reference buffer, bias internally generated, bias buffer enabled */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CONFIG3, ADS1299_REG_CONFIG3_REFBUF_ENABLED |
		//ADS1299_REG_CONFIG3_BIASREF_INT    |
		ADS1299_REG_CONFIG3_BIASBUF_ENABLED);
		/* Reference settling time */
		delay_ms(150);
			
		/* Write to CH1 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH1SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH2 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH2SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_NORMAL_ELECTRODE);
		/* Write to CH3 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH3SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
		/* Write to CH4 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH4SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
		/* Write to CH5 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH5SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
		/* Write to CH6 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH6SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
		/* Write to CH5 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH7SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
		/* Write to CH6 settings register, set as normal input, gain 24 */
		ads1299_wreg(chip_select, ADS1299_REGADDR_CH8SET, ADS1299_REG_CHNSET_CHANNEL_ON			|
		ADS1299_REG_CHNSET_GAIN_4			|
		ADS1299_REG_CHNSET_SRB2_DISCONNECTED	|
		ADS1299_REG_CHNSET_CHANNEL_OFF);
			
		/* Write to MISC1 register, SRB1 on (ref electrode) */
		ads1299_wreg(chip_select, ADS1299_REGADDR_MISC1, ADS1299_REG_MISC1_SRB1_ON);
	} 		
    CyDelay(100);
    uint8_t reg_val[1];
    ads1299_rreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CONFIG3, reg_val);
    printf("regval %x ", reg_val[0]);
    
	return ADS1299_STATUS_OK;
	#else
	#endif /* #if UC3 */
}

ads1299_error_t	ads1299_lead_off(uint8_t chip_select)
{
    ads1299_wreg(chip_select, ADS1299_REGADDR_LOFF, ADS1299_REG_LOFF_95_PERCENT | ADS1299_REG_LOFF_6_NA | ADS1299_REG_LOFF_AC_LEAD_OFF_FDR_DIV_4);
    ads1299_wreg(chip_select, ADS1299_REGADDR_CONFIG4, ADS1299_REG_CONFIG4_LEAD_OFF_ENABLED);
    ads1299_wreg(chip_select, ADS1299_REGADDR_LOFF_SENSP, ADS1299_REG_LOFF_SENSP_LOFFP1 |
    ADS1299_REG_LOFF_SENSP_LOFFP2 |
    ADS1299_REG_LOFF_SENSP_LOFFP3 |
    ADS1299_REG_LOFF_SENSP_LOFFP4 |
    ADS1299_REG_LOFF_SENSP_LOFFP5 |
    ADS1299_REG_LOFF_SENSP_LOFFP6 |
    ADS1299_REG_LOFF_SENSP_LOFFP7 |
    ADS1299_REG_LOFF_SENSP_LOFFP8);
    
    return ADS1299_STATUS_OK;
}


/* REGISTER READ/WRITE FUNCTIONS *****************************************************************************************************************/

ads1299_error_t ads1299_rreg(uint8_t chip_select, uint8_t reg_addr, uint8_t read_reg_val_arr[])
{
		
	ads1299_send_byte(chip_select, ADS1299_OPC_SDATAC);
    //ads1299_hard_stop_conversion();
    ads1299_soft_stop_conversion(chip_select);
	spi_selectChip(SPI_ADDRESS, chip_select);
    
    //CyDelay(1);
    
    /*SPI_Write(ADS1299_OPC_RREG | reg_addr);
    SPI_Write(0x00);
    SPI_Write(0x00);
    *read_reg_val_arr = SPI_Read();*/
    
    uint8_t tx_buf[] = {ADS1299_OPC_RREG | reg_addr, 0x00, 0xFF};
    spi_write_packet(tx_buf, 3);
	/* First byte: read command for specified register */
	//ads1299_send_byte(chip_select, ADS1299_OPC_RREG | reg_addr);
	    
    //CyDelay(1);
        
	/* Second byte: Read only 1 register (send n-1, where n is number of registers to read) */
	//ads1299_send_byte(chip_select, 0x00);
    
    //CyDelay(1);
	
    //uint8_t tx_buf[] = {ADS1299_OPC_RREG | reg_addr, 0x00};
    //spi_write_packet(tx_buf, 2);
	/* Dummy byte to clock in data and read RX register */
    //spi_read_packet(read_reg_val_arr, 1);
    
    spi_unselectChip(SPI_ADDRESS, chip_select);
    
	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_wreg(uint8_t chip_select, uint8_t reg_addr, uint8_t reg_val_to_write)
{
	
	//spi_selectChip(SPI_ADDRESS, chip_select);
    uint8_t tx_buf[] = {ADS1299_OPC_WREG | reg_addr, 0x00, reg_val_to_write};
    spi_write_packet(tx_buf, 3);
	/* First byte: write command for specified register */
    //ads1299_send_byte(chip_select, ADS1299_OPC_WREG | reg_addr);
    
	//CyDelay(100);
	/* Second byte: number of registers to write (1) */
	//ads1299_send_byte(chip_select, 0x00);
    
    //CyDelay(100);
	/* Third byte: write register value */
	//ads1299_send_byte(chip_select, reg_val_to_write);
		
	//spi_unselectChip(SPI_ADDRESS, chip_select);
		
	return ADS1299_STATUS_OK;
	
}

/* DATA RETRIEVAL FUNCTIONS **********************************************************************************************************************/

ads1299_error_t ads1299_rdata32_packet(uint8_t chip_select, volatile uint32_t sample_idx, bboard_data32bit_packet_t* packet_ptr)
{
	#if UC3
	volatile uint8_t channel_idx;	
	union {
		uint32_t raw;
		uint8_t status[4];
	} __attribute__((packed)) statustemp;	
	union {
		int32_t raw;
		uint8_t data[4];
	} __attribute__((packed)) sigtemp;
	statustemp.raw = 0;
	sigtemp.raw = 0;
	
	/* Begin SPI comms */
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* Function assumes we've already sent RDATA command or are in RDATAC mode */
		
	/* Read in status word first (24 bits) */
	spi_read_packet(&statustemp.status[1], 3);
	packet_ptr->eegstatus = statustemp.raw;
	
	/* Begin reading in data */	
	for (channel_idx = 0; channel_idx < MAX_EEG_CHANNELS; channel_idx++)
	{
		spi_read_packet(&sigtemp.data[1], 3);
		packet_ptr->eegdata[sample_idx][channel_idx] = SIGN_EXT_24(sigtemp.raw);
	}
		
	spi_unselectChip(SPI_ADDRESS, chip_select);
		
	#else
	#endif	/* #if UC3 */
	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_rdata24_packet(uint8_t chip_select, volatile uint32_t sample_idx, bboard_data24bit_packet_t* packet_ptr)
{
	#if UC3
	volatile uint8_t channel_idx;
	
	uint8_t temp[27];
	
	/* Begin SPI comms */
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* Function assumes we've already sent RDATA command or are in RDATAC mode */
	
	/* Read in status word first (24 bits) */
	spi_read_packet(temp, 3);
	packet_ptr->eegstatus[0] = temp[0];
	packet_ptr->eegstatus[1] = temp[1];
	packet_ptr->eegstatus[2] = temp[2];
	
	/* Begin reading in data */
	for (channel_idx = 0; channel_idx < MAX_EEG_CHANNELS; channel_idx++)
	{
		spi_read_packet(temp, 3);
		packet_ptr->eegdata[sample_idx][channel_idx][0] = temp[0];
		packet_ptr->eegdata[sample_idx][channel_idx][1] = temp[1];
		packet_ptr->eegdata[sample_idx][channel_idx][2] = temp[2];
	}
	
	spi_unselectChip(SPI_ADDRESS, chip_select);
	
	#else
	#endif	/* #if UC3 */
	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_rdata24_generic(uint8_t chip_select, volatile uint32_t sample_idx, volatile uint8_t status_array[], volatile uint8_t data_array[][MAX_EEG_CHANNELS][3])
{
	uint32_t transfer_status;
	volatile uint8_t channel_idx;
	uint8_t sigtemp[27];
	
	/* Begin SPI comms */
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* Function assumes we've already sent RDATA command or are in RDATAC mode */
	// check if DRDY
    
    
    int j = 0;
    while (spi_data_ready(SPI_ADDRESS) == 0) {
        j++;
    }
    ads1299_soft_stop_conversion(chip_select);
    
	transfer_status = spi_read_packet(sigtemp, 27);
    
    
	spi_unselectChip(SPI_ADDRESS, chip_select);
	
    ads1299_soft_start_conversion(chip_select);
    
    // split data
    
    status_array[0] = sigtemp[1];
    status_array[1] = sigtemp[2];
    status_array[2] = sigtemp[3];
    
    
    for (channel_idx = 0; channel_idx < MAX_EEG_CHANNELS; channel_idx++)
	{
		data_array[sample_idx][channel_idx][0] = sigtemp[4+channel_idx*3];
		data_array[sample_idx][channel_idx][1] = sigtemp[4+channel_idx*3+1];
        if (channel_idx == 7)
            data_array[sample_idx][channel_idx][2] = 0xff;
		else
            data_array[sample_idx][channel_idx][2] = sigtemp[4+channel_idx*3+2];
	}
	
	return ADS1299_STATUS_OK;
}

ads1299_error_t ads1299_rdata32_generic(uint8_t chip_select, volatile uint32_t sample_idx, volatile uint32_t status, volatile int32_t data_array[][MAX_EEG_CHANNELS])
{
	//#if UC3
	volatile uint8_t channel_idx;
	union {
		uint32_t raw;
		uint8_t status[4];
	} __attribute__((packed)) statustemp;
	union {
		int32_t raw;
		uint8_t data[4];
	} __attribute__((packed)) sigtemp;
	statustemp.raw = 0;
	sigtemp.raw = 0;
	
	/* Begin SPI comms */
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* Function assumes we've already sent RDATA command or are in RDATAC mode */
	
	/* Read in status word first (24 bits) */
	spi_read_packet(&statustemp.status[1], 3);
	status = statustemp.raw;
	
	/* Begin reading in data */
	/* Begin reading in data */
	for (channel_idx = 0; channel_idx < MAX_EEG_CHANNELS; channel_idx++)
	{
		spi_read_packet(&sigtemp.data[1], 3);
		data_array[sample_idx][channel_idx] = SIGN_EXT_24(sigtemp.raw);
	}
	
	spi_unselectChip(SPI_ADDRESS, chip_select);
	
	//#else
	//#endif	/* #if UC3 */
	return ADS1299_STATUS_OK;
}

#ifdef __cplusplus
}
#endif