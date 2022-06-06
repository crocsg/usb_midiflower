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

#include <stdint.h>
#include <string.h>
#include "timer_hal.h"
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

static  struct repeating_timer _timer;
static  hal_timer_callback _tcbks = NULL;
bool periodic_timer_callback(struct repeating_timer *t) {
    
    if (_tcbks)
        (*_tcbks)();
    return true;
}
void timer_hal_init ()
{
    memset (&_timer, 0, sizeof(_timer));
}
void timer_hal_add_timer_callback (uint32_t delay, hal_timer_callback tcbk)
{
     _tcbks = tcbk;
     add_repeating_timer_ms(delay, periodic_timer_callback, NULL, &_timer);
     
}

#ifdef __cplusplus
}
#endif
