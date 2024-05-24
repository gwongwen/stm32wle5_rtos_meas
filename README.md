# Code for 6sens Omnitilt Project : measurement of battery life

## Overview
This code allows us to evaluate the battery life. The characteristics of the test are as follows:

- for ADC, no threshold so no data is written to external memory
- for the sensor, 1 sample of battery level is taken and printed in console every 5 minutes.

The battery used has the following characteristics: 

- Li-Polymer battery
- 3.7V, 3000mAH
- 57 x 63 x 8.1mm

## Building and Running
The following commands clean build folder, build and flash the sample:

**Command to use**

west build -t pristine

west build -p always -b stm32wl_dw1000_iot_board applications/stm32wle5_rtos_meas

west flash --runner stm32cubeprogrammer