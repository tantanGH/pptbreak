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
#include "game.h"

#define USE_JOYSTICK

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
  scr->panel_colors[1] = 0x18e1;
  scr->panel_colors[2] = 0x794b;
  scr->panel_colors[3] = 0x515f;  
  scr->panel_colors[4] = 0x248b;  

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
  for (int i = 0; i < 48; i++) {
    SPRITE* sp_block = &(sp_set->sp_blocks[i]);
    sp_block->sprite_id = 5 + i*2;
    sp_block->pos_x = 16 + (i % 8) * 32;
    sp_block->pos_y = 16 + (i / 8) * 16;
    sp_block->priority = 3;
    if (i < 16) {
      sp_block->spp = &spp_set->spp_block1;
    } else if (i < 32) {
      sp_block->spp = &spp_set->spp_block2;
    } else {
      sp_block->spp = &spp_set->spp_block3;
    }
  }

  // stars (ending only)
  for (int i = 0; i < 24; i++) {
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

// adpcm setup
static void setup_adpcm(ADPCM_SET* as) {
  
  as->adpcm_bar.data = adpcm_data2;
  as->adpcm_bar.length = ADPCM_DATA2_LENGTH;
  as->adpcm_bar.mode = 4 * 256 + 2;
  
  as->adpcm_over.data = adpcm_data1;
  as->adpcm_over.length = ADPCM_DATA1_LENGTH;
  as->adpcm_over.mode = 4 * 256 + 3;
  
  as->adpcm_block1.data = adpcm_data3;
  as->adpcm_block1.length = ADPCM_DATA3_LENGTH;
  as->adpcm_block1.mode = 4 * 256 + 3;
  
  as->adpcm_block2.data = adpcm_data4;
  as->adpcm_block2.length = ADPCM_DATA4_LENGTH;
  as->adpcm_block2.mode = 4 * 256 + 3;

  as->adpcm_ending_music.data = adpcm_data_music1;
  as->adpcm_ending_music.length = ADPCM_DATA_MUSIC1_LENGTH;
  as->adpcm_ending_music.mode = 4 * 256 + 3;
}

// main
int main(int argc, char* argv[]) {

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
  game_open(&game, &scr, &sp_set, &adpcm_set);

  // game opening
  int scan_code = game_opening(&game);
  if (scan_code == KEY_SCAN_CODE_ESC) goto exit;

  // round loop
  int rc = 0;
  do {

    // sprite pointers
    SPRITE* sp_bar = &(sp_set.sp_bar);
    SPRITE* sp_ball = &(sp_set.sp_ball);
    SPRITE* sp_blocks = &(sp_set.sp_blocks[0]);

    // round start
    game_round_start(&game);

    // game loop
    for (;;) {

#ifdef USE_JOYSTICK
      // joy stick check
      int joy = JOYGET(0);
      if ((joy & JOY_SNS_LEFT) == 0) {
        if (sp_bar->pos_x > scr.panel_game_x) {
          sp_bar->pos_x -= sp_bar->pos_x2;
        }
      }
      if ((joy & JOY_SNS_RIGHT) == 0) {
        if (sp_bar->pos_x + 16 * sp_bar->spp->size_x < scr.panel_game_x + scr.panel_game_width) {
          sp_bar->pos_x += sp_bar->pos_x2;
        }
      }
#endif

      // keyboard check
      if (B_KEYSNS() != 0) {
        int scan_code = B_KEYINP() >> 8;
        switch (scan_code) {
          case KEY_SCAN_CODE_LEFT:
            if (sp_bar->pos_x > scr.panel_game_x) {
              sp_bar->pos_x -= sp_bar->pos_x2;
            }
            break;
          case KEY_SCAN_CODE_RIGHT:
            if (sp_bar->pos_x + 16 * sp_bar->spp->size_x < scr.panel_game_x + scr.panel_game_width) {
              sp_bar->pos_x += sp_bar->pos_x2;
            }
            break;
          case KEY_SCAN_CODE_ESC:
            goto exit;
        }
      }

      // ball move
      sp_ball->pos_x += sp_ball->pos_x2;
      sp_ball->pos_y += sp_ball->pos_y2;

      // collision check (ball and wall)
      if (sp_ball->pos_x < scr.panel_game_x) {
        sp_ball->pos_x2 = 1 + rand() % 4;
        sp_ball->pos_x += 2 * ( 0 - sp_ball->pos_x );
        adpcm_play(&adpcm_set.adpcm_block2);
      }
      if (sp_ball->pos_x + 16 > scr.panel_game_x + scr.panel_game_width) {
        sp_ball->pos_x2 = -(1 + rand() % 4);
        sp_ball->pos_x -= 2 * (sp_ball->pos_x + 16 - scr.panel_game_x - scr.panel_game_width);
        adpcm_play(&adpcm_set.adpcm_block2);
      }

      // collision check (ball and bar)
      if (sp_ball->pos_x+8 >= sp_bar->pos_x && sp_ball->pos_x+8 <= sp_bar->pos_x + 64 &&
          sp_ball->pos_y+10 >= sp_bar->pos_y && sp_ball->pos_y+10 <= sp_bar->pos_y+12) {
            sp_ball->pos_y2 = -(1 + rand() % 4);
            sp_ball->pos_y -= 2*(sp_ball->pos_y+10 - sp_bar->pos_y);
            adpcm_play(&adpcm_set.adpcm_bar);
      }
      if (sp_ball->pos_y < sp_blocks[40].pos_y+8) {
        sp_ball->pos_y2 = (1 + rand() % 4);
        sp_ball->pos_y += 2*(sp_blocks[40].pos_y+8 - sp_ball->pos_y);
        adpcm_play(&adpcm_set.adpcm_block1);
      }

      // ball out check
      if (sp_ball->pos_y >= scr.panel_game_y + scr.panel_game_height) {
        adpcm_play(&adpcm_set.adpcm_over);
        rc = 1;
        break;
      }

      WAIT_VSYNC;
      WAIT_VBLANK;

      sp_scroll(sp_bar);
      sp_scroll(sp_ball);

    }
    
    if (rc == 0) game.round++;

  } while (rc == 0);

  // game over
  if (rc != 0) {

    // game over
    game_over(&game);

    // true end
    game_ending(&game);

  } else {

    // true end
    game_ending(&game);

  }


exit:
  // game close
  game_close(&game);

  // resume screen
  resume_screen(&scr);

  return rc;
}