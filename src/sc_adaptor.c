#include "sc_adaptor.h"
#include <stdarg.h>

int sc_adaptor_write_cmd_and_args(struct sc_adaptor_i **self, int cmd, int arg_cnt, ...)
{
	va_list args;
	int i;

	va_start(args, arg_cnt);

	if (sc_adaptor_write_cmd(self, cmd))
		return 1;
	for (i = 0; i < arg_cnt; i++) {
		if (sc_adaptor_write_data(self, va_arg(args, int)))
			return 2;
	}

	va_end(args);
	return 0;
}
