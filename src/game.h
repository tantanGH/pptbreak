#ifndef __H_GAME__
#define __H_GAME__

#include <stdint.h>
#include "screen.h"
#include "sprite.h"
#include "adpcm.h"
#include "data_logo.h"

#define NUM_OF_BLOCKS (40)
#define NUM_OF_STARS  (24)

// game class
typedef struct {
  int16_t round;
  int32_t high_score;
  int32_t score;
  volatile int16_t score_invalidate;
  SCREEN_HANDLE* scr;
  SPRITE_HANDLE* sprite;
  ADPCM_HANDLE* adpcm;
} GAME_HANDLE;

void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_HANDLE* sprite, ADPCM_HANDLE* adpcm_handle);
void game_close(GAME_HANDLE* game);
int32_t game_opening_event(GAME_HANDLE* game);
int32_t game_ending_event(GAME_HANDLE* game);
int32_t game_over_event(GAME_HANDLE* game);
void game_reset(GAME_HANDLE* game);
void game_round_start_event(GAME_HANDLE* game);
void game_round_clear_event(GAME_HANDLE* game);
int32_t game_round_loop(GAME_HANDLE* game);

#endif