#ifndef WS2812_H
#define WS2812_H

#include "pico/types.h"
#include "hardware/pio.h"

/* Similar to above, but for an 8-bit gamma-correction table.
   Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
    print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
    if x&15 == 15: print
*/
static const uint8_t _NeoPixelGammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

class WS2812 {
    public:
        enum DataByte {
            NONE=0,
            RED=1,
            GREEN=2,
            BLUE=3,
            WHITE=4
        };
        enum DataFormat {
            FORMAT_RGB=0,
            FORMAT_GRB=1,
            FORMAT_WRGB=2
        };

        WS2812(uint pin, uint length, PIO pio, uint sm);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataFormat format);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3);
        WS2812(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        ~WS2812();

        static uint32_t RGB(uint8_t red, uint8_t green, uint8_t blue) {
            return (uint32_t)(blue) << 16 | (uint32_t)(green) << 8 | (uint32_t)(red);
        };

        static uint32_t RGBW(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
            return (uint32_t)(white) << 24 | (uint32_t)(blue) << 16 | (uint32_t)(green) << 8 | (uint32_t)(red);
        }

        void setPixelColor(uint index, uint32_t color);
        void setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue);
        void setPixelColor(uint index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
        void fill(uint32_t color);
        void fill(uint32_t color, uint first);
        void fill(uint32_t color, uint first, uint count);
        void show();
        void shift_right (uint16_t amount);

        static uint8_t gamma8(uint8_t x) {
                return (_NeoPixelGammaTable[x]); // 0-255 in, 0-255 out
        }
        static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
        static uint32_t gamma32(uint32_t x);

        void rainbow(uint16_t first_hue = 0, int8_t reps = 1,
               uint8_t saturation = 255, uint8_t brightness = 255,
               bool gammify = true);
    private:
        uint pin;
        uint length;
        PIO pio;
        uint sm;
        DataByte bytes[4];
        uint32_t *data;

        void initialize(uint pin, uint length, PIO pio, uint sm, DataByte b1, DataByte b2, DataByte b3, DataByte b4);
        uint32_t convertData(uint32_t rgbw);

};

#endif