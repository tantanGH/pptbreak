#ifndef __H_SPRITE__
#define __H_SPRITE__

#include <stdint.h>
#include "screen.h"

#define REG_SP_SCROLL   ((volatile uint16_t*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define REG_BG_CONTROL  ((volatile uint16_t*)0xEB0808)        // bg control register (Inside X68000 p190)
#define REG_PCG_DATA    ((volatile uint16_t*)0xEB8000)        // pcg data address (Inside X68000 p174)
#define REG_SP_PALETTE  ((volatile uint16_t*)0xE82200)        // test/bg/sprite palette (Inside X68000 p214)

#define SP_DISP_OFF     (REG_BG_CONTROL[0] &= 0xfdf6)
#define SP_DISP_ON      (REG_BG_CONTROL[0] |= 0x0200)

#define NUM_OF_BLOCKS (40)
#define NUM_OF_STARS  (24)

#define SPID_BAR   (0)
#define SPID_BALL  (1)
#define SPID_BLOCK (2)
#define SPID_STAR  (3)

// sprite pattern
typedef struct {
  int16_t pattern_id;
  int16_t size_x;
  int16_t size_y;
  int16_t palette_block;
  uint16_t* pattern_data;
  uint16_t* palette_data;
} SPRITE_PATTERN;

// sprite instance
typedef struct {
  int16_t sprite_id;
  int32_t pos_x;
  int32_t pos_y;
  int32_t pos_z;      // optional
  int32_t pos_x2;     // optional
  int32_t pos_y2;     // optional
  int32_t pos_z2;     // optional
  int16_t priority;
  volatile int16_t invalidate;    // for repaint
  SPRITE_PATTERN* spp;
} SPRITE;

// sprite manager handle
typedef struct {

  SPRITE_PATTERN spp_bar;
  SPRITE_PATTERN spp_ball;
  SPRITE_PATTERN spp_block1;
  SPRITE_PATTERN spp_block2;
  SPRITE_PATTERN spp_block3;
  SPRITE_PATTERN spp_star1;
  SPRITE_PATTERN spp_star2;
  SPRITE_PATTERN spp_star3;

  SPRITE sp_bar;
  SPRITE sp_ball;
  SPRITE sp_blocks[ NUM_OF_BLOCKS ];
  SPRITE sp_stars[ NUM_OF_STARS ];

} SPRITE_HANDLE;

// prototype declarations
void sprite_open(SPRITE_HANDLE* sprite, SCREEN_HANDLE* scr);
void sprite_close(SPRITE_HANDLE* sprite);
SPRITE* sprite_get(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id);
void sprite_scroll(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id);
void sprite_show(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id);
void sprite_hide(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id);

void sprite_direct_scroll(SPRITE* sprite);

#endif