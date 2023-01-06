#ifndef __H_SPRITE__
#define __H_SPRITE__

// sprite pattern class
typedef struct {
  int pattern_id;
  int size_x;
  int size_y;
  int palette_block;
  unsigned short* pattern_data;
  unsigned short* palette_data;
} SPRITE_PATTERN;

// sprite class
typedef struct {
  int sprite_id;
  int pos_x;
  int pos_y;
  int pos_z;      // optional
  int pos_x2;     // optional
  int pos_y2;     // optional
  int pos_z2;     // optional
  int priority;
  SPRITE_PATTERN* spp;
} SPRITE;

// prototype declarations
void sp_setup_pattern(SPRITE_PATTERN* spp);
void sp_scroll(SPRITE* sp);

#endif