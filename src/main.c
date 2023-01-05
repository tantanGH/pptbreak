#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iocslib.h>
#include <doslib.h>

#include "screen.h"
#include "sprite.h"
#include "sprite_pattern.h"

// game class
typedef struct {
  int high_score;
  int score;
  int round;
} GAME;

// setup screen
void setup_screen(SCREEN_HANDLE* scr) {

  scr->total_width = 384;
  scr->total_height = 256;

  scr->panel_game_width = 288;
  scr->panel_game_height = 256;
  scr->panel_game_x = 0;
  scr->panel_game_y = 0;

  scr->panel_score_width = 96;
  scr->panel_score_height = 256;
  scr->panel_score_x = 288;
  scr->panel_score_y = 0;

  scr->panel_colors[0] = 0x18e1;
  scr->panel_colors[1] = 0x794b;
  scr->panel_colors[2] = 0x515f;  
  scr->panel_colors[3] = 0x248b;  

  scr->text_colors[0] = 0xffff;
  scr->text_colors[1] = 0x07c1;
  scr->text_colors[2] = 0xffc1;
        
  screen_init(scr);
  screen_init_font(scr);

  // setup game panel
  screen_fill_panel(scr, 1, 0);

  // setup score panel
  screen_put_text(scr, scr->panel_score_x + 8,   8, 2, "HIGH SCORE");
  screen_put_text(scr, scr->panel_score_x + 8,  20, 1, "     76500");
  screen_put_text(scr, scr->panel_score_x + 8,  40, 2, "1UP");
  screen_put_text(scr, scr->panel_score_x + 8,  50, 1, "         0");
  screen_put_text(scr, scr->panel_score_x + 8, 200, 1, "ROUND 1");
}

// setup sprite patterns
static void setup_sprite_patterns(SPRITE_PATTERN* spp_bar,
                                  SPRITE_PATTERN* spp_ball,
                                  SPRITE_PATTERN* spp_block1,
                                  SPRITE_PATTERN* spp_block2,
                                  SPRITE_PATTERN* spp_block3) {

  spp_bar->pattern_id = 0;
  spp_bar->size_x = 4;
  spp_bar->size_y = 1;
  spp_bar->pattern_data = sp_pattern_data_bar1;
  spp_bar->palette_block = 1;
  spp_bar->palette_data = sp_palette_data_bar1;
  sp_setup_pattern(spp_bar);

  spp_ball->pattern_id = 4;
  spp_ball->size_x = 1;
  spp_ball->size_y = 1;
  spp_ball->pattern_data = sp_pattern_data_ball1;
  spp_ball->palette_block = 2;
  spp_ball->palette_data = sp_palette_data_ball1;
  sp_setup_pattern(spp_ball);

  spp_block1->pattern_id = 5;
  spp_block1->size_x = 2;
  spp_block1->size_y = 1;
  spp_block1->pattern_data = sp_pattern_data_block1;
  spp_block1->palette_block = 3;
  spp_block1->palette_data = sp_palette_data_block1;
  sp_setup_pattern(spp_block1);

  spp_block2->pattern_id = 7;
  spp_block2->size_x = 2;
  spp_block2->size_y = 1;
  spp_block2->pattern_data = sp_pattern_data_block2;
  spp_block2->palette_block = 4;
  spp_block2->palette_data = sp_palette_data_block2;
  sp_setup_pattern(spp_block2);
  
  spp_block3->pattern_id = 9;
  spp_block3->size_x = 2;
  spp_block3->size_y = 1;
  spp_block3->pattern_data = sp_pattern_data_block3;
  spp_block3->palette_block = 5;
  spp_block3->palette_data = sp_palette_data_block3;
  sp_setup_pattern(spp_block3);

}

// setup sprites
void setup_sprites(SCREEN_HANDLE* scr, 
                   SPRITE* sp_bar, 
                   SPRITE* sp_ball, 
                   SPRITE* sp_blocks[],
                   SPRITE_PATTERN* spp_bar,
                   SPRITE_PATTERN* spp_ball,
                   SPRITE_PATTERN* spp_block1,
                   SPRITE_PATTERN* spp_block2,
                   SPRITE_PATTERN* spp_block3) {

  sp_bar->sprite_id = 0;
  sp_bar->pos_x = scr->panel_game_x + ( scr->panel_game_width - 64 ) / 2;
  sp_bar->pos_y = scr->panel_game_y + scr->panel_game_height * 90 / 100;
  sp_bar->priority = 3;
  sp_bar->spp = spp_bar;

  sp_ball->sprite_id = 4;
  sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width / 2;
  sp_ball->pos_y = scr->panel_game_y + scr->panel_game_height * 2 / 3;
  sp_ball->priority = 3;
  sp_ball->spp = spp_ball;

  for (int i = 0; i < 48; i++) {
    sp_blocks[i]->sprite_id = 5 + i*2;
    sp_blocks[i]->pos_x = 16 + (i % 8) * 32;
    sp_blocks[i]->pos_y = 16 + (i / 8) * 16;
    sp_blocks[i]->priority = 3;
    if (i < 16) {
      sp_blocks[i]->spp = spp_block1;
    } else if (i < 32) {
      sp_blocks[i]->spp = spp_block2;
    } else {
      sp_blocks[i]->spp = spp_block3;
    }
  }

}

// wait
inline static void wait_seconds(int sec) {

  time_t t0 = time(NULL);
  time_t t1;

  do {
    t1 = time(NULL);
  } while ((t1 - t0) < 4);

}

// main
int main(int argc, char* argv[]) {

  // enter supervisor mode
  B_SUPER(0);

  // initialize screen,font and panels
  static SCREEN_HANDLE scr = { 0 };
  setup_screen(&scr);
  
  // define sprite patterns and palettes
  static SPRITE_PATTERN spp_bar;
  static SPRITE_PATTERN spp_ball;
  static SPRITE_PATTERN spp_block1;
  static SPRITE_PATTERN spp_block2;
  static SPRITE_PATTERN spp_block3;
  setup_sprite_patterns(&spp_bar, &spp_ball, &spp_block1, &spp_block2, &spp_block3);
  
  // initialize sprite objects
  static SPRITE sp_bar;
  static SPRITE sp_ball;
  static SPRITE sp_blocks[48];
  setup_sprites(&scr, &sp_bar, &sp_ball, (SPRITE**)&sp_blocks, &spp_bar, &spp_ball, &spp_block1, &spp_block2, &spp_block3);

  // ball velocity
  int ball_ax = 1;
  int ball_ay = -1;

  // exit code
  int rc = 0;

  // randomaize
  srand((unsigned int)time(NULL));

  // ready screen  
  WAIT_VSYNC;
  WAIT_VBLANK;

  sp_scroll(&sp_bar);
  for (int i = 0; i < 48; i++) {
    sp_scroll(&sp_blocks[i]);
  }

  screen_put_text(&scr, (scr.panel_game_width-8*5)/2, 150, 1, "READY");
  wait_seconds(3);
  screen_put_text(&scr, (scr.panel_game_width-8*5)/2, 150, 1, "     ");

  sp_scroll(&sp_ball);

  // game loop
  for (;;) {

    int joy = JOYGET(0);
    if ((joy & 4) == 0 && sp_bar.pos_x >= scr.panel_game_x+4) {
      sp_bar.pos_x -= 4;
    }
    if ((joy & 8) == 0 && sp_bar.pos_x <= scr.panel_game_x + scr.panel_game_width-4*16) {
      sp_bar.pos_x += 4;
    }

    sp_ball.pos_x += ball_ax;
    sp_ball.pos_y += ball_ay;
    if (sp_ball.pos_x < scr.panel_game_x) {
      ball_ax = 1 + rand() % 4;
      sp_ball.pos_x += 2*(0 - sp_ball.pos_x);
    }
    if (sp_ball.pos_x + 8 > scr.panel_game_x + scr.panel_game_width) {
      ball_ax = -(1 + rand() % 4);
      sp_ball.pos_x -= 2*(sp_ball.pos_x+8 - scr.panel_game_x - scr.panel_game_width);
    }

    if (sp_ball.pos_x+8 >= sp_bar.pos_x && sp_ball.pos_x+8 <= sp_bar.pos_x + 64 &&
        sp_ball.pos_y+10 >= sp_bar.pos_y && sp_ball.pos_y+10 <= sp_bar.pos_y+12) {
          ball_ay = -(1 + rand() % 4);
          sp_ball.pos_y -= 2*(sp_ball.pos_y+10 - sp_bar.pos_y);
        }
    if (sp_ball.pos_y < sp_blocks[40].pos_y+8) {
      ball_ay = (1 + rand() % 4);
      sp_ball.pos_y += 2*(sp_blocks[40].pos_y+8 - sp_ball.pos_y);
    }

    if (sp_ball.pos_y >= scr.panel_game_y + scr.panel_game_height) {
      rc = 1;
      break;
    }

    WAIT_VSYNC;
    WAIT_VBLANK;
    sp_scroll(&sp_bar);
    sp_scroll(&sp_ball);

    if (BITSNS(0) & 0x02) {   // ESC
      break;
    }
  }

  if (rc != 0) {
    screen_put_text(&scr, scr.panel_game_x + (scr.panel_game_width-8*9)/2, 150, 1, "GAME OVER");
  }

  // key wait
  for (;;) {
    if (B_KEYSNS() != 0) {
      int key_code = B_KEYINP();
      if (key_code == (9*8 + 6) ||      // ENTER
          key_code == (6*8 + 5) ||      // SPACE
          key_code == (0*8 + 1)) {      // ESC
            break;
          }
    }
  }

  screen_reset(&scr);

  return rc;
}