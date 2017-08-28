/*
 * GccApplication1.c
 *
 * Created: 28/08/2017 10:16:00
 * Author : michele
 */ 

 #define F_CPU 20000000


#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0b00000000);
	while((CLKCTRL_MCLKSTATUS & 0b00010000) != 0b00010000);

	PORTC.DIRSET = (1<<0);

    while (1) 
    {
		PORTC.OUTSET = (1<<0);
		_delay_ms(500);
		PORTC.OUTCLR = (1<<0);
		_delay_ms(500);
    }

}

