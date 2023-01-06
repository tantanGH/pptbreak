#ifndef __H_SCREEN__
#define __H_SCREEN__

#define FONT_ADDR_8x8    ((unsigned char*)0xF3A000)
#define FONT_ADDR_8x16   ((unsigned char*)0xF3A800)
#define FONT_ADDR_12x12  ((unsigned char*)0xF3B800)
#define FONT_ADDR_12x24  ((unsigned char*)0xF3D000)

#define FONT_BYTES_8x8   (8)
#define FONT_BYTES_8x16  (16)
#define FONT_BYTES_12x12 (24)
#define FONT_BYTES_12x24 (48)

// screen class
typedef struct {

  // total screen size
  int total_width;
  int total_height;

  // game panel
  int panel_game_width;
  int panel_game_height;
  int panel_game_depth;
  int panel_game_x;
  int panel_game_y;

  // score panel
  int panel_score_width;
  int panel_score_height;
  int panel_score_x;
  int panel_score_y;

  // panel colors
  int panel_colors[ 5 ];

  // text colors
  int text_colors[ 4 ];

  // original text palettes
  unsigned short original_tpalette[ 3 ];

  // 8x8 bold font data
  struct FNTBUF font_data_8x8[ 256 ];

} SCREEN_HANDLE;

// prototype declarations
void screen_init(SCREEN_HANDLE* scr);
void screen_reset(SCREEN_HANDLE* scr);
void screen_fill_panel(SCREEN_HANDLE* scr, int panel, int color);
void screen_clear_panel_text(SCREEN_HANDLE* scr, int panel);
void screen_init_font(SCREEN_HANDLE* scr);
void screen_put_text(SCREEN_HANDLE* scr, int x, int y, int color, const char* text);
void screen_put_text_center(SCREEN_HANDLE* scr, int x, int y, int width, int color, const char* text);
void screen_scroll(SCREEN_HANDLE* scr, int x, int y);
void screen_put_image(SCREEN_HANDLE* scr, int x, int y, int width, int height, unsigned short* image_data);
void screen_load_png(SCREEN_HANDLE* scr, int x, int y, int brightness, const char* png_file_name);

#endif