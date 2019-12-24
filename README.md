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
1) receive data from ADS1299 using SPI
2) send data to mobile phone using BLE

To get data from our ADC, we used finished driver with functions, which allows to communicate with other microcontroller, and adapted it to our PSoC 6.
Our firmware also allows us to use BLE to send data to another device. In order to do that we had write functions, which will be able to update our characteristics with new data and to react to different BLE events.
