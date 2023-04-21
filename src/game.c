#include <stdio.h>
#include <stdlib.h>
#include <iocslib.h>

#include "screen.h"
#include "sprite.h"
#include "keyboard.h"
#include "crtc.h"
#include "game.h"

// time wait
static inline void wait_time(int32_t msec) {
  for (uint32_t t0 = ONTIME(); ONTIME() < t0 + msec/10; ) {}
}

// key wait with timeout
static int32_t wait_key(int32_t msec) {

  uint16_t t0 = ONTIME();
  uint16_t t1 = t0;
  int32_t scan_code = 0;

  while (msec < 0 || t1 - t0 < msec/10) {
    if (B_KEYSNS() != 0) {
      scan_code = B_KEYINP() >> 8;
      if (scan_code == KEY_SCAN_CODE_CR || scan_code == KEY_SCAN_CODE_ENTER ||
          scan_code == KEY_SCAN_CODE_SPACE || scan_code == KEY_SCAN_CODE_ESC) break;
    }
    t1 = ONTIME();
  }

  return scan_code;
}

// game open
void game_open(GAME_HANDLE* game, SCREEN_HANDLE* scr, SPRITE_HANDLE* sprite, ADPCM_HANDLE* adpcm) {
  game->high_score = 76500;
  game->scr = scr;
  game->sprite = sprite;
  game->adpcm = adpcm;
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
int32_t game_opening_event(GAME_HANDLE* game) {

  int32_t rc = 0;

  static uint8_t high_score[] = "          ";
  static uint8_t score[]      = "          ";

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

  screen_put_image(scr, 7, 50, 269, 41, title_logo_data);

  for (;;) {

    screen_clear_panel_text(scr, 0);

    for (int16_t i = 0; i < 256; i++) {

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
    screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "PUSH SPACE KEY TO START");
    screen_put_text_center(scr, 0, 166, scr->panel_game_width, 1, "OR ESC KEY TO EXIT");
    screen_put_text_center(scr, 0, 210, scr->panel_game_width, 1, "2023 PRODUCED BY tantan");

    // key wait with 20 sec timeout
    int32_t scan_code = wait_key(20000);
    if (scan_code == KEY_SCAN_CODE_SPACE) {
      rc = 0;
      break;
    } else if (scan_code == KEY_SCAN_CODE_ESC) {
      rc = 1;
      break;
    }

  }

exit:
  return rc;
}

// ending
int32_t game_ending_event(GAME_HANDLE* game) {

  WAIT_VSYNC;
  WAIT_VBLANK;

  // hide bar
  sprite_hide(game->sprite, SPID_BAR, 0);

  // hide ball
  sprite_hide(game->sprite, SPID_BALL, 0);

  // hide blocks
  for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {  
    sprite_hide(game->sprite, SPID_BLOCK, i);
  }

  SCREEN_HANDLE* scr = game->scr;
  SPRITE* sp_stars = sprite_get(game->sprite, SPID_STAR, 0);

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
  
  adpcm_play_music(game->adpcm, 1);

  int scan_code = 0;

  for (;;) {

    if (B_KEYSNS() != 0) {
      scan_code = B_KEYINP() >> 8;
      if (scan_code == KEY_SCAN_CODE_SPACE || scan_code == KEY_SCAN_CODE_CR ||
          scan_code == KEY_SCAN_CODE_ENTER || scan_code == KEY_SCAN_CODE_ESC) break;
    }

    WAIT_VSYNC;
    WAIT_VBLANK;

    for (int16_t i = 0; i < NUM_OF_STARS; i++) {
      sprite_scroll(game->sprite, SPID_STAR, i);  
    }

    // move stars in z-axis
    for (int16_t i = 0; i < NUM_OF_STARS; i++) {

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
  for (int16_t i = 0; i < NUM_OF_STARS; i++) {
    sprite_hide(game->sprite, SPID_STAR, i);
  }

  return scan_code;
}

// game over
int32_t game_over_event(GAME_HANDLE* game) {
  
  SCREEN_HANDLE* scr = game->scr;
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "GAME OVER");
  
  int32_t scan_code = wait_key(5000);

  sprite_hide(game->sprite, SPID_BAR, 0);
  sprite_hide(game->sprite, SPID_BALL, 0);
  for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {  
    sprite_hide(game->sprite, SPID_BLOCK, i);
  }

  return scan_code;
}

// round start
void game_round_start_event(GAME_HANDLE* game) {

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // hide ball
  sprite_hide(game->sprite, SPID_BALL, 0);

  // reset bar position
  SPRITE* sp_bar = sprite_get(game->sprite, SPID_BAR, 0);
  sp_bar->pos_x = scr->panel_game_x + ( scr->panel_game_width - 64 ) / 2;
  sp_bar->pos_y = scr->panel_game_y + scr->panel_game_height * 90 / 100;
  sp_bar->pos_x2 = 4;     // velocity
  sp_bar->priority = 3;
  sp_bar->invalidate = 0;

  // reset ball position
  SPRITE* sp_ball = sprite_get(game->sprite, SPID_BALL, 0);
  sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width / 2;
  sp_ball->pos_y = scr->panel_game_y + scr->panel_game_height * 2 / 3;
  sp_ball->pos_x2 = 1;    // velocity
  sp_ball->pos_y2 = -1;   // velocity
  sp_ball->priority = 3;
  sp_ball->invalidate = 0;

  // reset blocks
  for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = sprite_get(game->sprite, SPID_BLOCK, i);
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
  static uint8_t score[] = "             ";
  sprintf(score, "%10d", game->score);
  screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);

  // show round number
  static uint8_t round[] = "          ";
  sprintf(round, "ROUND %d", game->round);
  screen_put_text(scr, scr->panel_score_x + 8, 200, 1, round);

  // show bar and blocks
  sprite_show(game->sprite, SPID_BAR, 0);
  for (int i = 0; i < NUM_OF_BLOCKS; i++) {
    sprite_show(game->sprite, SPID_BLOCK, i);
  }

  // message
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "READY");
  wait_time(1500);

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "     ");

  // show ball
  sprite_show(game->sprite, SPID_BALL, 0);
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
  wait_time(3000);

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;

  // delete message;
  screen_put_text_center(scr, 0, 150, scr->panel_game_width, 1, "           ");
}

//  vsync interrupt handler
volatile static int vsync_counter = 0;
static GAME_HANDLE* vsync_game;
static SPRITE* vsync_sp_bar = NULL;
static SPRITE* vsync_sp_ball = NULL;
static SPRITE* vsync_sp_blocks = NULL;

static void __attribute__((interrupt)) game_round_vsync_interrupt() {

  if (vsync_sp_bar->invalidate) {
    sprite_direct_scroll(vsync_sp_bar);
    vsync_sp_bar->invalidate = 0;
  }
  if (vsync_sp_ball->invalidate) {
    sprite_direct_scroll(vsync_sp_ball);
    vsync_sp_ball->invalidate = 0;
  }
  for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {
    SPRITE* sp_block = &(vsync_sp_blocks[i]);
    if (sp_block->invalidate) {
      sprite_direct_scroll(sp_block);
      sp_block->invalidate = 0;
    }
  }
  if (vsync_game->score_invalidate) {
    SCREEN_HANDLE* scr = vsync_game->scr;
    static uint8_t score[] = "             ";
    sprintf(score, "%10d", vsync_game->score);
    screen_put_text(scr, scr->panel_score_x + 8,  50, 1, score);
    vsync_game->score_invalidate = 0;
  }

  vsync_counter = (vsync_counter + 1) & 0xfffffff;
}

// game round loop
int32_t game_round_loop(GAME_HANDLE* game) {

  // return code
  int32_t rc = 0;     // 0: clear  1: esc  2: game over  -1:fatal error

  // screen handle
  SCREEN_HANDLE* scr = game->scr;

  // sprite handle
  SPRITE_HANDLE* sprite = game->sprite;
  SPRITE* sp_bar = sprite_get(sprite, SPID_BAR, 0);
  SPRITE* sp_ball = sprite_get(sprite, SPID_BALL, 0);
  SPRITE* sp_blocks = sprite_get(sprite, SPID_BLOCK, 0);
  vsync_sp_bar = sp_bar;
  vsync_sp_ball = sp_ball;
  vsync_sp_blocks = sp_blocks;

  // adpcm set
  ADPCM_HANDLE* adpcm = game->adpcm;

  // number of blocks remained
  int16_t remained_blocks = NUM_OF_BLOCKS;

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
    int16_t vsync_counter_current = vsync_counter;
    while (vsync_counter == vsync_counter_current) {
      ;
    }

    // no more blocks?
    if (remained_blocks <= 0) break;

    // joy stick check
    int32_t joy = JOYGET(0);
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
      SPRITE* sp_blocks = sprite_get(sprite, SPID_BLOCK, 8);
      for (int16_t i = 0; i < 8; i++) {
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
      SPRITE* sp_blocks = sprite_get(sprite, SPID_BLOCK, 0);
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
      //sp_ball->pos_x += 2 * ( scr->panel_game_x - sp_ball->pos_x -5 );
      sp_ball->pos_x = scr->panel_game_x - 5;
      sp_ball->invalidate = 1;
      adpcm_play_se(adpcm, 4);
    }
    if ((sp_ball->pos_x + 11) > scr->panel_game_x + scr->panel_game_width) {    // right wall
      sp_ball->pos_x2 = -(1 + rand() % 4);
      //sp_ball->pos_x -= 2 * ( sp_ball->pos_x +11 - scr->panel_game_x - scr->panel_game_width );
      sp_ball->pos_x = scr->panel_game_x + scr->panel_game_width - 11;
      sp_ball->invalidate = 1;
      adpcm_play_se(adpcm, 4);
    }
    if ((sp_ball->pos_y +3) < scr->panel_game_y) {    // top wall
      sp_ball->pos_y2 = 1 + rand() % 4;
      //sp_ball->pos_y += 2 * ( scr->panel_game_y - sp_ball->pos_y -3 );
      sp_ball->pos_y = scr->panel_game_y - 3;
      sp_ball->invalidate = 1;
      adpcm_play_se(adpcm, 4);
    }

    // collision check (ball and bar)
    if (sp_ball->pos_x +11 >= sp_bar->pos_x && sp_ball->pos_x +5 <= sp_bar->pos_x +63 &&
        sp_ball->pos_y +10 >= sp_bar->pos_y && sp_ball->pos_y +10 <= sp_bar->pos_y +14 ) {
          sp_ball->pos_y2 = -(1 + rand() % 4);
          //sp_ball->pos_y -= 2*(sp_ball->pos_y +10 - sp_bar->pos_y);   // adjust overflown position
          sp_ball->pos_y = sp_bar->pos_y - 10;
          sp_ball->invalidate = 1;
          adpcm_play_se(adpcm, 2);
    }

    // collision check (ball and block)
    for (int16_t i = 0; i < NUM_OF_BLOCKS; i++) {

      SPRITE* sp_block = sprite_get(sprite, SPID_BLOCK, i);

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
          adpcm_play_se(adpcm, 5);
        } else {
          adpcm_play_se(adpcm, 3);
        }
      }

    }

    // ball out check
    if (sp_ball->pos_y >= scr->panel_game_y + scr->panel_game_height) {
      adpcm_play_se(adpcm, 1);
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
