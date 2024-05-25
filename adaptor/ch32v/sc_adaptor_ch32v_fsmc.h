#ifndef __SC_ADAPTOR_CH32V_FSMC_H
#define __SC_ADAPTOR_CH32V_FSMC_H

#include "sc_adaptor.h"

/// A17 is controlling the DATA/CMD signal (the `DCX` Pin). 0 -> cmd, 1 -> data
#define ST7789_LCD_CMD ((uintptr_t)0x60000000)
#define ST7789_LCD_DATA ((uintptr_t)0x60020000)

struct sc_adaptor_ch32v_fsmc {
	struct sc_adaptor_i *adaptor;
};

int sc_adaptor_ch32v_fsmc_init(struct sc_adaptor_ch32v_fsmc *self);

#endif
