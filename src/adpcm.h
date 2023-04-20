#ifndef __H_ADPCM__
#define __H_ADPCM__

#include <stdint.h>
#include <iocslib.h>

#define ADPCM_MODE_15KHZ_L  (4 * 256 + 1)
#define ADPCM_MODE_15KHZ_R  (4 * 256 + 2)
#define ADPCM_MODE_15KHZ_LR (4 * 256 + 3)

#define ADPCM_MAX_CHAINS  (16)
#define ADPCM_CHAIN_BYTES (0xff00)

#define ADPCM_SE_NUM    (5)
#define ADPCM_MUSIC_NUM (1)

typedef struct {
  int mode;
  struct CHAIN chain_tables[ ADPCM_MAX_CHAINS ];
} ADPCM_HANDLE;

void adpcm_open(ADPCM_HANDLE* adpcm);
void adpcm_close(ADPCM_HANDLE* adpcm);
void adpcm_play_se(ADPCM_HANDLE* adpcm, int16_t id);
void adpcm_play_music(ADPCM_HANDLE* adpcm, int16_t id);

#endif