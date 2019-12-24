/*******************************************************************************
* File Name: spi_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "spi_PVT.h"

static spi_BACKUP_STRUCT spi_backup =
{
    spi_DISABLED,
    spi_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: spi_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void spi_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: spi_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void spi_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: spi_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  spi_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void spi_Sleep(void) 
{
    /* Save components enable state */
    spi_backup.enableState = ((uint8) spi_IS_ENABLED);

    spi_Stop();
}


/*******************************************************************************
* Function Name: spi_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  spi_backup - used when non-retention registers are restored.
*  spi_txBufferWrite - modified every function call - resets to
*  zero.
*  spi_txBufferRead - modified every function call - resets to
*  zero.
*  spi_rxBufferWrite - modified every function call - resets to
*  zero.
*  spi_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void spi_Wakeup(void) 
{
    #if(spi_RX_SOFTWARE_BUF_ENABLED)
        spi_rxBufferFull  = 0u;
        spi_rxBufferRead  = 0u;
        spi_rxBufferWrite = 0u;
    #endif /* (spi_RX_SOFTWARE_BUF_ENABLED) */

    #if(spi_TX_SOFTWARE_BUF_ENABLED)
        spi_txBufferFull  = 0u;
        spi_txBufferRead  = 0u;
        spi_txBufferWrite = 0u;
    #endif /* (spi_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    spi_ClearFIFO();

    /* Restore components block enable state */
    if(0u != spi_backup.enableState)
    {
        spi_Enable();
    }
}


/* [] END OF FILE */
