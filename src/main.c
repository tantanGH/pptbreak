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
#include "data_sprite.h"
#include "data_logo.h"

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
  SPRITE sp_blocks[ 48 ];
  SPRITE sp_stars[ 24 ];
} SPRITE_SET;

// game class
typedef struct {
  int high_score;
  int score;
  int round;
  SCREEN_HANDLE* scr;
  SPRITE_SET* sp_set;
} GAME_HANDLE;

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

// time wait
static void wait_time(int sec) {

  time_t t0 = time(NULL);
  time_t t1;

  do {
    t1 = time(NULL);
  } while ((t1 - t0) < sec);

}

// key wait with timeout
static int wait_key(int timeout) {

  time_t t0 = time(NULL);
  time_t t1 = t0;
  int scan_code = 0;

  while (timeout < 0 || t1 - t0 < timeout) {
    if (B_KEYSNS() != 0) {
      scan_code = B_KEYINP() >> 8;
      if (scan_code == KEY_SCAN_CODE_CR || scan_code == KEY_SCAN_CODE_ENTER ||
          scan_code == KEY_SCAN_CODE_SPACE || scan_code == KEY_SCAN_CODE_ESC) break;
    }
    t1 = time(NULL);
  }

  return scan_code;
}

// opening
int game_opening(GAME_HANDLE* game) {

  int scan_code = 0;

  static char high_score[] = "          ";
  static char score[]      = "          ";

  SCREEN_HANDLE* scr = game->scr;

  sprintf(high_score, "%10d", game->high_score);
  sprintf(score, "%10d", game->score);

  screen_fill_panel(scr, 0, 0);
  screen_fill_panel(scr, 1, 0);

  screen_clear_panel_text(scr, 0);
  screen_clear_panel_text(scr, 1);

  screen_put_text(scr, scr->panel_score_x + 8,   8, 2, "HIGH SCORE");
  screen_put_text(scr, scr->panel_score_x + 8,  20, 1, high_score);
  screen_put_text(scr, scr->panel_score_x + 8,  40, 2, "1UP");
  screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);  

  screen_scroll(scr, 0, 256);

  //screen_load_png(scr, 7, 50, 100, "title2.png");
  screen_put_image(scr, 7, 50, 269, 41, logo_data);

  do {

    screen_clear_panel_text(scr, 0);

    for (int i = 0; i < 256; i++) {

      WAIT_VSYNC;
      WAIT_VBLANK;

      screen_scroll(scr, 0, 256+i);

      // scroll skip
      if (B_KEYSNS() != 0) {
        int sc = B_KEYINP() >> 8;
        if (sc == KEY_SCAN_CODE_SPACE || sc == KEY_SCAN_CODE_CR || sc == KEY_SCAN_CODE_ESC) break;
      }

    }

    // reset scroll position
    screen_scroll(scr, 0, 0);

    // put text
    screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "PUSH SPACE KEY");
    screen_put_text_center(scr, 0, 210, scr->panel_game_width, 1, "2023 DESIGN AND PROGRAM BY TANTAN");

    // key wait with 20 sec timeout
    scan_code = wait_key(20);

  } while (scan_code != KEY_SCAN_CODE_SPACE && scan_code != KEY_SCAN_CODE_ESC);

  return scan_code;
}

// round start
void game_round_start(GAME_HANDLE* game) {

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;

  // clear game panel
  screen_clear_panel_text(scr, 0);
  screen_fill_panel(scr, 0, 1 + game->round % 4);
  screen_scroll(scr, 0, 0);

  // show round number
  static char round[]      = "          ";
  sprintf(round, "ROUND %d", game->round);
  screen_put_text(scr, scr->panel_score_x + 8, 200, 1, round);

  SPRITE* sp_bar = &(game->sp_set->sp_bar);
  SPRITE* sp_ball = &(game->sp_set->sp_ball);
  SPRITE* sp_blocks = &(game->sp_set->sp_blocks[0]);

  // show bar and blocks
  sp_scroll(sp_bar);
  for (int i = 0; i < 48; i++) {
    sp_scroll(&sp_blocks[i]);
  }

  // message
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "READY");
  wait_time(3);
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "     ");

  // show ball
  sp_scroll(sp_ball);
}

// ending
int game_ending(GAME_HANDLE* game) {

  SCREEN_HANDLE* scr = game->scr;
  SPRITE* sp_stars = &(game->sp_set->sp_stars[0]);

  WAIT_VSYNC;
  WAIT_VBLANK;

  screen_fill_panel(scr, 0, 0);
  screen_clear_panel_text(scr, 0);
  
  screen_scroll(scr, 0, 0);

  screen_put_text_center(scr, 0,  40, scr->panel_game_width, 4, "CONGRATULATIONS");
  screen_put_text_center(scr, 0,  70, scr->panel_game_width, 1, "YOU HAVE DEMOLISHED ALIENS");
  screen_put_text_center(scr, 0,  85, scr->panel_game_width, 1, "AND PEACE IS MAINTAINED.");
  screen_put_text_center(scr, 0, 100, scr->panel_game_width, 1, "THE UNIVERSE WILL NAME YOU");
  screen_put_text_center(scr, 0, 115, scr->panel_game_width, 2, "PPT MASTER.");
  screen_put_text_center(scr, 0, 130, scr->panel_game_width, 1, "YOU SHALL GET 10000PTS BONUS.");
  screen_put_text_center(scr, 0, 160, scr->panel_game_width, 1, "THE END");

  int scan_code = 0;

  for (;;) {

    if (B_KEYSNS() != 0) {
      scan_code = B_KEYINP() >> 8;
      if (scan_code == KEY_SCAN_CODE_SPACE || scan_code == KEY_SCAN_CODE_CR ||
          scan_code == KEY_SCAN_CODE_ENTER || scan_code == KEY_SCAN_CODE_ESC) break;
    }

    WAIT_VSYNC;
    WAIT_VBLANK;

    for (int i = 0; i < 24; i++) {
      sp_scroll(&sp_stars[i]);  
    }

    // move stars in z-axis
    for (int i = 0; i < 24; i++) {

      int d = scr->panel_game_depth;

      sp_stars[i].pos_z -= sp_stars[i].pos_z2;
      if (sp_stars[i].pos_z < 0) sp_stars[i].pos_z += d;

      float window_width = (float)d / 10.0 + ( (float)d - (float)d / 10.0 ) * ((float)sp_stars[i].pos_z / (float)d );
      float scale = (float)d / 10.0 / (float)window_width;

      sp_stars[i].pos_x = sp_stars[i].pos_x2 * scale + scr->panel_game_x + scr->panel_game_width  / 2;
      sp_stars[i].pos_y = sp_stars[i].pos_y2 * scale + scr->panel_game_y + scr->panel_game_height / 2;

      if (sp_stars[i].pos_x >= scr->panel_game_x && sp_stars[i].pos_x < scr->panel_game_x + scr->panel_game_width &&
          sp_stars[i].pos_y >= scr->panel_game_y && sp_stars[i].pos_y < scr->panel_game_y + scr->panel_game_height) {
        sp_stars[i].priority = 3;
      } else {
        sp_stars[i].priority = 0;
        sp_stars[i].pos_x = 0;
        sp_stars[i].pos_y = 0;
      }
    }
  }

  // hide stars
  for (int i = 0; i < 24; i++) {
    sp_stars[i].priority = 0;
    sp_scroll(&sp_stars[i]);  
  }

  return scan_code;
}

// game over
int game_over(GAME_HANDLE* game) {
  
  SCREEN_HANDLE* scr = game->scr;
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "GAME OVER");
  
  int scan_code = wait_key(20);

  // sprite pointers
  SPRITE* sp_bar = &(game->sp_set->sp_bar);
  sp_bar->priority = 0;
  sp_scroll(sp_bar);

  SPRITE* sp_ball = &(game->sp_set->sp_ball);
  sp_ball->priority = 0;
  sp_scroll(sp_ball);

  for (int i = 0; i < 48; i++) {  
    SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);
    sp_block->priority = 0;
    sp_scroll(sp_block);
  }

  return scan_code;
}

// game open
void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_SET* sp_set) {
  game->high_score = 76500;
  game->score = 0;
  game->round = 1;
  game->scr = scr;
  game->sp_set = sp_set;
}

// game close
void game_close(GAME_HANDLE* game) {
  resume_screen(game->scr);
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
  
  // initialize game object
  static GAME_HANDLE game;
  game_open(&game, &scr, &sp_set);

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
      }
      if (sp_ball->pos_x + 16 > scr.panel_game_x + scr.panel_game_width) {
        sp_ball->pos_x2 = -(1 + rand() % 4);
        sp_ball->pos_x -= 2 * (sp_ball->pos_x + 16 - scr.panel_game_x - scr.panel_game_width);
      }

      // collision check (ball and bar)
      if (sp_ball->pos_x+8 >= sp_bar->pos_x && sp_ball->pos_x+8 <= sp_bar->pos_x + 64 &&
          sp_ball->pos_y+10 >= sp_bar->pos_y && sp_ball->pos_y+10 <= sp_bar->pos_y+12) {
            sp_ball->pos_y2 = -(1 + rand() % 4);
            sp_ball->pos_y -= 2*(sp_ball->pos_y+10 - sp_bar->pos_y);
      }
      if (sp_ball->pos_y < sp_blocks[40].pos_y+8) {
        sp_ball->pos_y2 = (1 + rand() % 4);
        sp_ball->pos_y += 2*(sp_blocks[40].pos_y+8 - sp_ball->pos_y);
      }

      // ball out check
      if (sp_ball->pos_y >= scr.panel_game_y + scr.panel_game_height) {
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

  return rc;
}