#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sc_common.h"

void delay(int milliseconds)
{
	vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}
