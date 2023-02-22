#include "common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay(int milliseconds) {
	vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}
