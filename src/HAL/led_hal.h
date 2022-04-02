#ifndef __LED_HAL_H
#define __LED_HAL_H

#include "pico/stdlib.h"
#include "bsp/board.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif


void led_hal_init (void);
__attribute__((always_inline)) extern inline void led_hal_signal_led_on (void)
{
  gpio_put(SIGNAL_LED,1);
}

__attribute__((always_inline)) extern inline void led_hal_signal_led_off (void)
{
  gpio_put(SIGNAL_LED,0);
}

__attribute__((always_inline)) extern inline void led_hal_signal_led_toggle (void)
{
  gpio_put(SIGNAL_LED, gpio_get_out_level (SIGNAL_LED) == 0 ? 1 : 0);
}


#ifdef __cplusplus
}
#endif
#endif // __LED_HAL_H
