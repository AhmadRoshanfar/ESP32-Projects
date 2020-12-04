#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"

#include "sd_card.h"
#include "microphone.h"

static const char *TAG = "Main";

void app_main(void)
{
    sdcard_init();
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen(MOUNT_POINT "/samples.txt", "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello Ahmad!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

    sdcard_release();
}
