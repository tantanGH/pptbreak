#include <iocslib.h>
#include "adpcm.h"

void adpcm_play(ADPCM_SOUND* adpcm) {
  ADPCMOUT(adpcm->data, adpcm->mode, adpcm->length);
}