#include "common.h"

/// This function should be override. The library can not implement a delay
/// without knowing the speed of the target machine.
__attribute__((weak)) void delay(int milliseconds) {
	static int counter = 0;
	int i = 0, j;

	while (i++ < milliseconds)
		for (j = 0; j < 10000; j++);

	counter++;
}
