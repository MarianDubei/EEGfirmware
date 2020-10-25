#include "project.h"
#include <stdlib.h>
#include <math.h>
#include <ads1299.h>

#define NSUM 25

/*
 * Protocol format:
 * 
 * Header - 2 bytes
 * Packet type - 2 bits
 * Packet ID - 14 bits
 * Channel data - 12 bytes (x2)
 * ADC + FIFO Status - 2 bytes
 * CRC - 2 bytes
 * ---
 * 20 bytes
 */

uint16 generated_number = 0xFFFF;
volatile uint8_t ble_out_array[20];
uint16 generated_numberOld = 0xFFFF;
volatile uint8_t spi_rx_ads1299_status[3];
volatile uint8_t spi_rx_data_buffer[DSP_PREBUFFER_NB_SAMPLES][MAX_EEG_CHANNELS][3];
int numberNotify;



/***************************************************************
 * Function to update the CapSesnse state in the GATT database
 **************************************************************/
void updateNumber()
{
    if(Cy_BLE_GetConnectionState(cy_ble_connHandle[0]) != CY_BLE_CONN_STATE_CONNECTED)
        return;
    
	cy_stc_ble_gatt_handle_value_pair_t 	tempHandle;
    
    tempHandle.attrHandle = CY_BLE_BRAINWAVES_BRAINWAVESCHAR_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *)&ble_out_array;
    tempHandle.value.len = 20;
    
    
    cy_stc_ble_gatts_db_attr_val_info_t attrValueParam = {tempHandle, 0, cy_ble_connHandle[0], CY_BLE_GATT_DB_LOCALLY_INITIATED};
    Cy_BLE_GATTS_WriteAttributeValue(&attrValueParam);
    
    /* send notification to client if notifications are enabled and finger location has changed */
    //if (numberNotify && (generated_numberOld != generated_number))
    if (numberNotify)
    {
        cy_stc_ble_gatts_handle_value_ntf_t tempHandleNtf;
        tempHandleNtf.handleValPair = tempHandle;
        tempHandleNtf.connHandle = cy_ble_connHandle[0];
        Cy_BLE_GATTS_Notification(&tempHandleNtf);
        generated_numberOld = generated_number;
        
    }    
}

/***************************************************************
 * Function to handle the BLE stack
 **************************************************************/
void BleCallBack(uint32 event, void* eventParam)
{
    cy_stc_ble_gatts_write_cmd_req_param_t *wrReqParam;

    switch(event)
    {
        /* if there is a disconnect or the stack just turned on from a reset then start the advertising and turn on the LED blinking */
        case CY_BLE_EVT_STACK_ON:
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            numberNotify = 0;
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;
        
        /* when a connection is made, update the LED and Capsense states in the GATT database and stop blinking the LED */    
        case CY_BLE_EVT_GATT_CONNECT_IND:
            updateNumber();  
		break;

        /* handle a write request */
        case CY_BLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParam;
			      
            /* request write the LED value */
            if(wrReqParam->handleValPair.attrHandle == CY_BLE_BRAINWAVES_BRAINWAVESCHAR_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                cy_stc_ble_gatts_db_attr_val_info_t attrValueParam = {wrReqParam->handleValPair, 0, wrReqParam->connHandle, CY_BLE_GATT_DB_PEER_INITIATED};
                if(CY_BLE_GATT_ERR_NONE == Cy_BLE_GATTS_WriteAttributeValue(&attrValueParam))
                {
                    Cy_BLE_GATTS_WriteRsp(wrReqParam->connHandle);
                }
            }
            
            /* request to update the CapSense notification */
            if(wrReqParam->handleValPair.attrHandle == CY_BLE_BRAINWAVES_BRAINWAVESCHAR_BRAINWAVESCCCD_DESC_HANDLE) 
            {
                cy_stc_ble_gatts_db_attr_val_info_t attrValueParamNotify = {wrReqParam->handleValPair, 0, wrReqParam->connHandle, CY_BLE_GATT_DB_PEER_INITIATED};
                Cy_BLE_GATTS_WriteAttributeValue(&attrValueParamNotify);
                numberNotify = wrReqParam->handleValPair.value.val[0] & 0x01;
                Cy_BLE_GATTS_WriteRsp(wrReqParam->connHandle);
            }		
			break;  
        
        default:
            break;
    }
} 

int main(void)
{
    SPI_Start();
    // Enable global interrupts.
    __enable_irq();
    
    CLK_Enable();
    CLK_1_Enable();
    UART_Start();
    UART_PutString("alive");
    setvbuf(stdin, NULL, _IONBF, 0);
    
    // Start BLE stack and register the callback function
    Cy_BLE_Start(BleCallBack);

    UART_PutString("ads started ");
    ads1299_device_init(SPI_ADS1299_MAIN_CHIPNUM, 1);
    ads1299_soft_start_conversion(SPI_ADS1299_MAIN_CHIPNUM);
    ads1299_start_rdatac(SPI_ADS1299_MAIN_CHIPNUM);
    
    /*ads1299_rdata24_generic(SPI_ADS1299_MAIN_CHIPNUM, 0, spi_rx_ads1299_status, spi_rx_data_buffer);
    ads1299_stop_rdatac(SPI_ADS1299_MAIN_CHIPNUM);
    ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CONFIG2, 0xd0);
    ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH1SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH2SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH3SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH4SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH5SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH6SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH7SET, 0x05);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH8SET, 0x05);	
    ads1299_start_rdatac(SPI_ADS1299_MAIN_CHIPNUM);
    ads1299_rdata24_generic(SPI_ADS1299_MAIN_CHIPNUM, 0, spi_rx_ads1299_status, spi_rx_data_buffer);
    
    ads1299_stop_rdatac(SPI_ADS1299_MAIN_CHIPNUM);
    ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH1SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH2SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH3SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH4SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH5SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH6SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH7SET, 0x00);
	ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_REGADDR_CH8SET, 0x00);	
    ads1299_start_rdatac(SPI_ADS1299_MAIN_CHIPNUM);*/
    
    for (int m = 0; m < DSP_PREBUFFER_NB_SAMPLES; m++) {
		for (int n = 0; n < MAX_EEG_CHANNELS; n++) {
            for (int j=0; j<3; j++){
                spi_rx_data_buffer[m][n][j] = 0;
            }
		}
	}
    
    // Header
    ble_out_array[0] = 0xEE;
    ble_out_array[1] = 0xD5;
    
    uint8 packet_id = 0x00;
        
    for(;;)
    {
        // Getting data using SPI
            // change 0 to idx for saving samples
        //ads1299_send_byte(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_OPC_RDATA);
        //UART_PutString(" reading ");    
        ads1299_rdata24_generic(SPI_ADS1299_MAIN_CHIPNUM, 0, spi_rx_ads1299_status, spi_rx_data_buffer);
        
        /*int k = 0;
        while (spi_data_ready(SPI_ADS1299_MAIN_CHIPNUM) == 0) {
            k++;
        }*/
        
        //CyDelay(1000);
        // Preparing BLE packet
        for (int i=0; i<2; i++){
            
            // Packet id and type
            // 2 bytes = 4 bits type, 12 bits id
            
            ble_out_array[2] = (uint8)i;
            ble_out_array[3] = packet_id;
            packet_id += 1;
            
            // Channel data
            for (int j=0; j<12; j++){
                ble_out_array[j+4] = spi_rx_data_buffer[0][i*4 + (int)j/3][(int)j%3];
            }
            
            // ADC
            ble_out_array[16] = 0xAD;
            
            // FIFO
            ble_out_array[17] = 0xFA;
            
            // CRC
            //
        
            updateNumber(); 

            Cy_BLE_ProcessEvents();
            Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
        }
        
    }
}

/* [] END OF FILE */
