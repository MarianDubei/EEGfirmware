/*******************************************************************************
* File Name: spi_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "spi_PVT.h"
#include "cyapicallbacks.h"

/* User code required at start of ISR */
/* `#START spi_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: spi_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  spi_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  spi_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  spi_txBuffer[spi_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(spi_TX_ISR)
{
    #if(spi_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (spi_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef spi_TX_ISR_ENTRY_CALLBACK
        spi_TX_ISR_EntryCallback();
    #endif /* spi_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START spi_TX_ISR_START` */

    /* `#END` */
    
    #if(spi_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(spi_txBufferRead != spi_txBufferWrite)
        {
            tmpStatus = spi_GET_STATUS_TX(spi_swStatusTx);
            spi_swStatusTx = tmpStatus;

            if(0u != (spi_swStatusTx & spi_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == spi_txBufferFull)
                {
                   spi_txBufferRead++;

                    if(spi_txBufferRead >= spi_TX_BUFFER_SIZE)
                    {
                        spi_txBufferRead = 0u;
                    }
                }
                else
                {
                    spi_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(spi_TXDATA_PTR, 
                                             spi_txBuffer[spi_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(spi_txBufferRead == spi_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            spi_TX_STATUS_MASK_REG &= ((uint8) ~spi_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (spi_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START spi_TX_ISR_END` */

    /* `#END` */
    
    #ifdef spi_TX_ISR_EXIT_CALLBACK
        spi_TX_ISR_ExitCallback();
    #endif /* spi_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: spi_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  spi_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  spi_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  spi_rxBuffer[spi_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(spi_RX_ISR)
{
    #if(spi_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (spi_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef spi_RX_ISR_ENTRY_CALLBACK
        spi_RX_ISR_EntryCallback();
    #endif /* spi_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START spi_RX_ISR_START` */

    /* `#END` */
    
    #if(spi_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = spi_GET_STATUS_RX(spi_swStatusRx);
        spi_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (spi_swStatusRx & spi_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(spi_RXDATA_PTR);

            /* Set next pointer. */
            spi_rxBufferWrite++;
            if(spi_rxBufferWrite >= spi_RX_BUFFER_SIZE)
            {
                spi_rxBufferWrite = 0u;
            }

            if(spi_rxBufferWrite == spi_rxBufferRead)
            {
                spi_rxBufferRead++;
                if(spi_rxBufferRead >= spi_RX_BUFFER_SIZE)
                {
                    spi_rxBufferRead = 0u;
                }

                spi_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            spi_rxBuffer[spi_rxBufferWrite] = rxData;

            tmpStatus = spi_GET_STATUS_RX(spi_swStatusRx);
            spi_swStatusRx = tmpStatus;
        }

    #endif /* (spi_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START spi_RX_ISR_END` */

    /* `#END` */
    
    #ifdef spi_RX_ISR_EXIT_CALLBACK
        spi_RX_ISR_ExitCallback();
    #endif /* spi_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
