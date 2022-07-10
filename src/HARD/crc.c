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
#include <stdio.h>
#include "pico/stdlib.h"


#include "crc_hal.h"
#include "crc.h"

#define CRC_DEBUG   1

#ifdef CRC_DEBUG
#define DEBUG_PRINTF(x) do {printf x;} while(0)
#else
#define DEBUG_PRINTF(x)
#endif

uint32_t crc_crc32(void * ptr, size_t size)
{
    return (hal_crc_crc32(ptr, size));
}

void test_crc32 (void)
{
    uint8_t data[64];
    uint32_t crc;

    memset (data, 0, sizeof(data));
    data[0] = 1;
    crc = crc_crc32 (data, 1);
    DEBUG_PRINTF(("crc32 size 1 : OK %lu\r\n", crc));
    crc = crc_crc32 (data, 2);
    DEBUG_PRINTF(("crc32 size 2 : OK %lu \r\n", crc));
    crc = crc_crc32 (data, 3);
    DEBUG_PRINTF(("crc32 size 3 : OK %lu\r\n", crc));
    crc = crc_crc32 (data, sizeof(data));
    DEBUG_PRINTF(("crc32 size 64 : OK %lu\r\n", crc));

    for (int i = 0; i < sizeof(data); i++)
        data[i] = i;
    crc = crc_crc32 (data, sizeof(data));
    data[31] += 1;
    if (crc != crc_crc32(data, sizeof(data)))
    {
        DEBUG_PRINTF(("crc32 check 64 : OK %lu\r\n", crc));
    }
    else
    {
        DEBUG_PRINTF(("crc32 check 64 : FAILED %lu\r\n", crc));
    }
}