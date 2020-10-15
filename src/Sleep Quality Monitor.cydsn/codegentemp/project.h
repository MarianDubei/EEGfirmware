/*******************************************************************************
* File Name: project.h
* 
* PSoC Creator  4.2
*
* Description:
* It contains references to all generated header files and should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cyble_416045_02.h"
#include "cy_efuse.h"
#include "cy_flash.h"
#include "cy_gpio.h"
#include "cy_ipc_drv.h"
#include "cy_ipc_sema.h"
#include "cy_ipc_pipe.h"
#include "cy_lvd.h"
#include "cy_profile.h"
#include "cy_prot.h"
#include "cy_scb_common.h"
#include "cy_scb_i2c.h"
#include "cy_scb_ezi2c.h"
#include "cy_scb_spi.h"
#include "cy_scb_uart.h"
#include "cy_sysanalog.h"
#include "cy_sysclk.h"
#include "cy_sysint.h"
#include "cy_syslib.h"
#include "cy_syspm.h"
#include "cy_systick.h"
#include "cy_trigmux.h"
#include "cy_wdt.h"
#include "cy_device.h"
#include "ble/cy_ble.h"
#include "ble/cy_ble_hal_pvt.h"
#include "ble/cy_ble_defines.h"
#include "ble/cy_ble_gap.h"
#include "ble/cy_ble_gatt.h"
#include "ble/cy_ble_event_handler.h"
#include "ble/cy_ble_stack.h"
#include "ble/cy_ble_stack_host_main.h"
#include "ble/cy_ble_stack_gatt_server.h"
#include "ble/cy_ble_stack_gap_peripheral.h"
#include "ble/cy_ble_stack_gatt_client.h"
#include "ble/cy_ble_stack_l2cap.h"
#include "ble/cy_ble_stack_pvt.h"
#include "ble/cy_ble_stack_gatt_db.h"
#include "ble/cy_ble_stack_gap_central.h"
#include "ble/cy_ble_stack_gap.h"
#include "ble/cy_ble_stack_gatt.h"
#include "ble/cy_ble_stack_host_error.h"
#include "ble/cy_ble_ancs.h"
#include "ble/cy_ble_ans.h"
#include "ble/cy_ble_aios.h"
#include "ble/cy_ble_bas.h"
#include "ble/cy_ble_bcs.h"
#include "ble/cy_ble_bls.h"
#include "ble/cy_ble_bms.h"
#include "ble/cy_ble_bts.h"
#include "ble/cy_ble_cgms.h"
#include "ble/cy_ble_cps.h"
#include "ble/cy_ble_cscs.h"
#include "ble/cy_ble_cts.h"
#include "ble/cy_ble_custom.h"
#include "ble/cy_ble_dis.h"
#include "ble/cy_ble_ess.h"
#include "ble/cy_ble_gls.h"
#include "ble/cy_ble_hids.h"
#include "ble/cy_ble_hps.h"
#include "ble/cy_ble_hrs.h"
#include "ble/cy_ble_hts.h"
#include "ble/cy_ble_ias.h"
#include "ble/cy_ble_ips.h"
#include "ble/cy_ble_lls.h"
#include "ble/cy_ble_lns.h"
#include "ble/cy_ble_ndcs.h"
#include "ble/cy_ble_plxs.h"
#include "ble/cy_ble_pass.h"
#include "ble/cy_ble_rscs.h"
#include "ble/cy_ble_rtus.h"
#include "ble/cy_ble_scps.h"
#include "ble/cy_ble_tps.h"
#include "ble/cy_ble_uds.h"
#include "ble/cy_ble_wpts.h"
#include "ble/cy_ble_wss.h"
#include "ble/cy_ble_clk.h"
#if ((__CORTEX_M == 0) || (__CORTEX_M == 4))
    #include "psoc6_01_config.h"
#endif /*(__CORTEX_M == 0) || (__CORTEX_M == 4)*/
#include "cy_device_headers.h"
#if ((__CORTEX_M == 0) || (__CORTEX_M == 4))
    #include "gpio_psoc6_01_43_smt.h"
#endif /*(__CORTEX_M == 0) || (__CORTEX_M == 4)*/
#include "cyip_backup.h"
#include "cyip_ble.h"
#include "cyip_cpuss.h"
#include "cyip_cpuss_v2.h"
#include "cyip_crypto.h"
#include "cyip_crypto_v2.h"
#include "cyip_csd.h"
#include "cyip_ctbm.h"
#include "cyip_ctdac.h"
#include "cyip_dmac_v2.h"
#include "cyip_dw.h"
#include "cyip_dw_v2.h"
#include "cyip_efuse.h"
#if ((__CORTEX_M == 0) || (__CORTEX_M == 4))
    #include "cyip_efuse_data_psoc6_01.h"
#endif /*(__CORTEX_M == 0) || (__CORTEX_M == 4)*/
#include "cyip_fault.h"
#include "cyip_fault_v2.h"
#include "cyip_flashc.h"
#include "cyip_flashc_v2.h"
#include "cyip_gpio.h"
#include "cyip_gpio_v2.h"
#include "cyip_headers.h"
#include "cyip_hsiom.h"
#include "cyip_hsiom_v2.h"
#include "cyip_i2s.h"
#include "cyip_ipc.h"
#include "cyip_ipc_v2.h"
#include "cyip_lcd.h"
#include "cyip_lpcomp.h"
#include "cyip_pass.h"
#include "cyip_pdm.h"
#include "cyip_peri.h"
#include "cyip_peri_ms_v2.h"
#include "cyip_peri_v2.h"
#include "cyip_profile.h"
#include "cyip_prot.h"
#include "cyip_prot_v2.h"
#include "cyip_sar.h"
#include "cyip_scb.h"
#include "cyip_sdhc.h"
#include "cyip_sflash.h"
#include "cyip_smartio.h"
#include "cyip_smartio_v2.h"
#include "cyip_smif.h"
#include "cyip_srss.h"
#include "cyip_tcpwm.h"
#include "cyip_udb.h"
#include "cyip_usbfs.h"
#include "cyfitter_cfg.h"
#include "cyfitter_sysint_cfg.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cyfitter_gpio.h"
#include "cyfitter_sysint.h"
#include "cydisabledsheets.h"
#include "BLE.h"
#include "BLE_config.h"
#include "SPI.h"
#include "UART.h"
#include "CLK.h"
#include "CLK_1.h"
#include "UART_SCBCLK.h"

/*[]*/

