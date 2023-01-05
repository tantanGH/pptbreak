#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iocslib.h>
#include <doslib.h>

#include "crtc.h"
#include "sp_pat_blocks.h"

// screen object
typedef struct {

  // total screen size
  int total_width;
  int total_height;

  // game panel
  int panel_game_width;
  int panel_game_height;
  int panel_game_x;
  int panel_game_y;

  // score panel
  int panel_score_width;
  int panel_score_height;
  int panel_score_x;
  int panel_score_y;

  // panel colors
  int panel_colors[ 4 ];

  // text colors
  int text_colors[ 4 ];

  // original text palettes
  unsigned short original_tpalette[ 4 ];

  // 8x8 bold font data
  struct FNTBUF font_data_8x8[ 256 ];

} SCREEN_HANDLE;

// sprite object
typedef struct {
  int id;
  int pos_x;
  int pos_y;
  int pattern;
  int palette;
  int priority;
  int size_x;
  int size_y;
} SPRITE;

// game object
typedef struct {
  int high_score;
  int score;
  int round;
} GAME;

// clear and initialize screen and sprites
static void screen_init(SCREEN_HANDLE* scr) {

    CRTMOD(12);
    B_CUROFF();
    G_CLR_ON();

    crtc_set_mode(SCREEN_MODE_384x256);

    SP_INIT();
    for (int i = 0; i < 256; i++) {
        SP_CGCLR(i);
    }
    SP_ON();

    scr->original_tpalette[0] = TPALET2(1,-1);
    scr->original_tpalette[2] = TPALET2(2,-1);
    scr->original_tpalette[3] = TPALET2(4,-1);
    scr->original_tpalette[4] = TPALET2(8,-1);            
    
    TPALET2(1, scr->text_colors[0]);
    TPALET2(2, scr->text_colors[1]);
    TPALET2(3, scr->text_colors[2]);
    TPALET2(4, scr->text_colors[3]);
}

// reset screen
static void screen_reset(SCREEN_HANDLE* scr) {

    crtc_set_mode(SCREEN_MODE_768x512);

    CRTMOD(16);
    G_CLR_ON();    
    B_CURON();

    TPALET2(1, scr->original_tpalette[0]);
    TPALET2(2, scr->original_tpalette[1]);
    TPALET2(4, scr->original_tpalette[2]);
    TPALET2(8, scr->original_tpalette[3]);        
}

// fill background
static void screen_fill_panel(SCREEN_HANDLE* scr, int color, int panel) {
  if (panel == 0) {
    // game panel
    struct FILLPTR fill = {
      scr->panel_game_x,
      scr->panel_game_y,
      scr->panel_game_x + scr->panel_game_width - 1,
      scr->panel_game_y + scr->panel_game_height - 1,
      scr->panel_colors[color]};
    FILL(&fill);
  } else if (panel == 1) {
    // score panel
    struct FILLPTR fill = {
      scr->panel_score_x,
      scr->panel_score_y,
      scr->panel_score_x + scr->panel_score_width - 1,
      scr->panel_score_y + scr->panel_score_height - 1,
      scr->panel_colors[color]};
    FILL(&fill);
  }
}

// create 8x8 bold fonts
static void screen_init_font(SCREEN_HANDLE* scr) {
  for (int i = 0; i < 256; i++) {
    scr->font_data_8x8[i].xl = 8;
    scr->font_data_8x8[i].yl = 8;
    memcpy(scr->font_data_8x8[i].buffer, FONT_ADDR_8x8 + FONT_BYTES_8x8 * i, FONT_BYTES_8x8);
    for (int j = 0; j < FONT_BYTES_8x8; j++) {
      scr->font_data_8x8[i].buffer[j] |= scr->font_data_8x8[i].buffer[j] >> 1;
    }
  }
}

// put text in 8x8 bold font
static void screen_put_text(SCREEN_HANDLE* scr, int x, int y, int color, const char* text) {
  int len = strlen(text);
  TCOLOR(color);
  for (int i = 0; i < len; i++) { 
    TEXTPUT(x + 8*i, y, &scr->font_data_8x8[text[i]]);
  }
}

// set sprite palette
static void sp_setup_palette(int palette_block, unsigned short* palette_data) {
    for (int i = 0; i < 16; i++) {
        SP_PALETTE_REG[ palette_block * 16 + i ] = palette_data[i];
    }
}

// set sprite patterns
static void sp_setup_pattern(int pattern_number, unsigned short* pattern_data, int pattern_count) {
    for (int i = 0; i < pattern_count; i++) {
        for (int j = 0; j < 0x40; j++) {
            PCG_DATA_REG[ ( pattern_number + i)  * 0x40 + j ] = pattern_data[ i * 0x40 + j];
        }
    }
}

// set basic sprite pattern
static void sp_setup_pattern_basic(int pattern_number, unsigned char* pattern_array) {
    unsigned char pattern_data[ 0x80 ];
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 4; x++) {
            pattern_data[ 0x00 + y*4 + x ] = ( pattern_array[ (y+0)*16 + (x+0)*2 ] << 4 ) | ( pattern_array[ (y+0)*16 + (x+0)*2 + 1]);
            pattern_data[ 0x20 + y*4 + x ] = ( pattern_array[ (y+8)*16 + (x+0)*2 ] << 4 ) | ( pattern_array[ (y+8)*16 + (x+0)*2 + 1]);
            pattern_data[ 0x40 + y*4 + x ] = ( pattern_array[ (y+0)*16 + (x+4)*2 ] << 4 ) | ( pattern_array[ (y+0)*16 + (x+4)*2 + 1]);
            pattern_data[ 0x60 + y*4 + x ] = ( pattern_array[ (y+8)*16 + (x+4)*2 ] << 4 ) | ( pattern_array[ (y+8)*16 + (x+4)*2 + 1]);
        }
    }
    sp_setup_pattern(pattern_number, (unsigned short*)pattern_data, 1);
}

// scroll sprite
inline static void sp_scroll(SPRITE* sp, int offset) {
    int ofs = sp->id * 4;
    int pattern = sp->pattern;
    for (int y = 0; y < sp->size_y; y++) {
      for (int x = 0; x < sp->size_x; x++) {
        SP_SCROLL_REG[ofs++] = 16 * offset + sp->pos_x + x*16;
        SP_SCROLL_REG[ofs++] = 16 * offset + sp->pos_y + y*16;
        SP_SCROLL_REG[ofs++] = (sp->palette << 8) | pattern++;
        SP_SCROLL_REG[ofs++] = sp->priority;
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

  // screen object
  static SCREEN_HANDLE scr = { 0 };

  // initialize screen
  scr.total_width = 384;
  scr.total_height = 256;

  scr.panel_game_width = 288;
  scr.panel_game_height = 256;
  scr.panel_game_x = 0;
  scr.panel_game_y = 0;

  scr.panel_score_width = 96;
  scr.panel_score_height = 256;
  scr.panel_score_x = 288;
  scr.panel_score_y = 0;

  scr.panel_colors[0] = 0x18e1;
  scr.panel_colors[1] = 0x794b;
  scr.panel_colors[2] = 0x515f;  
  scr.panel_colors[3] = 0x248b;  

  scr.text_colors[0] = 0xffff;
  scr.text_colors[1] = 0x07c1;
  scr.text_colors[2] = 0xffc1;
  scr.text_colors[3] = 0xf83f;
        
  screen_init(&scr);
  screen_fill_panel(&scr, 0, 0);

  // initialize 8x8 bold font
  screen_init_font(&scr);

  // initialize score panel
  screen_put_text(&scr, scr.panel_score_x + 8,  8, 2, "HIGH SCORE");
  screen_put_text(&scr, scr.panel_score_x + 8, 20, 1, "     76500");
  screen_put_text(&scr, scr.panel_score_x + 8, 40, 2, "1UP");
  screen_put_text(&scr, scr.panel_score_x + 8, 50, 1, "         0");
  screen_put_text(&scr, scr.panel_score_x + 8, 200, 1, "ROUND 1");

  // initialize sprite palette
  sp_setup_palette(1, sp_palette_data_bar1);
  sp_setup_palette(2, sp_palette_data_ball1);
  sp_setup_palette(3, sp_palette_data_ball2);
  sp_setup_palette(4, sp_palette_data_wall1);
  sp_setup_palette(5, sp_palette_data_block2);
  sp_setup_palette(6, sp_palette_data_block1);
  sp_setup_palette(7, sp_palette_data_block3);
  sp_setup_palette(8, sp_palette_data_block4);

  // initialize sprite pattern
  sp_setup_pattern(0, sp_pattern_data_bar1, 4);     // 0 - 3
  sp_setup_pattern(4, sp_pattern_data_ball1, 1);    // 4
  sp_setup_pattern(5, sp_pattern_data_ball2, 1);    // 5
  sp_setup_pattern(6, sp_pattern_data_wall1, 1);    // 6
  sp_setup_pattern(7, sp_pattern_data_block2, 2);   // 7
  sp_setup_pattern(9, sp_pattern_data_block1, 2);   // 9
  sp_setup_pattern(11, sp_pattern_data_block3, 2);  // 11
  sp_setup_pattern(13, sp_pattern_data_block4, 2);  // 13       

  // initialize sprite objects
  SPRITE sp_ball;
  SPRITE sp_bar;
  SPRITE sp_blocks[60];

  sp_bar.id = 0;
  sp_bar.pos_x = scr.panel_game_x + ( scr.panel_game_width - 64 ) / 2;
  sp_bar.pos_y = scr.panel_game_y + scr.panel_game_height * 90 / 100;
  sp_bar.pattern = 0;
  sp_bar.palette = 1;
  sp_bar.priority = 3;
  sp_bar.size_x = 4;
  sp_bar.size_y = 1;

  sp_ball.id = 4;
  sp_ball.pos_x = scr.panel_game_x + scr.panel_game_width / 2;
  sp_ball.pos_y = scr.panel_game_y + scr.panel_game_height * 2 / 3;
  sp_ball.pattern = 4;
  sp_ball.palette = 2;
  sp_ball.priority = 3;
  sp_ball.size_x = 1;
  sp_ball.size_y = 1;

  for (int i = 0; i < 48; i++) {
    sp_blocks[i].id = 5 + i * 2;
    sp_blocks[i].pos_x = 16 + (i % 8) * 32;
    sp_blocks[i].pos_y = 16 + (i / 8) * 16;
    sp_blocks[i].pattern = 9 + (i / 16) * 2;
    sp_blocks[i].palette = 6 + (i / 16);
    sp_blocks[i].priority = 3;
    sp_blocks[i].size_x = 2;
    sp_blocks[i].size_y = 1;
  }

  int ball_ax = 1;
  int ball_ay = -1;
  int rc = 0;

  srand((unsigned int)time(NULL));

  // ready screen  
  WAIT_VSYNC;
  WAIT_VBLANK;

  sp_scroll(&sp_bar, 1);
  for (int i = 0; i < 48; i++) {
    sp_scroll(&sp_blocks[i], 1);
  }

  screen_put_text(&scr, (scr.panel_game_width-8*5)/2, 150, 1, "READY");
  wait_seconds(3);
  screen_put_text(&scr, (scr.panel_game_width-8*5)/2, 150, 1, "     ");

  sp_scroll(&sp_ball, 1);

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
    sp_scroll(&sp_bar, 1);
    sp_scroll(&sp_ball, 1);

    if (BITSNS(0) & 0x02) {   // ESC
      break;
    }
  }

  if (rc != 0) {
    screen_put_text(&scr, scr.panel_game_x + (scr.panel_game_width-8*9)/2, 150, 1, "GAME OVER");
  }

  for (;;) {

    WAIT_VBLANK;

    if (BITSNS(0) & 0x02) {     // ESC
      break;
    }
  }

  screen_reset(&scr);
  K_KEYSNS();
}