#include <iocslib.h>
#include <string.h>

#include "screen.h"
#include "crtc.h"

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

// fill panel
void screen_fill_panel(SCREEN_HANDLE* scr, int color, int panel) {
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
