/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpstate.h"
#include "py/mphal.h"
#include "lib/utils/interrupt_char.h"
#include "atmel_start.h"
#include "usb_micropython.h"

void mp_hal_set_interrupt_char(int c) {
}

void mp_hal_delay_ms(mp_uint_t ms) {
    ms += 1;
    uint32_t t0 = systick_ms;
    while (systick_ms - t0 < ms) {
        MICROPY_EVENT_POLL_HOOK
    }
}

void mp_hal_delay_us(mp_uint_t us) {
    uint32_t ms = us / 1000 + 1;
    uint32_t t0 = systick_ms;
    while (systick_ms - t0 < ms) {
        __WFI();
    }
}

int mp_hal_stdin_rx_chr(void) {
    for (;;) {
	 while(read_done) __WFI();
	read_done = 0;
    	if (RingBuf_IsEmpty(&rbuf_in)) {
		cdcdf_acm_read(usbd_cdc_buffer_in, CDCD_ECHO_BUF_SIZ);
        	while(!read_done) __WFI();
	} else {
		return RingBuf_Get(&rbuf_in);
	}
    }
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
	size_t idx = 0;
	while(idx < len){
		RingBuf_Put(&rbuf_out, str[idx]);
		idx++;
	}
	cdcdf_acm_write(0,0);
}
