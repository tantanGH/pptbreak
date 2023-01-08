#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iocslib.h>
#include <doslib.h>

#include "crtc.h"
#include "screen.h"
#include "keyboard.h"
#include "sprite.h"
#include "adpcm.h"
#include "data_sprite.h"
#include "data_adpcm.h"
#include "data_music.h"
#include "data_logo.h"
#include "game.h"

#define VERSION "0.1.0"

// setup screen
void setup_screen(SCREEN_HANDLE* scr) {

  scr->total_width = 384;
  scr->total_height = 512;

  scr->panel_game_width = 288;
  scr->panel_game_height = 256;
  scr->panel_game_depth = 2880;
  scr->panel_game_x = 0;
  scr->panel_game_y = 0;

  scr->panel_score_width = 96;
  scr->panel_score_height = 256;
  scr->panel_score_x = 288;
  scr->panel_score_y = 0;

  scr->panel_colors[0] = 0x0000;
  scr->panel_colors[1] = 0x794b;
  scr->panel_colors[2] = 0x18e1;
  scr->panel_colors[3] = 0x248b;  
  scr->panel_colors[4] = 0x515f;  

  scr->text_colors[0] = 0xffff;
  scr->text_colors[1] = 0x07c1;
  scr->text_colors[2] = 0xffc1;
        
  screen_init(scr);
  screen_init_font(scr);
}

// resume screen
void resume_screen(SCREEN_HANDLE* scr) {

  // resume screen
  screen_clear_panel_text(scr,0);
  screen_clear_panel_text(scr,1);
  screen_reset(scr);

}

// setup sprite patterns
static void setup_sprite_patterns(SPRITE_PATTERN_SET* spp_set) {

  SPRITE_PATTERN* spp_bar = &spp_set->spp_bar;
  spp_bar->pattern_id = 0;
  spp_bar->size_x = 4;
  spp_bar->size_y = 1;
  spp_bar->pattern_data = sp_pattern_data_bar1;
  spp_bar->palette_block = 1;
  spp_bar->palette_data = sp_palette_data_bar1;
  sp_setup_pattern(spp_bar);

  SPRITE_PATTERN* spp_ball = &spp_set->spp_ball;
  spp_ball->pattern_id = 4;
  spp_ball->size_x = 1;
  spp_ball->size_y = 1;
  spp_ball->pattern_data = sp_pattern_data_ball1;
  spp_ball->palette_block = 2;
  spp_ball->palette_data = sp_palette_data_ball1;
  sp_setup_pattern(spp_ball);

  SPRITE_PATTERN* spp_block1 = &spp_set->spp_block1;
  spp_block1->pattern_id = 5;
  spp_block1->size_x = 2;
  spp_block1->size_y = 1;
  spp_block1->pattern_data = sp_pattern_data_block1;
  spp_block1->palette_block = 3;
  spp_block1->palette_data = sp_palette_data_block1;
  sp_setup_pattern(spp_block1);

  SPRITE_PATTERN* spp_block2 = &spp_set->spp_block2;
  spp_block2->pattern_id = 7;
  spp_block2->size_x = 2;
  spp_block2->size_y = 1;
  spp_block2->pattern_data = sp_pattern_data_block2;
  spp_block2->palette_block = 4;
  spp_block2->palette_data = sp_palette_data_block2;
  sp_setup_pattern(spp_block2);
  
  SPRITE_PATTERN* spp_block3 = &spp_set->spp_block3;
  spp_block3->pattern_id = 9;
  spp_block3->size_x = 2;
  spp_block3->size_y = 1;
  spp_block3->pattern_data = sp_pattern_data_block3;
  spp_block3->palette_block = 5;
  spp_block3->palette_data = sp_palette_data_block3;
  sp_setup_pattern(spp_block3);

  SPRITE_PATTERN* spp_star1 = &spp_set->spp_star1;
  spp_star1->pattern_id = 11;
  spp_star1->size_x = 1;
  spp_star1->size_y = 1;
  spp_star1->pattern_data = sp_pattern_data_star1;
  spp_star1->palette_block = 6;
  spp_star1->palette_data = sp_palette_data_star1;
  sp_setup_pattern(spp_star1);

  SPRITE_PATTERN* spp_star2 = &spp_set->spp_star2;
  spp_star2->pattern_id = 12;
  spp_star2->size_x = 1;
  spp_star2->size_y = 1;
  spp_star2->pattern_data = sp_pattern_data_star2;
  spp_star2->palette_block = 7;
  spp_star2->palette_data = sp_palette_data_star2;
  sp_setup_pattern(spp_star2);

  SPRITE_PATTERN* spp_star3 = &spp_set->spp_star3;
  spp_star3->pattern_id = 13;
  spp_star3->size_x = 1;
  spp_star3->size_y = 1;
  spp_star3->pattern_data = sp_pattern_data_star3;
  spp_star3->palette_block = 8;
  spp_star3->palette_data = sp_palette_data_star3;
  sp_setup_pattern(spp_star3);

}

// setup sprites
void setup_sprites(SCREEN_HANDLE* scr, SPRITE_SET* sp_set, SPRITE_PATTERN_SET* spp_set) {

  // bar
  SPRITE* sp_bar = &sp_set->sp_bar;
  sp_bar->sprite_id = 0;
  sp_bar->pos_x = scr->panel_game_x + ( scr->panel_game_width - 64 ) / 2;
  sp_bar->pos_y = scr->panel_game_y + scr->panel_game_height * 90 / 100;
  sp_bar->pos_x2 = 4;     // velocity
  sp_bar->priority = 3;
  sp_bar->spp = &spp_set->spp_bar;

  // ball
  SPRITE* sp_ball = &sp_set->sp_ball;
  sp_ball->sprite_id = 4;
  sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width / 2;
  sp_ball->pos_y = scr->panel_game_y + scr->panel_game_height * 2 / 3;
  sp_ball->pos_x2 = 1;    // velocity
  sp_ball->pos_y2 = -1;   // velocity
  sp_ball->priority = 3;
  sp_ball->spp = &spp_set->spp_ball;

  // blocks
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(sp_set->sp_blocks[i]);
    sp_block->sprite_id = 5 + i*2;
    sp_block->pos_x = 16 + (i % 8) * 32;
    sp_block->pos_y = 16 + (i / 8) * 16;
    sp_block->priority = 3;
    if (i < 8) {
      sp_block->spp = &spp_set->spp_block1;
      sp_block->pos_z = 2;      // life
      sp_block->pos_z2 = 500;   // score
      sp_block->pos_y2 = 2;     // velocity
    } else if (i < 24) {
      sp_block->spp = &spp_set->spp_block2;
      sp_block->pos_z = 1;      // life
      sp_block->pos_z2 = 200;   // score
      sp_block->pos_y2 = 1;     // velocity
    } else {
      sp_block->spp = &spp_set->spp_block3;
      sp_block->pos_z = 1;      // life
      sp_block->pos_z2 = 100;   // score
      sp_block->pos_y2 = 0;     // velocity
    }
  }

  // stars (ending only)
  for (int i = 0; i < NUM_OF_STARS; i++) {
    SPRITE* sp_star = &(sp_set->sp_stars[i]);
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
        sp_star->spp = &spp_set->spp_star1;
        break;
      case 1:
        sp_star->spp = &spp_set->spp_star2;
        break;
      case 2:
        sp_star->spp = &spp_set->spp_star3;
        break;
    }
  }

}

// setup adpcm sound
static void setup_adpcm(ADPCM_SET* as) {
  
  as->adpcm_bar.data = adpcm_data2;
  as->adpcm_bar.length = ADPCM_DATA2_LENGTH;
  as->adpcm_bar.mode = ADPCM_MODE_15KHZ_LR;
  
  as->adpcm_block1.data = adpcm_data3;
  as->adpcm_block1.length = ADPCM_DATA3_LENGTH;
  as->adpcm_block1.mode = ADPCM_MODE_15KHZ_LR;
  
  as->adpcm_block2.data = adpcm_data5;
  as->adpcm_block2.length = ADPCM_DATA5_LENGTH;
  as->adpcm_block2.mode = ADPCM_MODE_15KHZ_LR;

  as->adpcm_wall1.data = adpcm_data4;
  as->adpcm_wall1.length = ADPCM_DATA4_LENGTH;
  as->adpcm_wall1.mode = ADPCM_MODE_15KHZ_L;

  as->adpcm_wall2.data = adpcm_data4;
  as->adpcm_wall2.length = ADPCM_DATA4_LENGTH;
  as->adpcm_wall2.mode = ADPCM_MODE_15KHZ_R;

  as->adpcm_wall3.data = adpcm_data4;
  as->adpcm_wall3.length = ADPCM_DATA4_LENGTH;
  as->adpcm_wall3.mode = ADPCM_MODE_15KHZ_LR;

  as->adpcm_over.data = adpcm_data1;
  as->adpcm_over.length = ADPCM_DATA1_LENGTH;
  as->adpcm_over.mode = ADPCM_MODE_15KHZ_LR;

  as->adpcm_ending_music.data = adpcm_data_music1;
  as->adpcm_ending_music.length = ADPCM_DATA_MUSIC1_LENGTH;
  as->adpcm_ending_music.mode = ADPCM_MODE_15KHZ_LR;
}

// main
int main(int argc, char* argv[]) {

  // version check
  if (argc >= 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-v") == 0)) {
    printf("pptbreak.x version " VERSION " 2023 by tantan\n");
    return 1;
  }

  // randomaize
  srand((unsigned int)time(NULL));

  // enter supervisor mode
  B_SUPER(0);

  // initialize screen,font and panels
  static SCREEN_HANDLE scr = { 0 };
  setup_screen(&scr);
  
  // define sprite patterns and palettes
  static SPRITE_PATTERN_SET spp_set;
  setup_sprite_patterns(&spp_set);
  
  // initialize sprite objects
  static SPRITE_SET sp_set;
  setup_sprites(&scr, &sp_set, &spp_set);
  
  // initialize adpcm sound objects
  static ADPCM_SET adpcm_set;
  setup_adpcm(&adpcm_set);

  // initialize game object
  static GAME_HANDLE game;
  game_open(&game, &scr, &sp_set, &adpcm_set, logo_data);

  // game loop
  for (;;) {

    // game opening
    if (game_opening_event(&game) != 0) goto exit;

    // reset game
    game_reset(&game);

    int rc = 0;
    do {

      // round start
      game_round_start_event(&game);

      // round loop
      rc = game_round_loop(&game);
    
      switch (rc) {

        case 0:  
          // round clear
          game_round_clear_event(&game);
          game.round++;
          break;

        case 1:
          // esc key
          goto exit;
      
        case 2:
          // game over
          game_over_event(&game);
          break;

        case -1:
          // fatal error
          goto exit;
      }
 
      if (game.round > 4) {
        // true end
        game_ending_event(&game);
        break;
      }

    } while (rc == 0);

  }

exit:
  // game close
  game_close(&game);

  // resume screen
  resume_screen(&scr);

  return 0;
}