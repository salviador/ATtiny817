#include <atmel_start.h>

int main(void)
{
	uint8_t key_status = 0;

	atmel_start_init();
		/* Enable interrupts */
		SREG |= 0x80;
	while (1) {

		touch_process();

		key_status = get_sensor_state(0) & 0x80;
		if (0u != key_status) {
			LED_set_level(1);
			} else {
			LED_set_level(0);
		}
		/*
		key_status = get_sensor_state(1) & 0x80;
		if (0u != key_status) {
			LED_set_level(1);
			} else {
			LED_set_level(0);
		}*/


	}
}
