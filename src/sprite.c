#include "sprite.h"
#include <iocslib.h>

#define REG_SP_SCROLL   ((volatile unsigned short*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define REG_BG_CONTROL  ((volatile unsigned short*)0xEB0808)        // bg control register (Inside X68000 p190)
#define REG_PCG_DATA    ((volatile unsigned short*)0xEB8000)        // pcg data address (Inside X68000 p174)
#define REG_SP_PALETTE  ((volatile unsigned short*)0xE82200)        // test/bg/sprite palette (Inside X68000 p214)

#define SP_DISP_OFF     (REG_BG_CONTROL[0] &= 0xfdf6)
#define SP_DISP_ON      (REG_BG_CONTROL[0] |= 0x0200)

// set sprite palette
static void sp_setup_palette(int palette_block, unsigned short* palette_data) {
  for (int i = 0; i < 16; i++) {
    REG_SP_PALETTE[ palette_block * 16 + i ] = palette_data[i];
  }
}

// set sprite patterns
void sp_setup_pattern(SPRITE_PATTERN* spp) {
  int pattern_count = spp->size_x * spp->size_y;
  for (int i = 0; i < pattern_count; i++) {
    for (int j = 0; j < 0x40; j++) {
      REG_PCG_DATA[ ( spp->pattern_id + i ) * 0x40 + j ] = spp->pattern_data[ i * 0x40 + j ];
    }
  }
  sp_setup_palette(spp->palette_block, spp->palette_data);
}

// scroll sprite
void sp_scroll(SPRITE* sp) {
  int ofs = sp->sprite_id * 4;
  int pattern = sp->spp->pattern_id;
  for (int y = 0; y < sp->spp->size_y; y++) {
    for (int x = 0; x < sp->spp->size_x; x++) {
      REG_SP_SCROLL[ofs++] = 16 + sp->pos_x + x*16;
      REG_SP_SCROLL[ofs++] = 16 + sp->pos_y + y*16;
      REG_SP_SCROLL[ofs++] = (sp->spp->palette_block << 8) | pattern++;
      REG_SP_SCROLL[ofs++] = sp->priority;
    }
  }
}
