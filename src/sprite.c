#include <iocslib.h>

#include "sprite.h"

#define SP_SCROLL_REG   ((volatile unsigned short*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define BG_CONTROL_REG  ((volatile unsigned short*)0xEB0808)        // bg control register (Inside X68000 p190)
#define PCG_DATA_REG    ((volatile unsigned short*)0xEB8000)        // pcg data address (Inside X68000 p174)
#define SP_PALETTE_REG  ((volatile unsigned short*)0xE82200)        // test/bg/sprite palette (Inside X68000 p214)

#define SP_DISP_OFF     (BG_CONTROL_REG[0] &= 0xfdf6)
#define SP_DISP_ON      (BG_CONTROL_REG[0] |= 0x0200)

// set sprite palette
static void sp_setup_palette(int palette_block, unsigned short* palette_data) {
  for (int i = 0; i < 16; i++) {
    SP_PALETTE_REG[ palette_block * 16 + i ] = palette_data[i];
  }
}

// set sprite patterns
void sp_setup_pattern(SPRITE_PATTERN* spp) {
  int pattern_count = spp->size_x * spp->size_y;
  for (int i = 0; i < pattern_count; i++) {
    for (int j = 0; j < 0x40; j++) {
      PCG_DATA_REG[ ( spp->pattern_id + i ) * 0x40 + j ] = spp->pattern_data[ i * 0x40 + j ];
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
      SP_SCROLL_REG[ofs++] = 16 + sp->pos_x + x*16;
      SP_SCROLL_REG[ofs++] = 16 + sp->pos_y + y*16;
      SP_SCROLL_REG[ofs++] = (sp->spp->palette_block << 8) | pattern++;
      SP_SCROLL_REG[ofs++] = sp->priority;
    }
  }
}
