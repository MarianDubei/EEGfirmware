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

#define RX_PACKET_SIZE  27
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
    cy_en_scb_spi_status_t transfer_status;
    transfer_status = SPI_Transfer(txBuffer, NULL, ONE_PACKET_SIZE);
    //CyDelay(10);
    return transfer_status;
}

cy_en_scb_spi_status_t spi_get(volatile uint8_t *spi, uint8_t rxBuffer[])
{
    cy_en_scb_spi_status_t transferStatus;
    transferStatus = SPI_Transfer(NULL, rxBuffer, ONE_PACKET_SIZE);
    //CyDelay(10);
    printf("%x ", rxBuffer[0]);
    return transferStatus;
}

void spi_selectChip(volatile uint8_t *spi, uint8_t chip_select)
{   
    gpio_clr_gpio_pin(ADS1299_PIN_CS);
    // CyDelayUs(1);
}

void spi_unselectChip(volatile uint8_t *spi, uint8_t chip_select)
{   
    // CyDelayUs(1);
    gpio_set_gpio_pin(ADS1299_PIN_CS);
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
        case ADS1299_PIN_CS:
            Cy_GPIO_Set(SPI_ss0_m_0_PORT, SPI_ss0_m_0_NUM);
        break;
        case ADS1299_PIN_CLKSEL:
            Cy_GPIO_Set(PIN_ADS1299_CLKSEL_0_PORT, PIN_ADS1299_CLKSEL_0_NUM);
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
        case ADS1299_PIN_CS:
            Cy_GPIO_Clr(SPI_ss0_m_0_PORT, SPI_ss0_m_0_NUM);
        break;
    }
}


uint32_t spi_write_packet(uint8_t txBuffer[], uint8_t packet_size)
{
 
    uint32_t status = TRANSFER_ERROR;
    cy_en_scb_spi_status_t errorStatus;
    uint8_t statusRxBuf[packet_size];
    
    do
    {
        
                
        // Initiate SPI Master write and read transaction.
        //printf(" %x ", txBuffer[0]);
        errorStatus = SPI_Transfer(txBuffer, statusRxBuf, packet_size);
        
        /* If no error wait till master sends data in Tx FIFO */
        if(errorStatus == CY_SCB_SPI_SUCCESS)
        {
            uint32_t masterStatus;        
            // Timeout 1 sec
            uint32_t timeOut = 1000UL;
            
            // Wait until master complete read transfer or time out has occured
            do
            {
                masterStatus  = SPI_GetTransferStatus();
                Cy_SysLib_Delay(CY_SCB_WAIT_1_UNIT);
                timeOut--;
                
            } while ((0UL != (masterStatus & CY_SCB_SPI_TRANSFER_ACTIVE)) && (timeOut > 0UL));
            
            if ((0UL == (MASTER_ERROR_MASK & masterStatus)) &&
                (packet_size == SPI_GetNumTransfered()))
            {
                status = TRANSFER_CMPLT;
                
            }
            else 
            {
                HandleError();
            } 
        }
        
    } while (status != TRANSFER_CMPLT);
    
    return (status);
}


uint32_t spi_read_packet(uint8_t data[], uint8_t packet_size)
{
    uint32_t status = TRANSFER_ERROR;
    
    /*printf("in_s ");
    for (int i = 0; i<packet_size; i++) {
        printf("%x ", data[i]);
    }
    printf("in_end ");*/
    
    do
    {
    	uint8_t count = -1;
        uint8_t tempVar = 0;
        uint32_t timeOutStatusCheck = 1000UL; /* Timeout 1 sec */
        cy_en_scb_spi_status_t errorStatus;
        
        
        /* Initiate SPI Master write and read transaction. Master will send one byte and receives one byte.
           Received byte is checked whether valid status byte is received. Till valid 5 bytes are received or timeout
           occurs, master will be sending dummy byte to slave. Valid bytes received is checked and status is set. */
        do
        {
            /*uint8_t tx_buf[packet_size];
            for (int i=0;i<packet_size;i++){
                tx_buf[i] = 0xFF;
            }
            errorStatus = SPI_Transfer(tx_buf, data, packet_size);*/
            uint8_t tx = 0x00;
            
            errorStatus = SPI_Transfer(&tx, &tempVar, ONE_PACKET_SIZE);
            
            if(errorStatus == CY_SCB_SPI_SUCCESS)
            {
                uint32_t masterStatus;         
                // Timeout 1 sec
                uint32_t timeOut = 1000UL;
                
                // Wait until master complete read transfer or time out has occured
                do
                {
                    masterStatus  = SPI_GetTransferStatus();
                    //Cy_SysLib_Delay(CY_SCB_WAIT_1_UNIT);
                    timeOut--;
                    
                } while ((0UL != (masterStatus & CY_SCB_SPI_TRANSFER_ACTIVE)) && (timeOut > 0UL) );
                
                //if ((0UL == (MASTER_ERROR_MASK & masterStatus)) && (tempVar != 0xFF) )
                if ((0UL == (MASTER_ERROR_MASK & masterStatus)))
                {  
                    
                    data[count] = tempVar;
                    count += 1;
             
                    if(count == packet_size)
                    {  
                        status = TRANSFER_CMPLT; 
                        //UART_PutString(" got it ");
                    }
                }
                /*else 
                {
                    HandleError();
                }*/
            }   
            
            Cy_SysLib_Delay(CY_SCB_WAIT_1_UNIT);
            timeOutStatusCheck--;
        
        }while((count < packet_size) && (timeOutStatusCheck > 0));
         
        /*if(timeOutStatusCheck == (0UL))
        {
            HandleError();       
        }*/
    } while (status != TRANSFER_CMPLT);
    
    printf("out_s ");
    for (int i = 0; i<packet_size; i++) {
        printf("%x ", data[i]);
    }
    printf("out_end     ");
    
    return status;
}


void HandleError(void)
{   
     /* Disable all interrupts. */
    //__disable_irq();
    
    /* Infinite loop. */
    //while(1u) {}
    return;
    
}