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
#include <project.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ads1299.h>


#define NSUM 25

uint16 generated_number = 0xFFFF;
uint16 generated_numberOld = 0xFFFF;
volatile uint8_t spi_rx_ads1299_status[3];
volatile uint8_t spi_rx_data_buffer[DSP_PREBUFFER_NB_SAMPLES][MAX_EEG_CHANNELS][3];
int numberNotify;

/***************************************************************
 * Function to update the LED state in the GATT database
 **************************************************************/


void updateLed()
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
   
    uint8 red_State = !red_Read();
    
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_LEDCAPSENSE_LED_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *) &red_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

/***************************************************************
 * Function to update the CapSesnse state in the GATT database
 **************************************************************/
void updateNumber()
{
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
	CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
    
    tempHandle.attrHandle = CYBLE_LEDCAPSENSE_CAPSENSE_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *)&generated_number;
    tempHandle.value.len = 2; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED );  
    
    /* send notification to client if notifications are enabled and finger location has changed */
    //if (numberNotify && (generated_numberOld != generated_number))
    if (numberNotify)
    {
        CyBle_GattsNotification(cyBle_connHandle,&tempHandle);
        generated_numberOld = generated_number;
    }    
}

/***************************************************************
 * Function to handle the BLE stack
 **************************************************************/
void BleCallBack(uint32 event, void* eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;

    switch(event)
    {
        /* if there is a disconnect or the stack just turned on from a reset then start the advertising and turn on the LED blinking */
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            numberNotify = 0;
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            pwm_Start();
        break;
        
        /* when a connection is made, update the LED and Capsense states in the GATT database and stop blinking the LED */    
        case CYBLE_EVT_GATT_CONNECT_IND:
            updateLed();
            updateNumber();  
            pwm_Stop();
		break;

        /* handle a write request */
        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
			      
            /* request write the LED value */
            if(wrReqParam->handleValPair.attrHandle == CYBLE_LEDCAPSENSE_LED_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    red_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                }
            }
            
            /* request to update the CapSense notification */
            if(wrReqParam->handleValPair.attrHandle == CYBLE_LEDCAPSENSE_CAPSENSE_CAPSENSECCCD_DESC_HANDLE) 
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                numberNotify = wrReqParam->handleValPair.value.val[0] & 0x01;
                CyBle_GattsWriteRsp(cyBle_connHandle);
            }		
			break;  
        
        default:
            break;
    }
} 

/***************************************************************
 * Main
 **************************************************************/
int main()
{
    CyGlobalIntEnable; 
    
    spi_Start();
    Clock_1_Start();
    Clock_2_Start();
    /* Start BLE stack and register the callback function */
    
    CyBle_Start(BleCallBack);

    ads1299_device_init(SPI_ADS1299_MAIN_CHIPNUM, 0);
    ads1299_send_byte(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_OPC_RDATAC);
 	ads1299_soft_start_conversion(SPI_ADS1299_MAIN_CHIPNUM);
    
    int counter = 0;
    
    uint8_t* num;
    
    for (int m = 0; m < DSP_PREBUFFER_NB_SAMPLES; m++) {
		for (int n = 0; n < MAX_EEG_CHANNELS; n++) {
            for (int j=0; j<3; j++){
                spi_rx_data_buffer[m][n][j] = 0;
            }
		}
	}
    
    for(;;)
    {        
               
        ads1299_send_byte(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_OPC_RDATA);
        ads1299_soft_start_conversion(SPI_ADS1299_MAIN_CHIPNUM);
        CyDelay(5);
        ads1299_rdata24_generic(SPI_ADS1299_MAIN_CHIPNUM, 0, spi_rx_ads1299_status, spi_rx_data_buffer);
        

        generated_number = (uint16) (spi_rx_data_buffer[0][3][1] << 8) | (spi_rx_data_buffer[0][3][0]);
        updateNumber();       

        CyBle_ProcessEvents();
        CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
        
    }
}

/* [] END OF FILE */
