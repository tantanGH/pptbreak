#include "crtc.h"
#include "screen.h"
#include "keyboard.h"
#include "sprite.h"
#include "adpcm.h"
#include "data_logo.h"
//#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iocslib.h>
#include <doslib.h>

#define VERSION "0.2.0 (2023/04/21)"

// main
int main(int argc, char* argv[]) {

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

/*
  // initialize game object
  static GAME_HANDLE game = { 0 };
  game_open(&game, &scr, &sp_set, &adpcm_set, logo_data);

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
*/
exit:
  // game close
//  game_close(&game);

  // adpcm close
  adpcm_close(&adpcm);

  // sprite close
  sprite_close(&sprite);

  // resume screen
  screen_close(&scr);

  return 0;
}