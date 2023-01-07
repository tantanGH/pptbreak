#ifndef __H_ADPCM__
#define __H_ADPCM__

#define ADPCM_MODE_15KHZ_L  (4 * 256 + 1)
#define ADPCM_MODE_15KHZ_R  (4 * 256 + 2)
#define ADPCM_MODE_15KHZ_LR (4 * 256 + 3)

typedef struct {
  int mode;
  int length;
  unsigned char* data;
} ADPCM_SOUND;

void adpcm_play(ADPCM_SOUND* adpcm);

#endif