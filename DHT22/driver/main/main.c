#include "DHT22.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"

void app_main() {

    while (1) {
     	   printf("Starting signal...\n");
		DHT22_ReadData();

		printf("Humidity: %f\n", getHumidity());
		printf("Temperature: %f\n", getTemperature());

     	   vTaskDelay(3000 / portTICK_PERIOD_MS);  // Read every 2 seconds
    }
}
