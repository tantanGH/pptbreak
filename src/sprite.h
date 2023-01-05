#ifndef __H_SPRITE__
#define __H_SPRITE__

#include "crtc.h"

#define SP_SCROLL_REG   ((volatile unsigned short*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define BG_CONTROL_REG  ((volatile unsigned short*)0xEB0808)        // bg control register (Inside X68000 p190)
#define PCG_DATA_REG    ((volatile unsigned short*)0xEB8000)        // pcg data address (Inside X68000 p174)
#define SP_PALETTE_REG  ((volatile unsigned short*)0xE82200)        // test/bg/sprite palette (Inside X68000 p214)

#define SP_DISP_OFF     (BG_CONTROL_REG[0] &= 0xfdf6)
#define SP_DISP_ON      (BG_CONTROL_REG[0] |= 0x0200)

// sprite pattern class
typedef struct {
  int pattern_id;
  int size_x;
  int size_y;
  int palette_block;
  unsigned short* pattern_data;
  unsigned short* palette_data;
} SPRITE_PATTERN;

// sprite class
typedef struct {
  int sprite_id;
  int pos_x;
  int pos_y;
  int priority;
  SPRITE_PATTERN* spp;
} SPRITE;

// prototype declarations
void sp_setup_pattern(SPRITE_PATTERN* spp);
void sp_scroll(SPRITE* sp);

#endif