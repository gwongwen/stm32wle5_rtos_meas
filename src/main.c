#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_adc.h"
#include "app_vbat.h"
#include "app_flash.h"

//  ======== globals ============================================
uint16_t cnt;           // declaration of counter index

//  ======== interrupt sub-routine ===============================
void adc_work_handler(struct k_timer *work)
{
	uint16_t value;
	value = app_adc_get_val();
	cnt++;
}
K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *timer)
{
	k_work_submit(&adc_work);
}
K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *vref_dev = NULL;
	const struct device *vbat_dev = NULL;
	struct nvs_fs flash;
	uint16_t vref, vbat;
	cnt = 0;
	uint32_t max_cnt = 0;
	int8_t ret;

	// setup all devices
	vref_dev = DEVICE_DT_GET_ONE(st_stm32_vref);
	vbat_dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
	app_stm32_vref_init(vref_dev);
	app_stm32_vbat_init(vbat_dev);
	app_flash_init(&flash);
	
	printk("Battery Level Measurement\nBoard: %s\n", CONFIG_BOARD);

	// beginning of isr timer
	k_timer_start(&adc_timer, K_NO_WAIT, K_MSEC(5));		// for test

	while (1) {
		// counter value for waiting 10 min -> 600000ms
		if (cnt >= 600000) {
			vref = app_stm32_get_vref(vref_dev);
			vbat = app_stm32_get_vbat(dev, vref);
			// writing data in the first page of 2kbytes
			(void)nvs_write(&flash, NVS_BAT_ID, &vbat, sizeof(vbat));
			
			max_cnt++;
			// writing data in the first page of 2kbytes
			(void)nvs_write(&flash, NVS_SENSOR_ID, &max_cnt, sizeof(max_cnt));
			cnt = 0;
		}
	}
	// reading the first page
	ret = nvs_read(&flash, NVS_SENSOR_ID, &max_cnt, sizeof(max_cnt));
	// printing data stored in memory
	printk("max value of counter: %"PRIu32"\n",max_cnt);

	// reading the first page
	ret = nvs_read(&flash, NVS_BAT_ID, &vbat, sizeof(vbat));
	// printing data stored in memory
	printk("min value of battery: %"PRIu32"\n",vbat);

	return 0;
}

