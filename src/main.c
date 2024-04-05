#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_lorawan.h"

//  ======== interrupt sub-routine ===============================
void geo_work_handler(struct k_work *work_geo)
{
	const struct device *rom_dev = NULL;
	uint8_t payload[2];

	printk("ADC handler called\n");
	app_rom_handler(rom_dev);
}
K_WORK_DEFINE(geo_work, geo_work_handler);

void geo_timer_handler(struct k_timer *geo_dum)
{
    printk("ADC timer called\n");
	k_work_submit(&geo_work);
}
K_TIMER_DEFINE(geo_timer, geo_timer_handler, NULL);

void sens_work_handler(struct k_work *work_rtc)
{
const struct device *flash_dev = NULL;

	printk("sensor handler called\n");
	app_flash_handler(flash_dev);
}
K_WORK_DEFINE(sens_work, sens_work_handler);

void sens_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&sens_work);
}
K_TIMER_DEFINE(sens_timer, sens_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *dev = NULL;

	// initialization of all devices
	app_bme280_init(dev);
	app_stm32_vbat_init(dev);
	app_rom_init(dev);
	app_flash_init(dev);
	
	// beginning of isr timer
	k_timer_start(&sens_timer, K_SECONDS(600), K_SECONDS(600));		// for test
	k_timer_start(&geo_timer, K_MSEC(5), K_MSEC(5));				// final value
	
	return 0;
}