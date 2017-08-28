#include <atmel_start.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

uint8_t key_status = 0;
unsigned char fstate;

unsigned char fstate;
unsigned char mDIMMER,sPressTime,fsegno,tGeneral8bit;
unsigned int tPressTime;

uint32_t millis(void);

uint32_t _mstime=0;
ISR(TCA0_CMP0_vect){
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
 TCA0.SINGLE.CNT = 0;
 _mstime++;
 tGeneral8bit++;
         if(sPressTime){
	         tPressTime++;
         }
}

ISR(TCA0_OVF_vect){
	
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;

}


int main(void)
{
	uint32_t t1,t2;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// Enable pin output
	PWM_0_enable_output_ch0();

	// Set channel 0 duty cycle value register value to 0x3f
	PWM_0_load_duty_cycle_ch0(5);

	// Set counter register value to 0
	PWM_0_load_counter(0);

	/* Enable interrupts */
	SREG |= 0x80;
	    
		
	fstate=0;
	sPressTime=0;
	tPressTime=0;
	fsegno=0;
	mDIMMER=255;


	while (1) {
		touch_process();
		
		
		//program
		switch(fstate){
			case 0:
			//se lampada è spenta
			//attendi tasto on
			key_status = get_sensor_state(0) & 0x80;
			if(key_status!=0){
				//Accendi
				PWM_0_load_duty_cycle_ch0(mDIMMER);
				PWM_0_load_counter(255);
				fstate=1;
			}
			break;
			
			case 1:
			//se lampada è accesa
			//attendi tasto rilascio
			key_status = get_sensor_state(0) & 0x80;
			if(key_status==0){
				fstate=2;
			}
			break;
			
			case 2:
			//se lampada è accesa
			//attendi pressione x spegnere o x programmazione
			key_status = get_sensor_state(0) & 0x80;
			if(key_status!=0){
				fstate=3;
				//start timer
				sPressTime=0;
				tPressTime=0;
				sPressTime=1;
			}
			break;
			
			case 3:
			//se lampada è accesa
			//pressione tasto = on ,  spegnere o x programmazione
			key_status = get_sensor_state(0) & 0x80;
			if(key_status==0){
				sPressTime=0;
				tPressTime=0;
				fstate=4;
				// spegni
			}
			sPressTime=0;
			if(tPressTime>1000){
				//vai a programmazione
				fstate=5;
				tPressTime=0;
				tGeneral8bit=0;
				}else{
				sPressTime=1;
			}
			break;
			
			case 4:
			//se lampada è accesa
			//spegni
				PWM_0_load_duty_cycle_ch0(0);
				PWM_0_load_counter(255);
				fstate=0;
			break;
			
			case 5:
			//se lampada è accesa e in programmazione
			key_status = get_sensor_state(0) & 0x80;
			if(key_status!=0){
				//dimmera
				if(tGeneral8bit>25){
					tGeneral8bit=0;
					if(fsegno==1){
						mDIMMER++;
						if(mDIMMER>=102){
							mDIMMER = 102;
						}
						}else{
						if(mDIMMER>=2){
							mDIMMER--;
						}
					}
				PWM_0_load_duty_cycle_ch0(mDIMMER);
				PWM_0_load_counter(255);
				}
				}else{
				//exit programmazione
				if(fsegno==1){
					fsegno=0;
					}else{
					fsegno=1;
				}
				fstate=2;
			}
			break;
		}
		
		
		
		











		/*
		key_status = get_sensor_state(0) & 0x80;
		if (0u != key_status) {
			PWM_0_load_duty_cycle_ch0(5);
			PWM_0_load_counter(255);
			} else {
			PWM_0_load_duty_cycle_ch0(155);
			PWM_0_load_counter(255);
		}
		*/

	}

}







uint32_t millis(void){
	uint32_t m;
	cli();
	m = _mstime;
	sei();
	return m;
}
