#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

void led_hal_init (void)
{
  gpio_set_dir(SIGNAL_LED, GPIO_OUT);
}


#ifdef __cplusplus
}
#endif
