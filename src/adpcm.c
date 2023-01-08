#include "adpcm.h"
#include <iocslib.h>

void adpcm_play(ADPCM_SOUND* adpcm) {
  ADPCMOUT(adpcm->data, adpcm->mode, adpcm->length);
}