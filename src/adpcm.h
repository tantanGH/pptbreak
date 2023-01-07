#ifndef __H_ADPCM__
#define __H_ADPCM__

typedef struct {
  int mode;         // 4 * 256 + 3 = 15.6kHz L+R
  int length;
  unsigned char* data;
} ADPCM_SOUND;

void adpcm_play(ADPCM_SOUND* adpcm);

#endif