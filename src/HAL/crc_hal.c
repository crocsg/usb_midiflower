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

#include "hardware/dma.h"

#include "crc_hal.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "pico/stdlib.h"


uint32_t hal_crc_crc32(void *ptr, size_t size)
{
	uint32_t dummy_dest, crc;

	int channel = dma_claim_unused_channel(true);
	dma_channel_config conf = dma_channel_get_default_config(channel);
	channel_config_set_transfer_data_size(&conf, DMA_SIZE_8 );
	channel_config_set_read_increment(&conf, true);
	channel_config_set_write_increment(&conf, false);
	channel_config_set_sniff_enable(&conf, true);
	dma_sniffer_enable(channel, 0x0, true); // mode 0 IEEE 802.3
	
	// Initialize CRC
	dma_hw->sniff_data = 0x0;

    // run dma transfert
	dma_channel_configure(channel, &conf, &dummy_dest, ptr, size, true);

	dma_channel_wait_for_finish_blocking(channel);

	// get result
	crc = dma_hw->sniff_data;

	dma_sniffer_disable();
	dma_channel_unclaim(channel);

	return crc;
}