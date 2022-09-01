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
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "board.h"
#include "gpio_pwm_hal.h"
#include "TOOLS/utils.h"

#ifdef __cplusplus
extern "C" {
#endif

gpio_pwm board_pwm[PWM_OUT_NBR] = {
    { PWM_OUT_1, 2},
    
};



void gpio_pwm_hal_init (void)
{
    for (uint8_t i = 0; i < ARRAYLEN(board_pwm); i++)
    {
        gpio_set_function(board_pwm[i].gpio, GPIO_FUNC_PWM);
      
        // Set period of 65536 cycles (0 to 65535 inclusive)
        pwm_set_wrap(pwm_gpio_to_slice_num(board_pwm[i].gpio), 1024);
	  
	    // define global level to 0
        pwm_set_chan_level(pwm_gpio_to_slice_num(board_pwm[i].gpio), pwm_gpio_to_channel(board_pwm[i].gpio) , 0);
      
	    // enable pwm
	    pwm_set_enabled(pwm_gpio_to_slice_num(board_pwm[i].gpio), true);
    }
}

void gpio_pwm_hal_set_pwm (uint8_t gpio_pwm_id, uint16_t pwmval)
{
    pwm_set_gpio_level (board_pwm[gpio_pwm_id].gpio, pwmval >> 6);
}

#ifdef __cplusplus
}
#endif
