#include <stdio.h>
#include <time.h>

#include "game.h"
#include "screen.h"
#include "sprite.h"
#include "keyboard.h"
#include "crtc.h"
#include "data_logo.h"

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

  adpcm_play(&game->adpcm_set->adpcm_ending_music);

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
void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_SET* sp_set, ADPCM_SET* adpcm_set) {
  game->high_score = 76500;
  game->score = 0;
  game->round = 1;
  game->scr = scr;
  game->sp_set = sp_set;
  game->adpcm_set = adpcm_set;
}

// game close
void game_close(GAME_HANDLE* game) {
  
}
