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
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define led 2

void app_main(void)
{
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    while (1)
    {
        gpio_set_level(led, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(led, 1);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
