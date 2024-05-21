#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sc_common.h"

int delay(int milliseconds)
{
	vTaskDelay(milliseconds / portTICK_PERIOD_MS);
	return 0;
}
