#ifndef __H_GAME__
#define __H_GAME__

#include "screen.h"
#include "sprite.h"
#include "adpcm.h"

#define NUM_OF_BLOCKS (40)
#define NUM_OF_STARS  (24)

// sprite pattern set
typedef struct {
  SPRITE_PATTERN spp_bar;
  SPRITE_PATTERN spp_ball;
  SPRITE_PATTERN spp_block1;
  SPRITE_PATTERN spp_block2;
  SPRITE_PATTERN spp_block3;
  SPRITE_PATTERN spp_star1;
  SPRITE_PATTERN spp_star2;
  SPRITE_PATTERN spp_star3;
} SPRITE_PATTERN_SET;

// sprite set
typedef struct {
  SPRITE sp_bar;
  SPRITE sp_ball;
  SPRITE sp_blocks[ NUM_OF_BLOCKS ];
  SPRITE sp_stars[ NUM_OF_STARS ];
} SPRITE_SET;

// adpcm set
typedef struct {
  ADPCM_SOUND adpcm_bar;
  ADPCM_SOUND adpcm_block1;
  ADPCM_SOUND adpcm_block2;
  ADPCM_SOUND adpcm_over;
  ADPCM_SOUND adpcm_ending_music;
} ADPCM_SET;

// game class
typedef struct {
  int round;
  int high_score;
  int score;
  volatile int score_invalidate;
  SCREEN_HANDLE* scr;
  SPRITE_SET* sp_set;
  ADPCM_SET* adpcm_set;
  unsigned short* logo_data;
} GAME_HANDLE;

void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_SET* sp_set, ADPCM_SET* adpcm_set, unsigned short* logo_data);
void game_close(GAME_HANDLE* game);
int game_opening_event(GAME_HANDLE* game);
int game_ending_event(GAME_HANDLE* game);
int game_over_event(GAME_HANDLE* game);
void game_reset(GAME_HANDLE* game);
void game_round_start_event(GAME_HANDLE* game);
void game_round_clear_event(GAME_HANDLE* game);
int game_round_loop(GAME_HANDLE* game);

#endif