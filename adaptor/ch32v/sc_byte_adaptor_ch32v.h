#ifndef __SC_BYTE_ADAPTOR_CH32V_H
#define __SC_BYTE_ADAPTOR_CH32V_H

#include "sc_adaptor.h"

struct sc_byte_adaptor_ch32v {
	struct sc_byte_adaptor_i *adaptor;
	unsigned char address;
};

int sc_byte_adaptor_ch32v_init(struct sc_byte_adaptor_ch32v *self, int address);

#endif
