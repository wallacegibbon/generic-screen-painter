#ifndef __SC_BYTE_ADAPTOR_CH32V_HWI2C_H
#define __SC_BYTE_ADAPTOR_CH32V_HWI2C_H

#include "sc_adaptor.h"

struct sc_byte_adaptor_ch32v_hwi2c {
	struct sc_byte_adaptor_i *adaptor;
	unsigned char address;
};

int sc_byte_adaptor_ch32v_hwi2c_init(struct sc_byte_adaptor_ch32v_hwi2c *self, int address);

#endif
