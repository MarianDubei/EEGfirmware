# Sleep Quality Monitor | Firmware
___
## Description of the problem
Our task was to create a device which will be able to read and keep electric potential differences from different points on a head and to analyze them.
Besides electrodes, ,which can read electric potential differences, we had to use analog-to-digital converter to transmit signal from electrodes to our microcontroller, which will send this data to phone, where data will be analyzed to presented to a user.
___
## Hardware
We decided to use ADS1299 as analog-to-digital converter, because it is sigma-delta ADC and it allows to read input signals simultaneously. We use PSoC 6 as our microcontroller, which is able to receive data from ADC and send it to a smartphone using BLE.
___
## Firmware
We had to write firmware, which will be able to:
1) receive data from ADS1299 using SPI;
2) send data to mobile phone using BLE;
___
When electric potential differences, which come from head to ADC, are processed, they are stored in its registers, from where we can them using SPI bus. During communication our PSoC 6 and ADC(ADS1299) will be SPI Master and SPI Slave respectively.
To get data from our ADC, we used [finished driver](https://github.com/gskelly/eeg/tree/master/firmware/at32uc3l064/brainboard_fw/brainboard_fw/src) with functions, which allows to communicate with other microcontroller, and adapted it to our PSoC 6.
Functions from driver, which we used:
 - ads1299_device_init(uint8_t chip_select, uint8_t init_regs);
 - ads1299_rreg(uint8_t chip_select, uint8_t reg_addr, uint8_t* read_reg_val_ptr)
 - ads1299_wreg(uint8_t chip_select, uint8_t reg_addr, uint8_t reg_val_to_write)
___
After we received data from ADC, we need to send it to a mobile phone using BLE. PSoC 6(GATT Server, GAP Peripheral) will advertise packets with characteristics, which contain processed data from electrodes to other devices.
Our firmware also allows us to use BLE to send data to another device. In order to do that we had write functions, which will be able to update our characteristics with new data and to react to different BLE events.
Functions we used:
 - updateLed() - update GATT database with the state of LED, which changes if some device is connected to PSoC or not;
 - updateNumber() - update GATT database with the state of data;
 - BleCallBack() - BLE event handler;
 - main() - main loop, where we get data, update it in infinite loop and process BLE events;
___
## Components used in our PSoC Creator Project
 - BLE 5.0
 - SPI Master
___
## References
[ADS1299 Datasheet](http://www.ti.com/lit/ds/symlink/ads1299.pdf) |
 [PSoC SPI Master Datasheet](https://www.cypress.com/file/185446/download) |
 [PSoC Bluetooth Low Energy Datasheet](https://www.cypress.com/file/408651/download)