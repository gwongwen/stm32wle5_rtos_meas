# Code for 6sens Project : measurement of battery life

## Overview
This code allows us to evaluate the battery life. The characteristics of the test are as follows:

- for ADC, no threshold so no data is written to external memory
- for the sensor, 1 sample of battery level is taken and printed in console every 10 minutes
- for the battery life, increase a counter and store in a nvs partition. We can know ife time of the battery

The battery used has the following characteristics: 

- Li-Polymer battery
- 3.7V, 3000mAH / 6000mAH

The solar panel used has the following characteristics :

- size of 100x80x2.5mm
- 1W
- 170mA of current, 5.5V of voltage, at peak power

## Building and Running
The following commands clean build folder, build and flash the sample:

**Command to use**

west build -t pristine

west build -p always -b stm32wl_dw1000_iot_board applications/stm32wle5_rtos_meas

west flash --runner stm32cubeprogrammer