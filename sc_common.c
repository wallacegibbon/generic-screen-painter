#include "sc_common.h"

/// This function should be override. The library can not implement a delay
/// without knowing the speed of the target machine.
__attribute__((weak)) void delay(int milliseconds) {
	volatile int i;
	while (milliseconds--) {
		for (i = 0; i < 1000; i++)
			;
	}
}
