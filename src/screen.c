
#include <stdint.h>
#include <string.h>
#include <iocslib.h>
#include <doslib.h>
#include "screen.h"
#include "crtc.h"

// clear and initialize screen and sprites
void screen_open(SCREEN_HANDLE* scr) {

  CRTMOD(12);
  G_CLR_ON();

  scr->original_fnk_mode = C_FNKMOD(-1);
  C_FNKMOD(3);
  B_CUROFF();

  crtc_set_mode(SCREEN_MODE_384x256);

  SP_INIT();
  for (int16_t i = 0; i < 256; i++) {
    SP_CGCLR(i);
  }
  SP_ON();

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

  scr->original_tpalette[0] = TPALET2(1,-1);
  scr->original_tpalette[1] = TPALET2(2,-1);
  scr->original_tpalette[2] = TPALET2(4,-1);          
    
  TPALET2(1, scr->text_colors[0]);
  TPALET2(2, scr->text_colors[1]);
  TPALET2(4, scr->text_colors[2]);

  for (int16_t i = 0; i < 256; i++) {
    scr->font_data_8x8[i].xl = 8;
    scr->font_data_8x8[i].yl = 8;
    memcpy(scr->font_data_8x8[i].buffer, FONT_ADDR_8x8 + FONT_BYTES_8x8 * i, FONT_BYTES_8x8);
    for (int16_t j = 0; j < FONT_BYTES_8x8; j++) {
      scr->font_data_8x8[i].buffer[j] |= scr->font_data_8x8[i].buffer[j] >> 1;
    }
  }
}

// close screen
void screen_close(SCREEN_HANDLE* scr) {

  // resume screen
  screen_clear_panel_text(scr,0);
  screen_clear_panel_text(scr,1);

  crtc_set_mode(SCREEN_MODE_768x512);

  CRTMOD(16);
  G_CLR_ON();

  C_FNKMOD(scr->original_fnk_mode);
  B_CURON();

  TPALET2(1, scr->original_tpalette[0]);
  TPALET2(2, scr->original_tpalette[1]);
  TPALET2(4, scr->original_tpalette[2]);      
}

// fill panel (graphic)
void screen_fill_panel(SCREEN_HANDLE* scr, int16_t panel, int16_t color) {
  if (panel == 0) {
    // game panel
    struct FILLPTR fill = {
      scr->panel_game_x,
      scr->panel_game_y,
      scr->panel_game_x + scr->panel_game_width - 1,
      scr->panel_game_y + scr->panel_game_height*2 - 1,     // for scroll area consideration
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

// fill panel (text)
void screen_clear_panel_text(SCREEN_HANDLE* scr, int16_t panel) {
  if (panel == 0) {
    // game panel
    for (int16_t i = 0; i < 4; i++) {
      struct TXFILLPTR fill = {
        i,
        scr->panel_game_x,
        scr->panel_game_y,
        scr->panel_game_x + scr->panel_game_width - 1,
        scr->panel_game_y + scr->panel_game_height*2 - 1,   // for scroll area consideration
        0};
      TXFILL(&fill);
    }

  } else if (panel == 1) {
    // score panel
    for (int16_t i = 0; i < 4; i++) {
      struct TXFILLPTR fill = {
        i,
        scr->panel_score_x,
        scr->panel_score_y,
        scr->panel_score_x + scr->panel_score_width - 1,
        scr->panel_score_y + scr->panel_score_height - 1,
        0};
      TXFILL(&fill);
    }

  }
}

// put text in 8x8 bold font
void screen_put_text(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t color, uint8_t* text) {
  int16_t len = strlen(text);
  TCOLOR(color);
  for (int16_t i = 0; i < len; i++) { 
    TEXTPUT(x + 8*i, y, &scr->font_data_8x8[text[i]]);
  }
}

// put text in 8x8 bold font with centering
void screen_put_text_center(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t width, int16_t color, uint8_t* text) {
  int16_t len = strlen(text);
  int16_t cx = x + ((width - 8 * len)>>1);
  screen_put_text(scr,cx,y,color,text);
}

// scroll screen (graphic)
void screen_scroll(SCREEN_HANDLE* scr, int16_t x, int16_t y) {
  SCROLL(0, x % scr->total_width, y % scr->total_height);
  SCROLL(1, x % scr->total_width, y % scr->total_height);
  SCROLL(2, x % scr->total_width, y % scr->total_height);
  SCROLL(3, x % scr->total_width, y % scr->total_height);
}

// put image dump dat to gvram
void screen_put_image(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t* image_data) {

  uint16_t* p = image_data;

  for (int16_t i = 0; i < height; i++) {
    volatile uint16_t* gvram = REG_GVRAM + ( y + i ) * 512 + x;
    for (int16_t j = 0; j < width; j++) {
      gvram[ j ] = *p++;
    }
  }
}
