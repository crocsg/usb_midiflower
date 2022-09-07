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

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CONFIG_MAGIC        0xee113344
#define CONFIG_NBCHANNEL    4

typedef struct _config_channel {
    uint32_t                    size;         // size in notes
    uint32_t                    bpm_multi;    // bpm multiplier
    uint32_t                    noteratio;    // sequence fill ratio
    uint32_t                    vol;          // sequence relative volume
    uint16_t                    midichannel;  // real midi channel
} config_channel;

typedef struct _midiflower_config {
    uint32_t    basebpm;
    uint8_t     scale;
    uint32_t    root;
    config_channel channel[CONFIG_NBCHANNEL];

} midiflower_config;

typedef struct _config {
    uint32_t            size;
    uint32_t            magic;

    midiflower_config   midiflower;
    uint32_t            crc;
} config;

#ifdef __cplusplus
}
#endif