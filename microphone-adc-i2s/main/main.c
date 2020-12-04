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
#include "wave.h"
#include "audio_example_file.h"

static const char *TAG = "Main";
int8_t header[HEADER_SIZE];

void write_to_SDCard()
{
    ESP_LOGI(TAG, "Opening file");
    FILE *wave_file;
    wave_file = fopen(MOUNT_POINT "/samples.wav", "w");
    if (wave_file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    // fprintf(wave_file, "Hello Ahmad!\n");
    // fwrite(&header, 44, sizeof(int8_t), wave_file);
    fwrite(&header, 44, sizeof(int8_t), wave_file);

    fclose(wave_file);
    ESP_LOGI(TAG, "File written");

    wave_file = fopen(MOUNT_POINT "/samples.wav", "a");
    if (wave_file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    // fprintf(wave_file, "Hello Ahmad!\n");
    // fwrite(&header, 44, sizeof(int8_t), wave_file);
    fwrite(&audio_table, sizeof(audio_table), sizeof(int8_t), wave_file);

    fclose(wave_file);
}

void app_main(void)
{
    /* Initilization */
    sdcard_init();
    Microphone_init();
    wavHeader(header, FLASH_RECORD_SIZE);
    write_to_SDCard();

    /* Free Buffers */
    sdcard_release();
}
