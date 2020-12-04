#include "driver/i2s.h"

#define I2S_NUM I2S_NUM_0
#define I2S_ADC_UINT ADC_UNIT_1
#define I2S_ADC_CHANNEL ADC_CHANNEL_7

#define I2S_SAMPLE_RATE (16000)
#define I2S_SAMPLE_BITS (16)
#define I2S_FORMAT (I2S_CHANNEL_FMT_RIGHT_LEFT)
#define I2S_CHANNEL_NUM ((I2S_FORMAT < I2S_CHANNEL_FMT_ONLY_RIGHT) ? (2) : (1))
/* Record 5 Seconds */
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * 5)
#define I2S_READ_LEN (16 * 1024)

void Microphone_init();
