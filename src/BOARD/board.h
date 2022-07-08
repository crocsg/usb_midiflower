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

#ifndef __BOARD_H
#define __BOARD_H

#define GPIO_FLOWER_SENSOR 14
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        PWM_OUT_1,
        PWM_OUT_2,
        PWM_OUT_NBR,
    } gpio_pwm_id;

    typedef struct _gpio_pwm
    {
        gpio_pwm_id id;
        uint8_t gpio;

    } gpio_pwm;

    extern gpio_pwm board_pwm[PWM_OUT_NBR];

#define DEBUG_UART_ENABLE   1
#define DEBUG_UART  uart0
#define DEBUG_UART_BAUDRATE 115200
#define DEBUG_UART_TX   16
#define DEBUG_UART_RX   17
    
#ifdef __cplusplus
}
#endif

#define SIGNAL_LED 25

#endif // __BOARD_H
