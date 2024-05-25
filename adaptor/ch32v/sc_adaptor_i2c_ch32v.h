#ifndef __SC_ADAPTOR_I2C_CH32V_H
#define __SC_ADAPTOR_I2C_CH32V_H

#include "sc_adaptor.h"

struct sc_adaptor_i2c_ch32v {
	struct sc_adaptor_i2c_i *adaptor;
	unsigned char address;
};

int sc_adaptor_i2c_ch32v_init(struct sc_adaptor_i2c_ch32v *self, int address);

#endif
