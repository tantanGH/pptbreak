#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "screen.h"
#include "sprite.h"
#include "keyboard.h"
#include "crtc.h"

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

// game open
void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_SET* sp_set, ADPCM_SET* adpcm_set, unsigned short* logo_data) {
  game->high_score = 76500;
  game->scr = scr;
  game->sp_set = sp_set;
  game->adpcm_set = adpcm_set;
  game->logo_data = logo_data;
  game_reset(game);
}

// game close
void game_close(GAME_HANDLE* game) {
  
}

// game reset
void game_reset(GAME_HANDLE* game) {
  if (game->score > game->high_score) {
    game->high_score = game->score;
  }
  game->score = 0;
  game->round = 1;
}

// opening
int game_opening_event(GAME_HANDLE* game) {

  int rc = 0;

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
  screen_put_image(scr, 7, 50, 269, 41, game->logo_data);

  for (;;) {

    screen_clear_panel_text(scr, 0);

    for (int i = 0; i < 256; i++) {

      WAIT_VSYNC;
      WAIT_VBLANK;

      screen_scroll(scr, 0, 256+i);

      // scroll skip
      if (B_KEYSNS() != 0) {
        int sc = B_KEYINP() >> 8;
        if (sc == KEY_SCAN_CODE_SPACE || sc == KEY_SCAN_CODE_CR) break;
      }

    }

    // reset scroll position
    screen_scroll(scr, 0, 0);

    // put text
    screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "PUSH SPACE KEY");
    screen_put_text_center(scr, 0, 210, scr->panel_game_width, 1, "2023 DESIGN AND PROGRAM BY TANTAN");

    // key wait with 20 sec timeout
    int scan_code = wait_key(20);
    if (scan_code == KEY_SCAN_CODE_SPACE) {
      rc = 0;
      break;
    } else if (scan_code == KEY_SCAN_CODE_ESC) {
      rc = 1;
      break;
    }

  }

  return rc;
}

// ending
int game_ending_event(GAME_HANDLE* game) {

  WAIT_VSYNC;
  WAIT_VBLANK;

  // hide bar
  SPRITE* sp_bar = &(game->sp_set->sp_bar);
  sp_bar->priority = 0;
  sp_scroll(sp_bar);

  // hide ball
  SPRITE* sp_ball = &(game->sp_set->sp_ball);
  sp_ball->priority = 0;
  sp_scroll(sp_ball);

  // hide blocks
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {  
    SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);
    sp_block->priority = 0;
    sp_scroll(sp_block);
  }

  SCREEN_HANDLE* scr = game->scr;
  SPRITE* sp_stars = &(game->sp_set->sp_stars[0]);

  game->score += 10000;

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
  
  static char score[] = "             ";
  sprintf(score, "%10d", game->score);
  screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);
  
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

    for (int i = 0; i < NUM_OF_STARS; i++) {
      sp_scroll(&sp_stars[i]);  
    }

    // move stars in z-axis
    for (int i = 0; i < NUM_OF_STARS; i++) {

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
  for (int i = 0; i < NUM_OF_STARS; i++) {
    sp_stars[i].priority = 0;
    sp_scroll(&sp_stars[i]);  
  }

  return scan_code;
}

// game over
int game_over_event(GAME_HANDLE* game) {
  
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

  for (int i = 0; i < NUM_OF_BLOCKS; i++) {  
    SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);
    sp_block->priority = 0;
    sp_scroll(sp_block);
  }

  return scan_code;
}

// round start
void game_round_start_event(GAME_HANDLE* game) {

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // hide ball
  SPRITE* sp_ball = &(game->sp_set->sp_ball);
  sp_ball->priority = 0;
  sp_scroll(sp_ball);

  // reset bar position
  SPRITE* sp_bar = &(game->sp_set->sp_bar);
  sp_bar->pos_x = scr->panel_game_x + ( scr->panel_game_width - 64 ) / 2;
  sp_bar->pos_y = scr->panel_game_y + scr->panel_game_height * 90 / 100;
  sp_bar->pos_x2 = 4;     // velocity
  sp_bar->priority = 3;
  sp_bar->invalidate = 0;

  // reset ball position
  //SPRITE* sp_ball = &(game->sp_set->sp_ball);
  sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width / 2;
  sp_ball->pos_y = scr->panel_game_y + scr->panel_game_height * 2 / 3;
  sp_ball->pos_x2 = 1;    // velocity
  sp_ball->pos_y2 = -1;   // velocity
  sp_ball->priority = 3;
  sp_ball->invalidate = 0;

  // reset blocks
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);
    sp_block->pos_x = 16 + (i % 8) * 32;
    sp_block->pos_y = 16 + (i / 8) * 16;
    sp_block->priority = 3;
    sp_block->invalidate = 0;
    if (i < 6) {
      sp_block->pos_z = 2;      // life
    } else if (i < 24) {
      sp_block->pos_z = 1;      // life
    } else {
      sp_block->pos_z = 1;      // life
    }
  }

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;

  // clear game panel
  screen_clear_panel_text(scr, 0);
  screen_fill_panel(scr, 0, 1 + ( game->round - 1 ) % 4);
  screen_scroll(scr, 0, 0);

  // show score
  static char score[] = "             ";
  sprintf(score, "%10d", game->score);
  screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);

  // show round number
  static char round[] = "          ";
  sprintf(round, "ROUND %d", game->round);
  screen_put_text(scr, scr->panel_score_x + 8, 200, 1, round);

  // show bar and blocks
  sp_scroll(sp_bar);
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);
    sp_scroll(sp_block);
  }

  // message
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "READY");
  wait_time(3);
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "     ");

  // show ball
  sp_scroll(sp_ball);
}

// round clear
void game_round_clear_event(GAME_HANDLE* game) {

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;

  // message
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "ROUND CLEAR");
  wait_time(5);
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "           ");
}

//  vdisk interrupt handler
volatile static GAME_HANDLE* vsync_game;
volatile static int vsync_counter;
static void __attribute__((interrupt)) game_round_vsync_interrupt() {

  // sprite pointers
  SPRITE* sp_bar = &(vsync_game->sp_set->sp_bar);
  SPRITE* sp_ball = &(vsync_game->sp_set->sp_ball);

  if (sp_bar->invalidate) {
    sp_scroll(sp_bar);
    sp_bar->invalidate = 0;
  }
  if (sp_ball->invalidate) {
    sp_scroll(sp_ball);
    sp_ball->invalidate = 0;
  }
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(vsync_game->sp_set->sp_blocks[i]);
    if (sp_block->invalidate) {
      sp_scroll(sp_block);
      sp_block->invalidate = 0;
    }
  }
  if (vsync_game->score_invalidate) {
    SCREEN_HANDLE* scr = vsync_game->scr;
    static char score[] = "             ";
    sprintf(score, "%10d", vsync_game->score);
    screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);
    vsync_game->score_invalidate = 0;
  }

  vsync_counter = (vsync_counter + 1) & 0xfffffff;
}

// game round loop
int game_round_loop(GAME_HANDLE* game) {

  // return code
  int rc = 0;     // 0: clear  1: esc  -1: game over

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // sprite pointers
  SPRITE* sp_bar = &(game->sp_set->sp_bar);
  SPRITE* sp_ball = &(game->sp_set->sp_ball);

  // adpcm set
  ADPCM_SET* adpcm_set = game->adpcm_set;

  // number of blocks remained
  int remained_blocks = NUM_OF_BLOCKS;

  // enable vsync interrupt handling
  vsync_game = game;
  if (VDISPST((unsigned char*)game_round_vsync_interrupt, 0, 1) != 0) {   // VBLANK, 1/60
    printf("error: cannot use VSYNC interrupt.\n");
    rc = -1;
    goto exit;
  } 
 
  // round loop
  for (;;) {

    // wait
    int vsync_counter_current = vsync_counter;
    while (vsync_counter == vsync_counter_current) {
      ;
    }

    // no more blocks?
    if (remained_blocks <= 0) break;

    // joy stick check
    int joy = JOYGET(0);
    if ((joy & JOY_SNS_LEFT) == 0) {
      if (sp_bar->pos_x > scr->panel_game_x) {
        sp_bar->pos_x -= sp_bar->pos_x2;
        sp_bar->invalidate = 1;
      }
    }
    if ((joy & JOY_SNS_RIGHT) == 0) {
      if ((sp_bar->pos_x + 16 * sp_bar->spp->size_x - 1) < scr->panel_game_x + scr->panel_game_width) {
        sp_bar->pos_x += sp_bar->pos_x2;
        sp_bar->invalidate = 1;
      }
    }

    // keyboard check
    if (BITSNS(KEY_GRP_LEFT) & KEY_SNS_LEFT) {
      if (sp_bar->pos_x > scr->panel_game_x) {
        sp_bar->pos_x -= sp_bar->pos_x2;
        sp_bar->invalidate = 1;
      }
    }
    if (BITSNS(KEY_GRP_RIGHT) & KEY_SNS_RIGHT) {
      if ((sp_bar->pos_x + 16 * sp_bar->spp->size_x -1) < scr->panel_game_x + scr->panel_game_width) {
        sp_bar->pos_x += sp_bar->pos_x2;
        sp_bar->invalidate = 1;
      }
    }
    if (BITSNS(KEY_GRP_ESC) & KEY_SNS_ESC) {
      rc = 1;
      goto exit;
    }

    // ball move
    sp_ball->pos_x += sp_ball->pos_x2;
    sp_ball->pos_y += sp_ball->pos_y2;
    sp_ball->invalidate = 1;

    // yellow block move (round 4 only)
    if (game->round >= 4) {
      SPRITE* sp_blocks = &(game->sp_set->sp_blocks[8]);
      for (int i = 0; i < 8; i++) {
        if ((sp_blocks[8*1+i].pos_z | sp_blocks[8*2+i].pos_z | sp_blocks[8*3+i].pos_z) == 0) {
          // if all the below blocks are broken
          sp_blocks[i].pos_y += sp_blocks[i].pos_y2;
          sp_blocks[i].invalidate = 1;
          if (sp_blocks[i].pos_y2 > 0 && sp_blocks[i].pos_y >= scr->panel_game_y + scr->panel_game_width * 1 / 2) {
            sp_blocks[i].pos_y2 *= -1;
          } else if (sp_blocks[i].pos_y2 < 0 && sp_blocks[i].pos_y <= scr->panel_game_y + 32) {
            sp_blocks[i].pos_y2 *= -1;
          }
        }
      }
    }

    // silver block move (round 3,4 only)
    if (game->round >= 3) {
      SPRITE* sp_blocks = &(game->sp_set->sp_blocks[0]);
      for (int i = 0; i < 8; i++) {
        if ((sp_blocks[8*1+i].pos_z | sp_blocks[8*2+i].pos_z | sp_blocks[8*3+i].pos_z | sp_blocks[8*4+i].pos_z) == 0) {
          // if all the below blocks are broken
          sp_blocks[i].pos_y += sp_blocks[i].pos_y2;
          sp_blocks[i].invalidate = 1;
          if (sp_blocks[i].pos_y2 > 0 && sp_blocks[i].pos_y >= scr->panel_game_y + scr->panel_game_width * 3 / 5) {
            sp_blocks[i].pos_y2 *= -1;
          } else if (sp_blocks[i].pos_y2 < 0 && sp_blocks[i].pos_y <= scr->panel_game_y + 16) {
            sp_blocks[i].pos_y2 *= -1;
          }
        }
      }
    }

    // collision check (ball and wall)
    if ((sp_ball->pos_x +5) < scr->panel_game_x) {    // left wall
      sp_ball->pos_x2 = 1 + rand() % 4;
      sp_ball->pos_x += 2 * ( scr->panel_game_x - sp_ball->pos_x -5 );
      sp_ball->invalidate = 1;
      adpcm_play(&adpcm_set->adpcm_wall1);
    }
    if ((sp_ball->pos_x + 11) > scr->panel_game_x + scr->panel_game_width) {    // right wall
      sp_ball->pos_x2 = -(1 + rand() % 4);
      sp_ball->pos_x -= 2 * ( sp_ball->pos_x +11 - scr->panel_game_x - scr->panel_game_width );
      sp_ball->invalidate = 1;
      adpcm_play(&adpcm_set->adpcm_wall2);
    }
    if ((sp_ball->pos_y +3) < scr->panel_game_y) {    // top wall
      sp_ball->pos_y2 = 1 + rand() % 4;
      sp_ball->pos_y += 2 * ( scr->panel_game_y - sp_ball->pos_y -3 );
      sp_ball->invalidate = 1;
      adpcm_play(&adpcm_set->adpcm_wall3);
    }

    // collision check (ball and bar)
    if (sp_ball->pos_x +11 >= sp_bar->pos_x && sp_ball->pos_x +5 <= sp_bar->pos_x +63 &&
        sp_ball->pos_y +10 >= sp_bar->pos_y && sp_ball->pos_y +10 <= sp_bar->pos_y +14 ) {
          sp_ball->pos_y2 = -(1 + rand() % 4);
          sp_ball->pos_y -= 2*(sp_ball->pos_y +10 - sp_bar->pos_y);   // adjust overflown position
          sp_ball->invalidate = 1;
          adpcm_play(&adpcm_set->adpcm_bar);
    }

    // collision check (ball and block)
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {

      SPRITE* sp_block = &(game->sp_set->sp_blocks[i]);

      // if this block is already broken, skip
      if (sp_block->pos_z <= 0) continue;

      int hit = 0;
      if (sp_ball->pos_x +11 >= sp_block->pos_x && sp_ball->pos_x +5 <= sp_block->pos_x +31 &&
          ((sp_ball->pos_y2 < 0 && sp_ball->pos_y +5 < sp_block->pos_y +14 && sp_ball->pos_y +5 > sp_block->pos_y +4) ||
           (sp_ball->pos_y2 > 0 && sp_ball->pos_y +11 > sp_block->pos_y && sp_ball->pos_y +11 < sp_block->pos_y +12))) {

        if (sp_ball->pos_y2 < 0) {
          // bottom hit
          sp_ball->pos_y2 = 1 + rand() % 4;
          sp_ball->pos_y += 2*(sp_block->pos_y +12 - sp_ball->pos_y -5);
        } else {
          // top hit
          sp_ball->pos_y2 = -(1 + rand() % 4);
          sp_ball->pos_y -= 2*(sp_block->pos_y - sp_ball->pos_y -11);
        }

        hit = 1;

      } else if (sp_ball->pos_y +5 >= sp_block->pos_y +15 && sp_ball->pos_y +11 <= sp_block->pos_y &&
                 ((sp_ball->pos_x2 < 0 && sp_ball->pos_x +5 < sp_block->pos_x +31 && sp_ball->pos_x +5 > sp_block->pos_x +15) ||
                  (sp_ball->pos_x2 > 0 && sp_ball->pos_x +11 > sp_block->pos_x && sp_ball->pos_x +11 < sp_block->pos_x +16))) {

        if (sp_ball->pos_x2 < 0) {
          // right hit
          sp_ball->pos_x2 = 1 + rand() % 4;
          sp_ball->pos_x += 2*(sp_block->pos_x +31 - sp_ball->pos_x -5);
        } else {
          // left hit
          sp_ball->pos_x2 = -(1 + rand() % 4);
          sp_ball->pos_x -= 2*(sp_block->pos_x - sp_ball->pos_x -11);
        }

        hit = 1;

      }

      // get more score at lower block position
      if (hit) {
        sp_block->pos_z--;
        if (sp_block->pos_z <= 0) {
          game->score += sp_block->pos_z2 * ((sp_block->pos_y > scr->panel_game_y + scr->panel_game_height/2) ? 2 : 1);
          game->score_invalidate = 1;
          sp_block->priority = 0;
          remained_blocks--;
        }
        sp_block->invalidate = 1;
        if (i < 8) {
          adpcm_play(&adpcm_set->adpcm_block2);
        } else {
          adpcm_play(&adpcm_set->adpcm_block1);
        }
      }

    }

    // ball out check
    if (sp_ball->pos_y >= scr->panel_game_y + scr->panel_game_height) {
      adpcm_play(&adpcm_set->adpcm_over);
      rc = 2;   // game over
      break;
    }

  }

  // wait vsync
  WAIT_VBLANK;
  WAIT_VSYNC;

  // stop vsync interrupt handling
  VDISPST(NULL, 0, 0);

exit:
  // flush key buffers
  while (B_KEYSNS() != 0) {
    B_KEYINP();
  }

  return rc;    
}
