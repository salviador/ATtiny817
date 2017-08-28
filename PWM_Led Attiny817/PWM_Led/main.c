#include <atmel_start.h>
#include <avr/delay.h>


volatile uint8_t i;
int main(void)
{


	/* Initializes MCU, drivers and middleware */
	atmel_start_init();


	
	// Enable pin output
	PWM_0_enable_output_ch0();

	// Set channel 0 duty cycle value register value to 0x3f
	PWM_0_load_duty_cycle_ch0(5);

	// Set counter register value to 0
	PWM_0_load_counter(0);
	
		
		
	//PORTMUX.CTRLD = 0;
	//PORTC.DIR = 0b1;


	while (1) {
		for(i=0;i<255;i++){
			PWM_0_load_duty_cycle_ch0(i);
			PWM_0_load_counter(255);
			_delay_ms(50);
		}


	
	}

}
