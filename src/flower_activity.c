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
#include "flower_activity.h"
#include "HAL/timer_hal.h"
#include "HARD/pwm.h"
#include "TOOLS/utils.h"
#include "HARD/led.h"

#define FLOWER_ACTIVITY_PERIOD  20

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    STOP,
    START,
    RUNNING,

} flower_activity_state;

static flower_activity_state _state = STOP;
static uint16_t flower_activity_id = 0;

static uint16_t _activity_val[] = {
  0,
 11380,
22414,
32767,
42125,
50202,
56754,
61582,
64539,
65535,
65495,
65375,
65175,
64897,
64539,
64102,
63588,
62996,
62327,
61582,
60762,
59869,
58902,
57863,
56754,
55576,
54330,
53018,
51642,
50202,
48701,
47141,
45524,
43851,
42125,
40347,
38520,
36646,
34728,
32767,
30766,
28728,
26655,
24549,
22414,
20251,
18063,
15854,
13625,
11380,
9120,
6850,
4571,
2287,

  0,
  0,
  0,

};
static void flower_timer_callback (void)
{
    switch (_state)
    {
        case STOP:
            break;
        case START:
            flower_activity_id = 0;
            _state = RUNNING;
            break;
        case RUNNING:
            pwm_activity_level (_activity_val[flower_activity_id]);
            flower_activity_id++;
            if (flower_activity_id >= ARRAYLEN (_activity_val))
            {
                _state = STOP;
                flower_activity_id = 0;
            }
            break;    
        default:
        _state = STOP;
    }
    
}

void flower_activity_init ()
{
    timer_hal_init ();
    timer_hal_add_timer_callback (FLOWER_ACTIVITY_PERIOD, flower_timer_callback);
}

void flower_activity_signal_event ()
{
    led_signal_led_toggle ();
    if (_state == STOP)
        _state = START;
}

#ifdef __cplusplus
}
#endif
