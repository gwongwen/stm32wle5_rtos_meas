#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_adc.h"
#include "app_vbat.h"

//  ======== interrupt sub-routine ===============================
void geo_work_handler(struct k_work *work_geo)
{
	uint16_t value;
	value = app_adc_get_val();
}
K_WORK_DEFINE(geo_work, geo_work_handler);

void geo_timer_handler(struct k_timer *geo_dum)
{
	k_work_submit(&geo_work);
}
K_TIMER_DEFINE(geo_timer, geo_timer_handler, NULL);

void vbat_work_handler(struct k_work *work_rtc)
{
	const struct device *dev;
	uint16_t vbat;

	vbat = app_stm32_get_vbat(dev);
	printk("stm32 vbat: %d", vbat);
	
}
K_WORK_DEFINE(vbat_work, vbat_work_handler);

void vbat_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&vbat_work);
}
K_TIMER_DEFINE(vbat_timer, vbat_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *dev = NULL;
	uint16_t adc_val;

	// initialization of all devices
	app_stm32_vbat_init(dev);
	
	// beginning of isr timer
	k_timer_start(&geo_timer, K_MSEC(5), K_MSEC(5));				// for test
	k_timer_start(&vbat_timer, K_SECONDS(300), K_SECONDS(300));		// for test
	
	return 0;
}