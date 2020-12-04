#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#ifdef CONFIG_IDF_TARGET_ESP32
#include "driver/sdmmc_host.h"
#endif

#include "esp_err.h"
#include "esp_log.h"

#define MOUNT_POINT "/sdcard"

#define USE_SPI_MODE

// DMA channel to be used by the SPI peripheral
#ifndef SPI_DMA_CHAN
#define SPI_DMA_CHAN 1
#endif //SPI_DMA_CHAN

#define SD_MISO 19
#define SD_MOSI 23
#define SD_CLK 18
#define SD_CS 5

void sdcard_init();
void sdcard_release();
