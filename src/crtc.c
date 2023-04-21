#include <stdint.h>
#include "crtc.h"

static void set_palette_65536() {

  volatile uint16_t* palette_ptr = REG_PALETTE;

  for (int32_t i = 0x0001; i <= 0x10000; i += 0x0202) {
    *palette_ptr++ = (uint16_t)i;
    *palette_ptr++ = (uint16_t)i;
  }
}

int32_t crtc_set_mode(int16_t mode) {

  // return code
  int32_t rc = -1;

  // wait vblank
  WAIT_VSYNC;
  WAIT_VBLANK;

  // change CRTC screen mode
  switch (mode) {
    
    case SCREEN_MODE_384x256: {

      // 512x256(384x256),31kHz,65536 colors
      uint16_t current_resolution = REG_CRTC_R20[0] & 0x013;
      if (current_resolution > 0x11) {

        REG_CRTC_R20[0] = 0x311;      // set first

        REG_CRTC_R00[1] = 0x0006;
        REG_CRTC_R00[2] = 0x000b;
        REG_CRTC_R00[3] = 0x003b; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 
        REG_CRTC_R00[8] = 0x001b;

        REG_CRTC_R00[0] = 0x0045;     // set last

      } else {

        REG_CRTC_R00[0] = 0x0045;     // set first

        REG_CRTC_R00[1] = 0x0006;
        REG_CRTC_R00[2] = 0x000b;
        REG_CRTC_R00[3] = 0x003b; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 
        REG_CRTC_R00[8] = 0x001b;

        REG_CRTC_R20[0] = 0x311;      // set last

      }

      SET_SYSP;                       // system port for dot clock change - Inside/Out p44

      REG_VDC_R0[0] = 3;              // memory mode 3

      REG_SCON[1] = 0x000b + 4;       // R02 + 4

      WAIT_SCON;
      REG_SCON[0] = 0xff;             // 256 color mode: R00  else: 0xff
      REG_SCON[2] = 0x28;             // R06
      REG_SCON[3] = 0x11;             // R20 & 0xff

      REG_VDC_R2[0] = 0x2f;           // text/graphic on, sprite off (w:512 h:512)

      REG_CRTC_R12[0] = 0;            // scroll position X
      REG_CRTC_R12[1] = 0;            // scroll position Y
      REG_CRTC_R12[2] = 0;            // scroll position X
      REG_CRTC_R12[3] = 0;            // scroll position Y    
      REG_CRTC_R12[4] = 0;            // scroll position X
      REG_CRTC_R12[5] = 0;            // scroll position Y
      REG_CRTC_R12[6] = 0;            // scroll position X
      REG_CRTC_R12[7] = 0;            // scroll position Y

      rc = 0;
      break;
    }

    case SCREEN_MODE_512x512: {

      // 512x512(512x512),31kHz,65536 colors
      uint16_t current_resolution = REG_CRTC_R20[0] & 0x013;
      if (current_resolution > 0x15) {

        REG_CRTC_R20[0] = 0x315;      // set last

        REG_CRTC_R00[1] = 0x0009;
        REG_CRTC_R00[2] = 0x0011;
        REG_CRTC_R00[3] = 0x0051; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 
        REG_CRTC_R00[8] = 0x001b;

        REG_CRTC_R00[0] = 0x005b;     // set last

      } else {

        REG_CRTC_R00[0] = 0x005b;     // set first

        REG_CRTC_R00[1] = 0x0009;
        REG_CRTC_R00[2] = 0x0011;
        REG_CRTC_R00[3] = 0x0051; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 

        REG_CRTC_R20[0] = 0x0315;     // set last, memory mode 3

      }

      RESET_SYSP;                     // system port for dot clock change - Inside/Out p44

      REG_VDC_R0[0] = 3;              // memory mode 3

//      REG_SCON[1] = 0x0011 + 4;     // R02 + 4

//      WAIT_SCON;
//      REG_SCON[0] = 0xff;           // 256 color: R00  else: 0xff
//      REG_SCON[2] = 0x28;           // R06
//      REG_SCON[3] = 0x15;           // R20 & 0xff

      REG_VDC_R2[0] = 0x2f;           // text/graphic on, sprite off (w:512 h:512)

      REG_CRTC_R12[0] = 0;            // scroll position X
      REG_CRTC_R12[1] = 0;            // scroll position Y
      REG_CRTC_R12[2] = 0;            // scroll position X
      REG_CRTC_R12[3] = 0;            // scroll position Y    
      REG_CRTC_R12[4] = 0;            // scroll position X
      REG_CRTC_R12[5] = 0;            // scroll position Y
      REG_CRTC_R12[6] = 0;            // scroll position X
      REG_CRTC_R12[7] = 0;            // scroll position Y
  
      rc = 0;
      break;
    }

    case SCREEN_MODE_768x512: {

      // 512x512(768x512),31kHz,65536 colros
      uint16_t current_resolution = REG_CRTC_R20[0] & 0x013;
      if (current_resolution > 0x16) {

        REG_CRTC_R20[0] = 0x316;      // set first

        REG_CRTC_R00[1] = 0x000e;
        REG_CRTC_R00[2] = 0x001c;
        REG_CRTC_R00[3] = 0x007c; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 
        REG_CRTC_R00[8] = 0x001b;

        REG_CRTC_R00[0] = 0x0089;     // set last

      } else {

        REG_CRTC_R00[0] = 0x0089;     // set first

        REG_CRTC_R00[1] = 0x000e;
        REG_CRTC_R00[2] = 0x001c;
        REG_CRTC_R00[3] = 0x007c; 
        REG_CRTC_R00[4] = 0x0237;
        REG_CRTC_R00[5] = 0x0005;
        REG_CRTC_R00[6] = 0x0028;
        REG_CRTC_R00[7] = 0x0228; 
        REG_CRTC_R00[8] = 0x001b;    

        REG_CRTC_R20[0] = 0x0316;     // set last, memory mode 3

      }

      RESET_SYSP;                     // system port for dot clock change - Inside/Out p44

      REG_VDC_R0[0] = 3;              // memory mode 3

//      REG_SCON[1] = 0x001c + 4;

//      WAIT_SCON;
//      REG_SCON[0] = 0xff;           // 256 color: R00  else: 0xff
//      REG_SCON[2] = 0x28;           // R06
//      REG_SCON[3] = 0x16;           // R20 & 0xff

      REG_VDC_R2[0] = 0x2f;           // text/graphic on, sprite off (w:512 h:512)

      REG_CRTC_R12[0] = 0;            // scroll position X
      REG_CRTC_R12[1] = 0;            // scroll position Y
      REG_CRTC_R12[2] = 0;            // scroll position X
      REG_CRTC_R12[3] = 0;            // scroll position Y    
      REG_CRTC_R12[4] = 0;            // scroll position X
      REG_CRTC_R12[5] = 0;            // scroll position Y
      REG_CRTC_R12[6] = 0;            // scroll position X
      REG_CRTC_R12[7] = 0;            // scroll position Y

      rc = 0;
      break;
    }

    case SCREEN_MODE_768x512_FULL: {

      // 1024x1024(768x512),31kHz,65536 colros (for XEiJ only)
      REG_CRTC_R00[0] = 0x0089;

      REG_CRTC_R00[1] = 0x000e;
      REG_CRTC_R00[2] = 0x001c;
      REG_CRTC_R00[3] = 0x007c; 
      REG_CRTC_R00[4] = 0x0237;
      REG_CRTC_R00[5] = 0x0005;
      REG_CRTC_R00[6] = 0x0028;
      REG_CRTC_R00[7] = 0x0228; 
      REG_CRTC_R00[8] = 0x001b;

      REG_CRTC_R20[0] = 0x0716;       // memory mode 7 (for XEiJ only)

      RESET_SYSP;                     // system port for dot clock change - Inside/Out p44

      REG_VDC_R0[0] = 7;              // memory mode 7 (for XEiJ only)
  
//      REG_SCON[1] = 0x001c + 4;     // R02 + 0x04

//      WAIT_SCON;
//      REG_SCON[0] = 0xff;           // 256 color: R00  else: 0xff
//      REG_SCON[2] = 0x28;           // R06
//      REG_SCON[3] = 0x16;           // R20 & 0xff

      REG_VDC_R2[0] = 0x30;           // text/graphic on (w:1024 h:1024)

      REG_CRTC_R12[0] = 0;            // scroll position X
      REG_CRTC_R12[1] = 0;            // scroll position Y

      rc = 0;
      break;
    }

    default:
      // unknown mode
      break;

  }

  set_palette_65536();

exit:
  return rc;
}
