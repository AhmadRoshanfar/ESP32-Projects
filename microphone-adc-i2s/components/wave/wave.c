#include <stdio.h>
#include "wave.h"

static const char *TAG = "Wave";

void wavHeader(int8_t *header, int wavSize)
{
    /* ChunkID */
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    /* ChunkSize */
    unsigned int fileSize = wavSize + HEADER_SIZE - 8;
    header[4] = (int8_t)(fileSize & 0xFF);
    header[5] = (int8_t)((fileSize >> 8) & 0xFF);
    header[6] = (int8_t)((fileSize >> 16) & 0xFF);
    header[7] = (int8_t)((fileSize >> 24) & 0xFF);
    /* Format */
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    /* Subchunk1ID */
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    /* Subchunk1Size: 16 for PCM */
    header[16] = 0x10;
    header[17] = 0x00;
    header[18] = 0x00;
    header[19] = 0x00;
    /* AudioFormat: 1 for PCM */
    header[20] = 0x01;
    header[21] = 0x00;
    /* NumChannels: 1 for Mono */
    header[22] = 0x01;
    header[23] = 0x00;
    /* SampleRate: 16000 = 0x3E80 */
    header[24] = 0x80;
    header[25] = 0x3E;
    header[26] = 0x00;
    header[27] = 0x00;
    /* ByteRate  = (sampleRate * bitPersample * channels) / 8  = 32000 = 0x7D00  */
    header[28] = 0x00;
    header[29] = 0x7D;
    header[30] = 0x00;
    header[31] = 0x00;
    /* BlockAlign */
    header[32] = 0x02;
    header[33] = 0x00;
    /* BitsPerSample = 16*/
    header[34] = 0x10;
    header[35] = 0x00;
    /* Subchunk2ID: 'data' */
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    /* Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8 */
    header[40] = (int8_t)(wavSize & 0xFF);
    header[41] = (int8_t)((wavSize >> 8) & 0xFF);
    header[42] = (int8_t)((wavSize >> 16) & 0xFF);
    header[43] = (int8_t)((wavSize >> 24) & 0xFF);
    /* Data */
}

/* 
typedef struct
{
    //   RIFF Section
    char riff_section_id[4]; // Letters "RIFF"
    uint32_t size;           // Size of entire file less 8
    char riff_format[4];     // Letters "WAVE"

    //   Format Section
    char format_section_id[4]; // letters "fmt"
    uint32_t format_size;      // Size of format section less 8
    uint16_t format_id;        // 1=uncompressed PCM
    uint16_t num_channels;     // 1=mono,2=stereo
    uint32_t sample_rate;      // 44100, 16000, 8000 etc.
    uint16_t bits_per_sample;  // 8,16,24 or 32
    uint32_t byte_rate;        // =sample_rate * Channels * (bits_per_sample/8)
    uint16_t block_align;      // =Channels * (bits_per_sample/8)

    // Data Section
    char data_section_id[4]; // The letters "data"
    uint32_t data_size;      // Size of the data that follows
} wave_header_t;

wave_header_t makeWaveHeader(int const sample_rate, short int const num_channels, short int const bits_per_sample)
{
    wave_header_t wave_header;

    // RIFF WAVE Header
    wave_header.riff_section_id[0] = 'R';
    wave_header.riff_section_id[1] = 'I';
    wave_header.riff_section_id[2] = 'F';
    wave_header.riff_section_id[3] = 'F';

    wave_header.riff_format[0] = 'W';
    wave_header.riff_format[1] = 'A';
    wave_header.riff_format[2] = 'V';
    wave_header.riff_format[3] = 'E';

    // Format subchunk
    wave_header.format_section_id[0] = 'f';
    wave_header.format_section_id[1] = 'm';
    wave_header.format_section_id[2] = 't';
    wave_header.format_section_id[3] = ' ';

    wave_header.format_id = 1; // FOR PCM

    wave_header.num_channels = num_channels;       // 1 for MONO, 2 for stereo
    wave_header.sample_rate = sample_rate;         // ie 44100 hertz, cd quality audio
    wave_header.bits_per_sample = bits_per_sample; //
    wave_header.byte_rate = wave_header.sample_rate * wave_header.num_channels * wave_header.bits_per_sample / 8;
    wave_header.block_align = wave_header.num_channels * wave_header.bits_per_sample / 8;

    // Data subchunk
    wave_header.data_section_id[0] = 'd';
    wave_header.data_section_id[1] = 'a';
    wave_header.data_section_id[2] = 't';
    wave_header.data_section_id[3] = 'a';

    // All sizes for later:
    // chuckSize = 4 + (8 + subChunk1Size) + (8 + subChubk2Size)
    // subChunk1Size is constanst, i'm using 16 and staying with PCM
    // subChunk2Size = nSamples * nChannels * bits_per_sample/8
    // Whenever a sample is added:
    //    chunkSize += (nChannels * bits_per_sample/8)
    //    subChunk2Size += (nChannels * bits_per_sample/8)
    wave_header.size = 4 + 8 + 16 + 8 + 0;
    wave_header.format_size = 16;
    wave_header.data_size = 0;

    return wave_header;
}

typedef struct
{
    wave_header_t header;
    char *data;
    long long int index;
    long long int size;
    long long int nSamples;
} wave_t;

wave_t makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample)
{
    wave_t myWave;
    myWave.header = makeWaveHeader(sampleRate, numChannels, bitsPerSample);
    return myWave;
}

void waveSetDuration(wave_t *wave, const float seconds)
{
    long long int totalBytes = (long long int)(wave->header.byte_rate * seconds);
    wave->data = (char *)malloc(totalBytes);
    wave->index = 0;
    wave->size = totalBytes;
    wave->nSamples = (long long int)wave->header.num_channels * wave->header.sample_rate * seconds;
    wave->header.chunk_size = 4 + 8 + 16 + 8 + totalBytes;
    wave->header.subChunk2Size = totalBytes;
}

void waveDestroy(wave_t *wave) */
/* {
    free(wave->data);
} */