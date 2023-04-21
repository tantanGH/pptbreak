#include <stdio.h>
#include <stdint.h>
#include "adpcm.h"

void main(int32_t argc, uint8_t* argv[]) {

  ADPCM_HANDLE adpcm = { 0 };
  adpcm_open(&adpcm);

/*
  adpcm_play_se(&adpcm, 2);
  for (uint32_t t0 = ONTIME(); ONTIME() < t0 + 10; ) {}
  adpcm_play_se(&adpcm, 3);
  for (uint32_t t0 = ONTIME(); ONTIME() < t0 + 10; ) {}
  adpcm_play_se(&adpcm, 4);
*/

  adpcm_play_music(&adpcm, 1);

  for (;;) {
    if (B_SFTSNS() & 0x01) break;
    if (ADPCMSNS() == 0) break;
    for (uint32_t t0 = ONTIME(); ONTIME() < t0 + 100; ) {}
    printf(".");
  }
  printf("\n");

  adpcm_close(&adpcm);
}