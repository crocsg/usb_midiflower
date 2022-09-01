/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 S Godin (Climate Change Lab)
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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "board.h"
#include "tusb.h"
#include "usb_midiflower.h"
#include "HARD/led.h"
#include "gpio_hal.h"
#include "HARD/pwm.h"
#include "flower_activity.h"
#include "usb_lwip.h"
#include "http_server.h"
#include "crc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
void midi_task(void)
{
    static bool sysex = false;
    uint8_t msg[3];
    int n_data;

    while(n_data = tud_midi_n_available(0, 0)) {
	msg[0] = 0; msg[1] = 0; msg[2] = 0;
	if (n_data = tud_midi_n_stream_read(0, 0, msg, 3)) {

	    printf(" | ");

	    if (sysex) {
		printf("       |           : ");
		for (int i = 0; i < 3; i++) {
		    printf("%02x ", msg[i]);
		    if (msg[i] == 0xf7) {
			sysex = false;
			break;
		    }
		}
		printf("\n");
	    } else {
		sysex = (msg[0] == 0xf0);

	    }
	}
    }
}
*/
#define PIN PICO_DEFAULT_UART_RX_PIN 

int main() {

    board_init();
    tusb_init();
#if DEBUG_UART_ENABLE    
    
    stdio_uart_init_full(DEBUG_UART, 
        DEBUG_UART_BAUDRATE,
        DEBUG_UART_TX,
        DEBUG_UART_RX);
#endif
    gpio_hal_init ();
    led_init ();    
    pwm_init ();
    flower_activity_init ();
    network_init ();
    http_server_init ();
    sleep_ms(100);

    for (int i = 0; i < 100; i++)
        printf ("yahoo\r\n");

    test_crc32 ();        
    setup();
    while (true)
    {
        tud_task();
        service_traffic();
        //midi_task();
        loop ();
        
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
