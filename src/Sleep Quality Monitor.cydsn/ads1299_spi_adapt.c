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

#define RX_PACKET_SIZE  25
#define ONE_PACKET_SIZE  1
#define TRANSFER_ERROR        (0xFFUL)

/* Combine master error statuses in single mask */
#define MASTER_ERROR_MASK  (CY_SCB_SPI_SLAVE_TRANSFER_ERR  | CY_SCB_SPI_TRANSFER_OVERFLOW    | \
                            CY_SCB_SPI_TRANSFER_UNDERFLOW)  

/* Start and end of packet markers */
#define PACKET_SOP            (0x01UL)
#define PACKET_EOP            (0x17UL)

/* Command valid status */
#define TRANSFER_CMPLT        (0x00UL)
#define TRANSFER_ERROR        (0xFFUL)
#define READ_CMPLT            (TRANSFER_CMPLT)
#define READ_ERROR            (TRANSFER_ERROR)


/* Command valid status */
#define STS_CMD_DONE    (0x00UL)
#define STS_CMD_FAIL    (0x1FUL)

/* Packet positions */
#define PACKET_SOP_POS        (0UL)
#define PACKET_CMD_1_POS      (1UL)
#define PACKET_CMD_2_POS      (2UL)
#define PACKET_CMD_3_POS      (3UL)
#define PACKET_EOP_POS        (4UL)
#define PACKET_STS_POS        (1UL)
#define RX_PACKET_SOP_POS     (0UL)
#define RX_PACKET_STS_POS     (1UL)
#define RX_PACKET_EOP_POS     (2UL)


uint8_t spi = 0xFF;


cy_en_scb_spi_status_t spi_put(volatile uint8_t *spi, uint8_t opcode)
{
    uint8_t txBuffer[ONE_PACKET_SIZE];
    txBuffer[0] = opcode;
    printf("%x ", opcode);
    return SPI_Transfer(txBuffer, NULL, ONE_PACKET_SIZE);
}

cy_en_scb_spi_status_t spi_get(volatile uint8_t *spi, uint8_t rxBuffer[])
{
    cy_en_scb_spi_status_t transferStatus;
    transferStatus = SPI_Transfer(NULL, rxBuffer, ONE_PACKET_SIZE);
    printf("%x ", rxBuffer[0]);
    return transferStatus;
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
            Cy_GPIO_Set(PIN_ADS1299_PWDN_0_PORT, PIN_ADS1299_PWDN_0_NUM);
        break;
        case ADS1299_PIN_DRDY:
            Cy_GPIO_Set(PIN_ADS1299_DRDY_0_PORT, PIN_ADS1299_DRDY_0_NUM);
        break;
        case ADS1299_PIN_START:
            Cy_GPIO_Set(PIN_ADS1299_START_0_PORT, PIN_ADS1299_START_0_NUM);
        break;
        case ADS1299_PIN_RESET:
            Cy_GPIO_Set(PIN_ADS1299_RESET_0_PORT, PIN_ADS1299_RESET_0_NUM);
        break;
    }
}

void gpio_clr_gpio_pin(uint32_t pin)
{
    switch (pin)
    {
        case ADS1299_PIN_PWDN:
            Cy_GPIO_Clr(PIN_ADS1299_PWDN_0_PORT, PIN_ADS1299_PWDN_0_NUM);
        break;
        case ADS1299_PIN_DRDY:
            Cy_GPIO_Clr(PIN_ADS1299_DRDY_0_PORT, PIN_ADS1299_DRDY_0_NUM);
        break;
        case ADS1299_PIN_START:
            Cy_GPIO_Clr(PIN_ADS1299_START_0_PORT, PIN_ADS1299_START_0_NUM);
        break;
        case ADS1299_PIN_RESET:
            Cy_GPIO_Clr(PIN_ADS1299_RESET_0_PORT, PIN_ADS1299_RESET_0_NUM);
        break;
    }
}

uint32_t spi_read_packet(volatile uint8_t *spi, uint8_t data[])
{
	uint32_t count = 0;
    uint8_t tempVar;
    uint32_t timeOutStatusCheck = 1000UL; /* Timeout 1 sec */
    uint8_t statusRxBuf[RX_PACKET_SIZE];
    cy_en_scb_spi_status_t errorStatus;
    uint32_t status = TRANSFER_ERROR;

	do
    {
        errorStatus = spi_get(spi, &tempVar);
        printf("data received");
        if(errorStatus == CY_SCB_SPI_SUCCESS)
        {
            
            uint32_t masterStatus;         
            /* Timeout 1 sec */
            uint32_t timeOut = 1000UL;
            
            /* Wait until master complete read transfer or time out has occured */
            do
            {
                masterStatus  = SPI_GetTransferStatus();
                CyDelay(CY_SCB_WAIT_1_UNIT);
                timeOut--;
                
            } while ((0UL != (masterStatus & CY_SCB_SPI_TRANSFER_ACTIVE)) && (timeOut > 0UL) );
                
            if ((0UL == (MASTER_ERROR_MASK & masterStatus)) && (tempVar != 0xFF) )
            {  
                statusRxBuf[count] = (uint8_t)tempVar;
                count++;
         
                if(count == RX_PACKET_SIZE)
                {
                    /* Check packet structure and set status */
                    /*if((PACKET_SOP   == statusRxBuf[RX_PACKET_SOP_POS]) &&
                        (PACKET_EOP   == statusRxBuf[RX_PACKET_EOP_POS]) &&
                        (STS_CMD_DONE == statusRxBuf[RX_PACKET_STS_POS]) )
                    {*/
                        status = TRANSFER_CMPLT;
                    
                    //}
                }
            }
            else 
            {
                HandleError();
            }
        }     
        
        Cy_SysLib_Delay(CY_SCB_WAIT_1_UNIT);
        timeOutStatusCheck--;
        
    }while((count < RX_PACKET_SIZE) && (timeOutStatusCheck > 0));
    
    if(timeOutStatusCheck == (0UL))
    {
        HandleError();       
    }
    
    return status;
}

/**
 *	\brief Send a single byte to the ADS1299 without manipulating chip select.
 *
 *	Use this function when multiple bytes need to be sent and you want the chip to remain selected
 *	throughout the process.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param opcode The opcode to send.
 * \return Zero if successful, or an error code if unsuccessful.
 */
uint32_t ads1299_send_byte_no_cs(uint8_t opcode)
{
    
    uint32_t status = TRANSFER_ERROR;
    cy_en_scb_spi_status_t errorStatus;
    
    /* Initiate SPI Master write and read transaction. */
    errorStatus = spi_put(SPI_ADDRESS, opcode);
    
    /* If no error wait till master sends data in Tx FIFO */
    if(errorStatus == CY_SCB_SPI_SUCCESS)
    {
        uint32_t masterStatus;        
        uint32_t timeOut = 1000UL;
       
        do
        {
            masterStatus  = SPI_GetTransferStatus();
            Cy_SysLib_Delay(CY_SCB_WAIT_1_UNIT);
            timeOut--;
            //printf("test");
        } while ((0UL != (masterStatus & CY_SCB_SPI_TRANSFER_ACTIVE)) && (timeOut > 0UL));
    
    }
    
    printf("%#08x ", errorStatus);   
    return (status);
}

            
        /*
        if ((0UL == (MASTER_ERROR_MASK & masterStatus)) &&
            (TX_PACKET_SIZE == SPI_GetNumTransfered()))
        {
            status = TRANSFER_CMPLT;
            
        }
        else 
        {
            HandleError();
        }  */


void HandleError(void)
{   
     /* Disable all interrupts. */
    //__disable_irq();
    
    /* Infinite loop. */
    //while(1u) {}
    return;
    
}