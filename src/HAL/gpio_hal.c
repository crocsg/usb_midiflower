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
#include <stddef.h>
#include <string.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "../TOOLS/utils.h"
#include "gpio_hal.h"

#include "board.h"

#define MAX_GPIO_IRQ_HANDLER  4

typedef struct {
  uint16_t                  gpio;
  gpio_hal_irq_callback_t   callback;
} gpio_hal_pin_handler;

gpio_hal_pin_handler  _handler[MAX_GPIO_IRQ_HANDLER];

void _gpio_hal_interrupt_handler (unsigned int gpio, long unsigned int events)
{
  //led_hal_signal_led_toggle ();
  for (uint8_t i = 0; i < MAX_GPIO_IRQ_HANDLER; i++)
  {
    if (/*gpio == _handler[i].gpio && */_handler[i].callback != NULL)
      _handler[i].callback (gpio);
  }
}

void gpio_hal_init (void)
{
    memset (_handler, 0, sizeof(_handler));

    gpio_set_input_enabled (GPIO_FLOWER_SENSOR, true);

}

void gpio_hal_enable_interrupt (uint16_t gpiopin, gpio_hal_irq_callback_t callback)
{
      for (uint8_t i= 0; i < ARRAYLEN(_handler); i++)
      {
        if (_handler[i].gpio == 0 || _handler[i].gpio == gpiopin)
        {
          _handler[i].gpio = gpiopin;
          _handler[i].callback = callback;
          if (i == 0)
          {
              gpio_set_irq_enabled_with_callback(gpiopin, GPIO_IRQ_EDGE_RISE, true, _gpio_hal_interrupt_handler);
          }
          gpio_set_irq_enabled (gpiopin, GPIO_IRQ_EDGE_RISE, true)  ;
          break;
        }
      }
}

void gpio_hal_disable_interrupt (uint16_t gpiopin)
{
  for (uint8_t i= 0; i < ARRAYLEN(_handler); i++)
  {
    if (_handler[i].gpio == gpiopin)
    {
      gpio_set_irq_enabled (gpiopin, GPIO_IRQ_EDGE_RISE, false);
      _handler[i].gpio = 0;
      _handler[i].callback = NULL;

      break;
    }
  }

}
