/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_spi_H)
#define CY_SPIM_PVT_spi_H

#include "spi.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 spi_swStatusTx;
extern volatile uint8 spi_swStatusRx;

#if(spi_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 spi_txBuffer[spi_TX_BUFFER_SIZE];
    extern volatile uint8 spi_txBufferRead;
    extern volatile uint8 spi_txBufferWrite;
    extern volatile uint8 spi_txBufferFull;
#endif /* (spi_TX_SOFTWARE_BUF_ENABLED) */

#if(spi_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint8 spi_rxBuffer[spi_RX_BUFFER_SIZE];
    extern volatile uint8 spi_rxBufferRead;
    extern volatile uint8 spi_rxBufferWrite;
    extern volatile uint8 spi_rxBufferFull;
#endif /* (spi_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_spi_H */


/* [] END OF FILE */
