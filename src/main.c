#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_adc.h"
#include "app_vbat.h"

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
	const struct device *dev;
	uint16_t vbat;
	cnt = 0;

	// initialization of all devices
	app_stm32_vbat_init(dev);
	
	printk("Battery Level Measurement\nBoard: %s\n", CONFIG_BOARD);

	// beginning of isr timer
	k_timer_start(&adc_timer, K_NO_WAIT, K_MSEC(5));		// for test

	while (1) {
		// counter value for waiting 10 min -> 600000ms
		if (cnt >= 600000) {
			vbat = app_stm32_get_vbat(dev);
			printk("stm32 vbat: %"PRIu16"\n", vbat);
			cnt = 0;
		}	
	}	
	return 0;
}

