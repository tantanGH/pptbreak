#include "crtc.h"

static void set_palette_65536() {

  volatile unsigned short* palette_ptr = PALETTE;

  for (int i = 0x0001; i <= 0x10000; i += 0x0202) {
    *palette_ptr++ = (unsigned short)i;
    *palette_ptr++ = (unsigned short)i;
  }
}

int crtc_set_mode(int mode) {

  // return code
  int rc = -1;

  // wait vblank
  WAIT_VSYNC;
  WAIT_VBLANK;

  // change CRTC screen mode
  switch (mode) {
    
    case SCREEN_MODE_384x256: {

      // 512x256(384x256),31kHz,65536 colors
      int current_resolution = CRTC_R20[0] & 0x013;
      if (current_resolution > 0x11) {

        CRTC_R20[0] = 0x311;    // set first

        CRTC_R00[1] = 0x0006;
        CRTC_R00[2] = 0x000b;
        CRTC_R00[3] = 0x003b; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 
        CRTC_R00[8] = 0x001b;

        CRTC_R00[0] = 0x0045;   // set last

      } else {

        CRTC_R00[0] = 0x0045;   // set first

        CRTC_R00[1] = 0x0006;
        CRTC_R00[2] = 0x000b;
        CRTC_R00[3] = 0x003b; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 
        CRTC_R00[8] = 0x001b;

        CRTC_R20[0] = 0x311;    // set last

      }

      SET_SYSP;                     // system port for dot clock change - Inside/Out p44

      VDC_R0[0] = 3;            // memory mode 3

      SCON[1] = 0x000b + 4;         // R02 + 4

      WAIT_SCON;
      SCON[0] = 0xff;               // 256 color mode: R00  else: 0xff
      SCON[2] = 0x28;               // R06
      SCON[3] = 0x11;               // R20 & 0xff

      VDC_R2[0] = 0x2f;         // test/graphic on, sprite off (w:512 h:512)

      CRTC_R12[0] = 0;          // scroll position X
      CRTC_R12[1] = 0;          // scroll position Y
      CRTC_R12[2] = 0;          // scroll position X
      CRTC_R12[3] = 0;          // scroll position Y    
      CRTC_R12[4] = 0;          // scroll position X
      CRTC_R12[5] = 0;          // scroll position Y
      CRTC_R12[6] = 0;          // scroll position X
      CRTC_R12[7] = 0;          // scroll position Y

      rc = 0;
      break;
    }

    case SCREEN_MODE_512x512: {

      // 512x512(512x512),31kHz,65536 colors
      int current_resolution = CRTC_R20[0] & 0x013;
      if (current_resolution > 0x15) {

        CRTC_R20[0] = 0x315;      // set last

        CRTC_R00[1] = 0x0009;
        CRTC_R00[2] = 0x0011;
        CRTC_R00[3] = 0x0051; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 
        CRTC_R00[8] = 0x001b;

        CRTC_R00[0] = 0x005b;     // set last

      } else {

        CRTC_R00[0] = 0x005b;     // set first

        CRTC_R00[1] = 0x0009;
        CRTC_R00[2] = 0x0011;
        CRTC_R00[3] = 0x0051; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 

        CRTC_R20[0] = 0x0315;     // set last, memory mode 3

      }

      RESET_SYSP;                     // system port for dot clock change - Inside/Out p44

      VDC_R0[0] = 3;              // memory mode 3

//      SCON[1] = 0x0011 + 4;           // R02 + 4

//      WAIT_SCON;
//      SCON[0] = 0xff;                 // 256 color: R00  else: 0xff
//      SCON[2] = 0x28;                 // R06
//      SCON[3] = 0x15;                 // R20 & 0xff

      VDC_R2[0] = 0x2f;           // text/graphic on, sprite off (w:512 h:512)

      CRTC_R12[0] = 0;            // scroll position X
      CRTC_R12[1] = 0;            // scroll position Y
      CRTC_R12[2] = 0;            // scroll position X
      CRTC_R12[3] = 0;            // scroll position Y    
      CRTC_R12[4] = 0;            // scroll position X
      CRTC_R12[5] = 0;            // scroll position Y
      CRTC_R12[6] = 0;            // scroll position X
      CRTC_R12[7] = 0;            // scroll position Y
  
      rc = 0;
      break;
    }

    case SCREEN_MODE_768x512: {

      // 512x512(768x512),31kHz,65536 colros
      int current_resolution = CRTC_R20[0] & 0x013;
      if (current_resolution > 0x16) {

        CRTC_R20[0] = 0x316;      // set first

        CRTC_R00[1] = 0x000e;
        CRTC_R00[2] = 0x001c;
        CRTC_R00[3] = 0x007c; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 
        CRTC_R00[8] = 0x001b;

        CRTC_R00[0] = 0x0089;     // set last

      } else {

        CRTC_R00[0] = 0x0089;     // set first

        CRTC_R00[1] = 0x000e;
        CRTC_R00[2] = 0x001c;
        CRTC_R00[3] = 0x007c; 
        CRTC_R00[4] = 0x0237;
        CRTC_R00[5] = 0x0005;
        CRTC_R00[6] = 0x0028;
        CRTC_R00[7] = 0x0228; 
        CRTC_R00[8] = 0x001b;    

        CRTC_R20[0] = 0x0316;     // set last, memory mode 3

      }

      RESET_SYSP;                 // system port for dot clock change - Inside/Out p44

      VDC_R0[0] = 3;              // memory mode 3

//      SCON[1] = 0x001c + 4;

//      WAIT_SCON;
//      SCON[0] = 0xff;                 // 256 color: R00  else: 0xff
//      SCON[2] = 0x28;                 // R06
//      SCON[3] = 0x16;                 // R20 & 0xff

      VDC_R2[0] = 0x2f;           // text/graphic on, sprite off (w:512 h:512)

      CRTC_R12[0] = 0;            // scroll position X
      CRTC_R12[1] = 0;            // scroll position Y
      CRTC_R12[2] = 0;            // scroll position X
      CRTC_R12[3] = 0;            // scroll position Y    
      CRTC_R12[4] = 0;            // scroll position X
      CRTC_R12[5] = 0;            // scroll position Y
      CRTC_R12[6] = 0;            // scroll position X
      CRTC_R12[7] = 0;            // scroll position Y

      rc = 0;
      break;
    }

    case SCREEN_MODE_768x512_FULL: {

      // 1024x1024(768x512),31kHz,65536 colros (for XEiJ only)
      CRTC_R00[0] = 0x0089;

      CRTC_R00[1] = 0x000e;
      CRTC_R00[2] = 0x001c;
      CRTC_R00[3] = 0x007c; 
      CRTC_R00[4] = 0x0237;
      CRTC_R00[5] = 0x0005;
      CRTC_R00[6] = 0x0028;
      CRTC_R00[7] = 0x0228; 
      CRTC_R00[8] = 0x001b;

      CRTC_R20[0] = 0x0716;       // memory mode 7 (for XEiJ only)

      RESET_SYSP;                 // system port for dot clock change - Inside/Out p44

      VDC_R0[0] = 7;              // memory mode 7 (for XEiJ only)
  
//      SCON[1] = 0x001c + 4;           // R02 + 0x04

//      WAIT_SCON;
//      SCON[0] = 0xff;                 // 256 color: R00  else: 0xff
//      SCON[2] = 0x28;                 // R06
//      SCON[3] = 0x16;                 // R20 & 0xff

      VDC_R2[0] = 0x30;           // text/graphic on (w:1024 h:1024)

      CRTC_R12[0] = 0;            // scroll position X
      CRTC_R12[1] = 0;            // scroll position Y

      rc = 0;
      break;
    }

    default:
      // unknown mode
      break;

  }

  set_palette_65536();

  return rc;
}
