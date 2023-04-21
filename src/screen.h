#ifndef __H_SCREEN__
#define __H_SCREEN__

#include <stdint.h>
#include <iocslib.h>

#define FONT_ADDR_8x8    ((uint8_t*)0xF3A000)
#define FONT_ADDR_8x16   ((uint8_t*)0xF3A800)
#define FONT_ADDR_12x12  ((uint8_t*)0xF3B800)
#define FONT_ADDR_12x24  ((uint8_t*)0xF3D000)

#define FONT_BYTES_8x8   (8)
#define FONT_BYTES_8x16  (16)
#define FONT_BYTES_12x12 (24)
#define FONT_BYTES_12x24 (48)

// screen class
typedef struct {

  // total screen size
  int16_t total_width;
  int16_t total_height;

  // game panel
  int16_t panel_game_width;
  int16_t panel_game_height;
  int16_t panel_game_depth;
  int16_t panel_game_x;
  int16_t panel_game_y;

  // score panel
  int16_t panel_score_width;
  int16_t panel_score_height;
  int16_t panel_score_x;
  int16_t panel_score_y;

  // panel colors
  uint16_t panel_colors[ 5 ];

  // text colors
  uint16_t text_colors[ 4 ];

  // original text palettes
  uint16_t original_tpalette[ 3 ];

  // original function key display mode
  int16_t original_fnk_mode;

  // 8x8 bold font data
  struct FNTBUF font_data_8x8[ 256 ];

} SCREEN_HANDLE;

// prototype declarations
void screen_open(SCREEN_HANDLE* scr);
void screen_close(SCREEN_HANDLE* scr);
void screen_fill_panel(SCREEN_HANDLE* scr, int16_t panel, int16_t color);
void screen_clear_panel_text(SCREEN_HANDLE* scr, int16_t panel);
void screen_init_font(SCREEN_HANDLE* scr);
void screen_put_text(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t color, uint8_t* text);
void screen_put_text_center(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t width, int16_t color, uint8_t* text);
void screen_scroll(SCREEN_HANDLE* scr, int16_t x, int16_t y);
void screen_put_image(SCREEN_HANDLE* scr, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t* image_data);

#endif