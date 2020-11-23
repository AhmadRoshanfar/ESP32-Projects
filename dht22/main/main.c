/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
// #include "esp_spi_flash.h"
#include <dht.h>

static const gpio_num_t dht_pin = GPIO_NUM_23;

void dhtTask(void *prvParameter)
{
    int16_t Humidity = 0;
    int16_t Temperature = 0;

    if (dht_read_data(DHT_TYPE_AM2301, dht_pin, &Humidity, &Temperature) == ESP_OK)
    {
        printf("Humidity: %d%% \t | \t Temperature:%d^C \r\n", Humidity / 10, Temperature / 10);
    }
    else
    {
        printf("\r\n !!! Reading ERROR !!! \r\n");
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}
void app_main(void)
{
    printf("\r\n *** DHT22 Monitoring *** \r\n");
    xTaskCreate(&dhtTask, "dhtTask", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
