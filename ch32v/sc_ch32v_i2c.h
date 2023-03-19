#ifndef __SC_CH32V_I2C_H
#define __SC_CH32V_I2C_H

#ifdef CHIP_CH32V10X
#include "ch32v10x_i2c.h"
#elif CHIP_CH32V30X
#include "ch32v30x_i2c.h"
#else
#include "ch32v20x_i2c.h"
#endif

#endif

