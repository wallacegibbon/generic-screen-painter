#ifndef __SC_ADAPTOR_CH32V_HWSPI_H
#define __SC_ADAPTOR_CH32V_HWSPI_H

#include "sc_adaptor.h"

struct sc_adaptor_ch32v_hwspi {
	struct sc_adaptor_i *adaptor;
};

int sc_adaptor_ch32v_hwspi_init(struct sc_adaptor_ch32v_hwspi *self);

#endif
