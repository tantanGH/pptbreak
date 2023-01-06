#include <iocslib.h>
#include <string.h>

#include "screen.h"
#include "crtc.h"

#ifdef USE_PNG
#include "ext/png.h"
#endif

// clear and initialize screen and sprites
void screen_init(SCREEN_HANDLE* scr) {

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
  scr->original_tpalette[1] = TPALET2(2,-1);
  scr->original_tpalette[2] = TPALET2(4,-1);          
    
  TPALET2(1, scr->text_colors[0]);
  TPALET2(2, scr->text_colors[1]);
  TPALET2(4, scr->text_colors[2]);
}

// reset screen
void screen_reset(SCREEN_HANDLE* scr) {

  crtc_set_mode(SCREEN_MODE_768x512);

  CRTMOD(16);
  G_CLR_ON();    
  B_CURON();

  TPALET2(1, scr->original_tpalette[0]);
  TPALET2(2, scr->original_tpalette[1]);
  TPALET2(4, scr->original_tpalette[2]);      
}

// fill panel (graphic)
void screen_fill_panel(SCREEN_HANDLE* scr, int panel, int color) {
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
void screen_clear_panel_text(SCREEN_HANDLE* scr, int panel) {
  if (panel == 0) {

    // game panel
    for (int i = 0; i < 4; i++) {
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
    for (int i = 0; i < 4; i++) {
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

// create 8x8 bold fonts
void screen_init_font(SCREEN_HANDLE* scr) {
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
void screen_put_text(SCREEN_HANDLE* scr, int x, int y, int color, const char* text) {
  int len = strlen(text);
  TCOLOR(color);
  for (int i = 0; i < len; i++) { 
    TEXTPUT(x + 8*i, y, &scr->font_data_8x8[text[i]]);
  }
}

// put text in 8x8 bold font with centering
void screen_put_text_center(SCREEN_HANDLE* scr, int x, int y, int width, int color, const char* text) {
  int len = strlen(text);
  int cx = x + ((width - 8 * len)>>1);
  screen_put_text(scr,cx,y,color,text);
}

// scroll screen (graphic)
void screen_scroll(SCREEN_HANDLE* scr, int x, int y) {
  SCROLL(0, x % scr->total_width, y % scr->total_height);
  SCROLL(1, x % scr->total_width, y % scr->total_height);
  SCROLL(2, x % scr->total_width, y % scr->total_height);
  SCROLL(3, x % scr->total_width, y % scr->total_height);
}

// put image dump dat to gvram
void screen_put_image(SCREEN_HANDLE* scr, int x, int y, int width, int height, unsigned short* image_data) {

  unsigned short* p = image_data;

  for (int i = 0; i < height; i++) {
    volatile unsigned short* gvram = GVRAM + ( y + i ) * 512 + x;
    for (int j = 0; j < width; j++) {
      gvram[ j ] = *p++;
    }
  }
}

#ifdef USE_PNG

// load png image
void screen_load_png(SCREEN_HANDLE* scr, int x, int y, int brightness, const char* png_file_name) {

  static PNG_DECODE_HANDLE png;

  png.input_buffer_size = 65536;
  png.output_buffer_size = 65536 * 2;
  png.use_high_memory = 0;
  png.brightness = brightness;
  png.offset_x = x;
  png.offset_y = y;

  png_init(&png);
  png_load(&png, png_file_name);
  png_close(&png);
}

#endif