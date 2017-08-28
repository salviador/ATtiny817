/**
 * \file
 *
 * \brief PWM Basic driver implementation.
 *
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *
 */

#include <pwm_basic.h>

pwm_irq_cb_t PWM_0_cb = NULL;

/**
 * \brief Initialize PWM
 */
int8_t PWM_0_init()
{

	TCB0.CCMPL = 255; /* PWM Period: 255 */

	TCB0.CCMPH = 100; /* PWM Compare: 100 */

	// TCB0.CNT = 0; /* Count: 0 */

	TCB0.CTRLB = 0 << TCB_ASYNC_bp      /* Asynchronous Enable: disabled */
	             | 1 << TCB_CCMPEN_bp   /* Pin Output Enable: enabled */
	             | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
	             | TCB_CNTMODE_PWM8_gc; /* 8-bit PWM */

	// TCB0.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

	// TCB0.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
	//		 | 0 << TCB_EDGE_bp /* Event Edge: disabled */
	//		 | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

	// TCB0.INTCTRL = 0 << TCB_CAPT_bp; /* Setting: disabled */

	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc  /* CLK_PER (No Prescaling) */
	             | 1 << TCB_ENABLE_bp   /* Enable: enabled */
	             | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
	             | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */

	return 0;
}

void PWM_0_enable()
{
	TCB0.CTRLA |= TCB_ENABLE_bm;
}

void PWM_0_disable()
{
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void PWM_0_enable_output_ch0()
{
	TCB0.CTRLA |= TCB_ENABLE_bm;
}

void PWM_0_disable_output_ch0()
{
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void PWM_0_load_counter(PWM_0_register_t counter_value)
{
	TCB0.CNT = counter_value;
}

void PWM_0_load_top(PWM_0_register_t top_value)
{
	TCB0.CCMPL = top_value;
}

void PWM_0_load_duty_cycle_ch0(PWM_0_register_t duty_value)
{
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
	TCB0.CCMPH = duty_value;
	TCB0.CNT   = 0;
	TCB0.CTRLA |= TCB_ENABLE_bm;
}

void PWM_0_register_callback(pwm_irq_cb_t f)
{
	PWM_0_cb = f;
}

ISR(TCB0_INT_vect)
{
	static volatile uint8_t callback_count = 0;

	// Clear the interrupt flag
	TCB0.INTFLAGS = TCB_CAPT_bm;

	// callback function - called every 0 passes
	if ((++callback_count >= PWM_0_INTERRUPT_CB_RATE) && (PWM_0_INTERRUPT_CB_RATE != 0)) {
		if (PWM_0_cb != NULL) {
			PWM_0_cb();
		}
	}
}
