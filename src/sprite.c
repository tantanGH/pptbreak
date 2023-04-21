#include <stdint.h>
#include <stdlib.h>
#include <iocslib.h>
#include "sprite.h"
#include "data_sprite.h"

// set sprite palette
static void sp_setup_palette(int16_t palette_block, uint16_t* palette_data) {
  for (int16_t i = 0; i < 16; i++) {
    REG_SP_PALETTE[ palette_block * 16 + i ] = palette_data[i];
  }
}

// set sprite patterns
static void sp_setup_pattern(SPRITE_PATTERN* spp) {
  int16_t pattern_count = spp->size_x * spp->size_y;
  for (int16_t i = 0; i < pattern_count; i++) {
    for (int16_t j = 0; j < 0x40; j++) {
      REG_PCG_DATA[ ( spp->pattern_id + i ) * 0x40 + j ] = spp->pattern_data[ i * 0x40 + j ];
    }
  }
  sp_setup_palette(spp->palette_block, spp->palette_data);
}

// scroll sprite
void sprite_direct_scroll(SPRITE* sp) {
  int16_t ofs = sp->sprite_id * 4;
  int16_t pattern = sp->spp->pattern_id;
  for (int16_t y = 0; y < sp->spp->size_y; y++) {
    for (int16_t x = 0; x < sp->spp->size_x; x++) {
      REG_SP_SCROLL[ofs++] = 16 + sp->pos_x + x*16;
      REG_SP_SCROLL[ofs++] = 16 + sp->pos_y + y*16;
      REG_SP_SCROLL[ofs++] = (sp->spp->palette_block << 8) | pattern++;
      REG_SP_SCROLL[ofs++] = sp->priority;
    }
  }
}

// setup sprite patterns
static void setup_sprite_patterns(SPRITE_HANDLE* sprite) {

  SPRITE_PATTERN* spp_bar = &(sprite->spp_bar);
  spp_bar->pattern_id = 0;
  spp_bar->size_x = 4;
  spp_bar->size_y = 1;
  spp_bar->pattern_data = sp_pattern_data_bar1;
  spp_bar->palette_block = 1;
  spp_bar->palette_data = sp_palette_data_bar1;
  sp_setup_pattern(spp_bar);

  SPRITE_PATTERN* spp_ball = &(sprite->spp_ball);
  spp_ball->pattern_id = 4;
  spp_ball->size_x = 1;
  spp_ball->size_y = 1;
  spp_ball->pattern_data = sp_pattern_data_ball1;
  spp_ball->palette_block = 2;
  spp_ball->palette_data = sp_palette_data_ball1;
  sp_setup_pattern(spp_ball);

  SPRITE_PATTERN* spp_block1 = &(sprite->spp_block1);
  spp_block1->pattern_id = 5;
  spp_block1->size_x = 2;
  spp_block1->size_y = 1;
  spp_block1->pattern_data = sp_pattern_data_block1;
  spp_block1->palette_block = 3;
  spp_block1->palette_data = sp_palette_data_block1;
  sp_setup_pattern(spp_block1);

  SPRITE_PATTERN* spp_block2 = &(sprite->spp_block2);
  spp_block2->pattern_id = 7;
  spp_block2->size_x = 2;
  spp_block2->size_y = 1;
  spp_block2->pattern_data = sp_pattern_data_block2;
  spp_block2->palette_block = 4;
  spp_block2->palette_data = sp_palette_data_block2;
  sp_setup_pattern(spp_block2);
  
  SPRITE_PATTERN* spp_block3 = &(sprite->spp_block3);
  spp_block3->pattern_id = 9;
  spp_block3->size_x = 2;
  spp_block3->size_y = 1;
  spp_block3->pattern_data = sp_pattern_data_block3;
  spp_block3->palette_block = 5;
  spp_block3->palette_data = sp_palette_data_block3;
  sp_setup_pattern(spp_block3);

  SPRITE_PATTERN* spp_star1 = &(sprite->spp_star1);
  spp_star1->pattern_id = 11;
  spp_star1->size_x = 1;
  spp_star1->size_y = 1;
  spp_star1->pattern_data = sp_pattern_data_star1;
  spp_star1->palette_block = 6;
  spp_star1->palette_data = sp_palette_data_star1;
  sp_setup_pattern(spp_star1);

  SPRITE_PATTERN* spp_star2 = &(sprite->spp_star2);
  spp_star2->pattern_id = 12;
  spp_star2->size_x = 1;
  spp_star2->size_y = 1;
  spp_star2->pattern_data = sp_pattern_data_star2;
  spp_star2->palette_block = 7;
  spp_star2->palette_data = sp_palette_data_star2;
  sp_setup_pattern(spp_star2);

  SPRITE_PATTERN* spp_star3 = &(sprite->spp_star3);
  spp_star3->pattern_id = 13;
  spp_star3->size_x = 1;
  spp_star3->size_y = 1;
  spp_star3->pattern_data = sp_pattern_data_star3;
  spp_star3->palette_block = 8;
  spp_star3->palette_data = sp_palette_data_star3;
  sp_setup_pattern(spp_star3);

}

// setup sprite instances
static void setup_sprite_instances(SPRITE_HANDLE* sprite, SCREEN_HANDLE* scr) {

  // bar
  SPRITE* sp_bar = &(sprite->sp_bar);
  sp_bar->sprite_id = 0;
  sp_bar->pos_x = scr->panel_game_x + ( scr->panel_game_width - 64 ) / 2;
  sp_bar->pos_y = scr->panel_game_y + scr->panel_game_height * 90 / 100;
  sp_bar->pos_x2 = 4;     // velocity
  sp_bar->priority = 3;
  sp_bar->spp = &(sprite->spp_bar);

  // ball
  SPRITE* sp_ball = &(sprite->sp_ball);
  sp_ball->sprite_id = 4;
  sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width / 2;
  sp_ball->pos_y = scr->panel_game_y + scr->panel_game_height * 2 / 3;
  sp_ball->pos_x2 = 1;    // velocity
  sp_ball->pos_y2 = -1;   // velocity
  sp_ball->priority = 3;
  sp_ball->spp = &(sprite->spp_ball);

  // blocks
  for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(sprite->sp_blocks[i]);
    sp_block->sprite_id = 5 + i*2;
    sp_block->pos_x = 16 + (i % 8) * 32;
    sp_block->pos_y = 16 + (i / 8) * 16;
    sp_block->priority = 3;
    if (i < 8) {
      sp_block->spp = &(sprite->spp_block1);
      sp_block->pos_z = 2;      // life
      sp_block->pos_z2 = 500;   // score
      sp_block->pos_y2 = 2;     // velocity
    } else if (i < 24) {
      sp_block->spp = &(sprite->spp_block2);
      sp_block->pos_z = 1;      // life
      sp_block->pos_z2 = 200;   // score
      sp_block->pos_y2 = 1;     // velocity
    } else {
      sp_block->spp = &(sprite->spp_block3);
      sp_block->pos_z = 1;      // life
      sp_block->pos_z2 = 100;   // score
      sp_block->pos_y2 = 0;     // velocity
    }
  }

  // stars (ending only)
  for (int16_t i = 0; i < NUM_OF_STARS; i++) {
    SPRITE* sp_star = &(sprite->sp_stars[i]);
    sp_star->sprite_id = 101 + i;
    sp_star->pos_x = 0;
    sp_star->pos_y = 0;
    sp_star->pos_z = (rand() % scr->panel_game_depth);                                // 0 <= z < 2880
    sp_star->pos_x2 = (rand() % scr->panel_game_depth) - scr->panel_game_depth / 2;   // -1440 <= x2 < 1440
    sp_star->pos_y2 = (rand() % scr->panel_game_depth) - scr->panel_game_depth / 2;   // -1440 <= y2 < 1440
    sp_star->pos_z2 = 80;   // z-velocity
    sp_star->priority = 3;
    switch (rand() % 3) {
      case 0:
        sp_star->spp = &(sprite->spp_star1);
        break;
      case 1:
        sp_star->spp = &(sprite->spp_star2);
        break;
      case 2:
        sp_star->spp = &(sprite->spp_star3);
        break;
    }
  }

}

void sprite_open(SPRITE_HANDLE* sprite, SCREEN_HANDLE* scr) {
  setup_sprite_patterns(sprite);
  setup_sprite_instances(sprite, scr);
}

void sprite_close(SPRITE_HANDLE* sprite) {

}

SPRITE* sprite_get(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id) {
  SPRITE* s = NULL;
  if (sp_id == SPID_BAR) {
    s = &(sprite->sp_bar);
  } else if (sp_id == SPID_BALL) {
    s = &(sprite->sp_ball);
  } else if (sp_id == SPID_BLOCK) {
    s = &(sprite->sp_blocks[ sub_id ]);
  } else if (sp_id == SPID_STAR) {
    s = &(sprite->sp_stars[ sub_id ]);
  }
  return s;
}

void sprite_scroll(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id) {
  SPRITE* s = sprite_get(sprite, sp_id, sub_id);
  if (s != NULL) {
    sprite_direct_scroll(s);
  }
}

void sprite_show(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id) {
  SPRITE* s = sprite_get(sprite, sp_id, sub_id);
  if (s != NULL) {
    s->priority = 3;
    sprite_direct_scroll(s);
  }
}

void sprite_hide(SPRITE_HANDLE* sprite, int16_t sp_id, int16_t sub_id) {
  SPRITE* s = sprite_get(sprite, sp_id, sub_id);
  if (s != NULL) {
    s->priority = 0;
    sprite_direct_scroll(s);
  }
}
