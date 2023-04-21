#include <stdint.h>
#include <string.h>
#include <iocslib.h>
#include "data_se.h"
#include "data_music.h"
#include "adpcm.h"

void adpcm_open(ADPCM_HANDLE* adpcm) {
  adpcm->mode = ADPCM_MODE_15KHZ_LR;
  memset(adpcm->chain_tables, 0, sizeof(struct CHAIN) * ADPCM_MAX_CHAINS);
  adpcm_stop(adpcm);
}

void adpcm_close(ADPCM_HANDLE* adpcm) {
  adpcm_stop(adpcm);
}

void adpcm_stop(ADPCM_HANDLE* adpcm) {
  ADPCMMOD(0);
}

void adpcm_play_se(ADPCM_HANDLE* adpcm, int16_t id) {
  uint8_t* buf = NULL;
  size_t len = 0;
  switch (id) {
    case 1: buf = adpcm_se01; len = ADPCM_SE01_LENGTH; break;
    case 2: buf = adpcm_se02; len = ADPCM_SE02_LENGTH; break;
    case 3: buf = adpcm_se03; len = ADPCM_SE03_LENGTH; break;
    case 4: buf = adpcm_se04; len = ADPCM_SE04_LENGTH; break;
    case 5: buf = adpcm_se05; len = ADPCM_SE05_LENGTH; break;
  }
  if (buf != NULL) {
    if (ADPCMSNS() != 0) ADPCMMOD(0);
    ADPCMOUT(buf, adpcm->mode, len);
  }
}

void adpcm_play_music(ADPCM_HANDLE* adpcm, int16_t id) {
  uint8_t* buf = NULL;
  size_t len = 0;
  switch (id) {
    case 1: buf = adpcm_music01; len = ADPCM_MUSIC01_LENGTH; break;
  }
  if (buf != NULL) {
    if (ADPCMSNS() != 0) ADPCMMOD(0);
    int16_t chain_count = 1 + (int16_t)( len / ADPCM_CHAIN_BYTES );
    for (int16_t i = 0; i < chain_count; i++) {
      size_t chain_len = len > ADPCM_CHAIN_BYTES ? ADPCM_CHAIN_BYTES : len;
      adpcm->chain_tables[i].adr = (uint32_t)buf;
      adpcm->chain_tables[i].len = chain_len;
      buf += chain_len;
      len -= chain_len;
    }
    ADPCMAOT(adpcm->chain_tables, adpcm->mode, chain_count);
  }
}