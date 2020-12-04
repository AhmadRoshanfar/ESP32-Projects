#include <stdio.h>
#include "microphone.h"

#include "driver/adc.h"
#include "driver/i2s.h"

static const char *TAG = "Microphone";

void Microphone_init(void)
{
    /* I2S Configuration */
    const i2s_config_t i2s_adc_cfg = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
        /* Sampling 16K and 16Bit Per sample */
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_SAMPLE_BITS,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .intr_alloc_flags = 0,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
        .use_apll = 1,
    };
    i2s_driver_install(I2S_NUM, &i2s_adc_cfg, 0, NULL);
    i2s_set_adc_mode(I2S_ADC_UINT, I2S_ADC_CHANNEL);
}

esp_err_t Microphone_read()
{
    int i2s_read_len = I2S_READ_LEN;
    int flash_wr_size = 0;

    char *i2s_read_buf = (char *)calloc(i2s_read_len, sizeof(char));
    size_t bytes_read;

    i2s_adc_enable(I2S_NUM);
    while (flash_wr_size < FLASH_RECORD_SIZE)
    {
        i2s_read(I2S_NUM, (void *)i2s_read_buf, i2s_read_len, &bytes_read, portMAX_DELAY);
        flash_wr_size += i2s_read_len;
    }
    i2s_adc_disable(I2S_NUM);

    // TODO: FREE Buffers
    // TODO: Write "i2s_read_buf" TO SD CARDS
    // free(i2s_read_buf);
    // i2s_read_buf = NULL;
    return ESP_OK;
}
