#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_adc.h"
#include "app_vbat.h"

//  ======== interrupt sub-routine ===============================
void vbat_work_handler(struct k_timer *work)
{
	const struct device *dev;
	uint16_t vbat;

	vbat = app_stm32_get_vbat(dev);
	printk("stm32 vbat: %d\n", vbat);
}
K_WORK_DEFINE(vbat_work, vbat_work_handler);

void vbat_timer_handler(struct k_timer *timer)
{
	k_work_submit(&vbat_work);
}
K_TIMER_DEFINE(vbat_timer, vbat_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *dev;
	uint16_t value;

	// initialization of all devices
	app_stm32_vbat_init(dev);
	
	printk("Battery Level Measurement\nBoard: %s\n", CONFIG_BOARD);

	// beginning of isr timer
	k_timer_start(&vbat_timer, K_NO_WAIT, K_SECONDS(5));		// for test
	while (1) {
		value = app_adc_get_val();
		k_msleep(1000);
	}
	return 0;
}

