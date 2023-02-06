#include "WS2812.hpp"
#include "WS2812.pio.h"

//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm)  {
    initialize(pin, length, pio, sm, NONE, GREEN, RED, BLUE);
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataFormat format) {
    switch (format) {
        case FORMAT_RGB:
            initialize(pin, length, pio, sm, NONE, RED, GREEN, BLUE);
            break;
        case FORMAT_GRB:
            initialize(pin, length, pio, sm, NONE, GREEN, RED, BLUE);
            break;
        case FORMAT_WRGB:
            initialize(pin, length, pio, sm, WHITE, RED, GREEN, BLUE);
            break;
    }
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3) {
    initialize(pin, length, pio, sm, b1, b1, b2, b3);
}

WS2812::WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4) {
    initialize(pin, length, pio, sm, b1, b2, b3, b4);
}

WS2812::~WS2812() {
    
}

void WS2812::initialize(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4) {
    this->pin = pin;
    this->length = length;
    this->pio = pio;
    this->sm = sm;
    this->data = new uint32_t[length];
    this->bytes[0] = b1;
    this->bytes[1] = b2;
    this->bytes[2] = b3;
    this->bytes[3] = b4;
    uint offset = pio_add_program(pio, &ws2812_program);
    uint bits = (b1 == NONE ? 24 : 32);
    #ifdef DEBUG
    printf("WS2812 / Initializing SM %u with offset %X at pin %u and %u data bits...\n", sm, offset, pin, bits);
    #endif
    ws2812_program_init(pio, sm, offset, pin, 800000, bits);
}

uint32_t WS2812::convertData(uint32_t rgbw) {
    uint32_t result = 0;
    for (uint b = 0; b < 4; b++) {
        switch (bytes[b]) {
            case RED:
                result |= (rgbw & 0xFF);
                break;
            case GREEN:
                result |= (rgbw & 0xFF00) >> 8;
                break;
            case BLUE:
                result |= (rgbw & 0xFF0000) >> 16;
                break;
            case WHITE:
                result |= (rgbw & 0xFF000000) >> 24;
                break;
            case NONE:
                break;
        }
        result <<= 8;
    }
    return result;
}

void WS2812::setPixelColor(uint index, uint32_t color) {
    if (index < length) {
        data[index] = convertData(color);
    }
}

void WS2812::setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue) {
    setPixelColor(index, RGB(red, green, blue));
}

void WS2812::setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    setPixelColor(index, RGBW(red, green, blue, white));
}

void WS2812::fill(uint32_t color) {
    fill(color, 0, length);
}

void WS2812::fill(uint32_t color, uint first) {
    fill(color, first, length-first);
}

void WS2812::fill(uint32_t color, uint first, uint count) {
    uint last = (first + count);
    if (last > length) {
        last = length;
    }
    color = convertData(color);
    for (uint i = first; i < last; i++) {
        data[i] = color;
    }
}

void WS2812::show() {
    #ifdef DEBUG
    for (uint i = 0; i < length; i++) {
        printf("WS2812 / Put data: %08X\n", data[i]);
    }
    #endif
    for (uint i = 0; i < length; i++) {
        pio_sm_put_blocking(pio, sm, data[i]);
    }
}

uint32_t WS2812::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {

  uint8_t r, g, b;

  // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
  // 0 is not the start of pure red, but the midpoint...a few values above
  // zero and a few below 65536 all yield pure red (similarly, 32768 is the
  // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
  // each for red, green, blue) really only allows for 1530 distinct hues
  // (not 1536, more on that below), but the full unsigned 16-bit type was
  // chosen for hue so that one's code can easily handle a contiguous color
  // wheel by allowing hue to roll over in either direction.
  hue = (hue * 1530L + 32768) / 65536;
  // Because red is centered on the rollover point (the +32768 above,
  // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
  // where 0 and 1530 would yield the same thing. Rather than apply a
  // costly modulo operator, 1530 is handled as a special case below.

  // So you'd think that the color "hexcone" (the thing that ramps from
  // pure red, to pure yellow, to pure green and so forth back to red,
  // yielding six slices), and with each color component having 256
  // possible values (0-255), might have 1536 possible items (6*256),
  // but in reality there's 1530. This is because the last element in
  // each 256-element slice is equal to the first element of the next
  // slice, and keeping those in there this would create small
  // discontinuities in the color wheel. So the last element of each
  // slice is dropped...we regard only elements 0-254, with item 255
  // being picked up as element 0 of the next slice. Like this:
  // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
  // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
  // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
  // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
  // the constants below are not the multiples of 256 you might expect.

  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if (hue < 510) { // Red to Green-1
    b = 0;
    if (hue < 255) { //   Red to Yellow-1
      r = 255;
      g = hue;       //     g = 0 to 254
    } else {         //   Yellow to Green-1
      r = 510 - hue; //     r = 255 to 1
      g = 255;
    }
  } else if (hue < 1020) { // Green to Blue-1
    r = 0;
    if (hue < 765) { //   Green to Cyan-1
      g = 255;
      b = hue - 510;  //     b = 0 to 254
    } else {          //   Cyan to Blue-1
      g = 1020 - hue; //     g = 255 to 1
      b = 255;
    }
  } else if (hue < 1530) { // Blue to Red-1
    g = 0;
    if (hue < 1275) { //   Blue to Magenta-1
      r = hue - 1020; //     r = 0 to 254
      b = 255;
    } else { //   Magenta to Red-1
      r = 255;
      b = 1530 - hue; //     b = 255 to 1
    }
  } else { // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }

  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
  uint16_t s1 = 1 + sat;  // 1 to 256; same reason
  uint8_t s2 = 255 - sat; // 255 to 0
  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
         (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
         (((((b * s1) >> 8) + s2) * v1) >> 8);
}

uint32_t WS2812::gamma32(uint32_t x) {
  uint8_t *y = (uint8_t *)&x;
  // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
  // to avoid a bunch of shifting and masking that would be necessary for
  // properly handling different endianisms (and each byte is a fairly
  // trivial operation, so it might not even be wasting cycles vs a check
  // and branch for the RGB case). In theory this might cause trouble *if*
  // someone's storing information in the unused most significant byte
  // of an RGB value, but this seems exceedingly rare and if it's
  // encountered in reality they can mask values going in or coming out.
  for (uint8_t i = 0; i < 4; i++)
    y[i] = gamma8(y[i]);
  return x; // Packed 32-bit return
}

void WS2812::rainbow(uint16_t first_hue, int8_t reps,
  uint8_t saturation, uint8_t brightness, bool gammify) {
  for (uint16_t i=0; i < length; i++) {
    uint16_t hue = first_hue + (i * reps * 65536) / length;
    uint32_t color = ColorHSV(hue, saturation, brightness);
    if (gammify) color = gamma32(color);
    setPixelColor(i, color);
  }
}

void WS2812::shift_right (uint16_t amount)
{
  if (length > amount)
  {
    for (int16_t i = length - 1, src = length - 1 - amount; i >= 0; i--,src--)
      data[i] = data[src];
  }
}