#include "crtc.h"
#include "screen.h"
#include "keyboard.h"
#include "sprite.h"
#include "adpcm.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iocslib.h>
#include <doslib.h>

#define VERSION "0.2.0 (2023/04/21)"

#define REG_TACR         ((volatile uint8_t*)0xE88019)     // timer-A control register
#define REG_TADR         ((volatile uint8_t*)0xE8801F)     // timer-A data register

static volatile int16_t g_vsync_available = 0;

// vsync init
static void __attribute__((interrupt)) vsync_interrupt() {
  g_vsync_available = 1;
}

// main
int32_t main(int argc, char* argv[]) {

  // version check
  if (argc >= 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-v") == 0)) {
    printf("pptbreak.x version " VERSION " 2023 by tantan\n");
    return 1;
  }

  // randomaize
  srand((unsigned int)time(NULL));

  // enter supervisor mode
  B_SUPER(0);

  // initialize screen,font and panels
  static SCREEN_HANDLE scr = { 0 };
  screen_open(&scr);
  
  // define sprite patterns and palettes
  static SPRITE_HANDLE sprite = { 0 };
  sprite_open(&sprite, &scr);

  // initialize adpcm sound objects
  static ADPCM_HANDLE adpcm = { 0 };
  adpcm_open(&adpcm);

  // initialize game object
  static GAME_HANDLE game = { 0 };
  game_open(&game, &scr, &sprite, &adpcm);

  // wait for vsync interrupt readiness
  REG_TADR[0] = 2;      // reset Timer-A counter
  REG_TACR[0] = 8;      // start Timer-A in event count mode
  g_vsync_available = 0;
  if (VDISPST((unsigned char*)vsync_interrupt, 0, 1) != 0) {   // VBLANK, 1/60
    printf("error: VSYNC interrupt is being used by other applications.\n");
    return -1;
  }
  while (g_vsync_available == 0);
  VDISPST(0, 0, 0);

  // game loop
  for (;;) {

    // game opening
    if (game_opening_event(&game) != 0) goto exit;

    // reset game
    game_reset(&game);

    int rc = 0;
    do {

      // round start
      game_round_start_event(&game);

      // round loop
      rc = game_round_loop(&game);
    
      switch (rc) {

        case 0:  
          // round clear
          game_round_clear_event(&game);
          game.round++;
          break;

        case 1:
          // esc key
          goto exit;
      
        case 2:
          // game over
          game_over_event(&game);
          break;

        case -1:
          // fatal error
          goto exit;
      }
 
      if (game.round > 4) {
        // true end
        game_ending_event(&game);
        break;
      }

    } while (rc == 0);

  }

exit:
  // game close
  game_close(&game);

  // adpcm close
  adpcm_close(&adpcm);

  // sprite close
  sprite_close(&sprite);

  // resume screen
  screen_close(&scr);

  return 0;
}