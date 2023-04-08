#ifndef __SC_ST7789_CH32V_FSMC_H
#define __SC_ST7789_CH32V_FSMC_H

#include "sc_st7789.h"

struct ST7789_ScreenAdaptorCH32VFSMC {
	const struct ST7789_ScreenAdaptorInterface *adaptor;
};

void ST7789_ScreenAdaptorCH32VFSMC_initialize(
	struct ST7789_ScreenAdaptorCH32VFSMC *self
);

#endif

