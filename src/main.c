#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iocslib.h>

#include "crtc.h"
#include "sp_util.h"
#include "sp_pattern.h"

// clear and initialize screen and sprites
static void setup_screen() {

    CRTMOD(12);
    B_CUROFF();
    G_CLR_ON();

    crtc_initialize_screen(SCREEN_MODE_384x256);

    SP_INIT();

    for (int i = 0; i < 256; i++) {
        SP_CGCLR(i);
    }

    SP_ON();
}

static void reset_screen() {

    crtc_initialize_screen(SCREEN_MODE_768x512);

    G_CLR_ON();    
    B_CURON();
}

// set sprite palette
static void setup_sp_palette(int palette_block, unsigned short* palette_data) {
    for (int i = 0; i < 16; i++) {
        SPALET(0x80000000 | i, palette_block, sp_palette_data[i]);
    }
}

// set sprite patterns
static void setup_sp_pattern(int pattern_number, unsigned short* pattern_data, int pattern_count) {
    for (int i = 0; i < pattern_count; i++) {
        for (int j = 0; j < 0x40; j++) {
            PCG_DATA_REG[ ( pattern_number + i)  * 0x40 + j ] = pattern_data[ i * 0x40 + j];
        }
    }
}

// scroll sprite
inline static void scroll_sp(short sp_number, short x, short y, short color, short pattern, short priority) {
    int ofs = sp_number * 4;
    SP_SCROLL_REG[ofs+0] = x;                           // XPOS
    SP_SCROLL_REG[ofs+1] = y;                           // YPOS
    SP_SCROLL_REG[ofs+2] = (color << 8) | pattern;      // COLOR | SPAT#
    SP_SCROLL_REG[ofs+3] = priority;                    // PRW
}

// main
int main(int argc, char* argv[]) {

    B_SUPER(0);

    setup_screen();
    setup_sp_palette(1, sp_palette_data);
    setup_sp_pattern(0, sp_pattern_data, 4);

    struct BOXPTR box = {0,0,383,255,0xffe0,(unsigned short)0xffff};
    BOX(&box);    

    int ball_count = 4;
    int x_min = 16;
    int x_max = 16 + 384 - 32;
    int y_min = 16;
    int y_max = 16 + 256 - 32;
    int x[] = { 16, 16, 16, 16, };
    int y[] = { 16, 16, 16, 16, };
    int ax[] = { 3, 5, 7, 11, };
    int ay[] = { 4, 4, 4, 4, };
    int g = 1;

    for (;;) {

        WAIT_VSYNC;
        WAIT_VBLANK;

        SP_DISP_OFF;
        for (int i = 0; i < ball_count; i++) {
            scroll_sp(i*4+0, x[i]     , y[i]     , 1, 0, 3);
            scroll_sp(i*4+1, x[i] + 16, y[i]     , 1, 1, 3);
            scroll_sp(i*4+2, x[i]     , y[i] + 16, 1, 2, 3);
            scroll_sp(i*4+3, x[i] + 16, y[i] + 16, 1, 3, 3);
        }
        SP_DISP_ON;

        for (int i = 0; i < ball_count; i++) {

            x[i] += ax[i];
            y[i] += ay[i];

            if (x[i] < x_min) {
                ax[i] *= -1;
                x[i] = x_min + (x_min - x[i]);
            }
            if (x[i] > x_max) {
                ax[i] *= -1;
                x[i] = x_max - (x[i] - x_max);
            }
            if (y[i] < y_min) {
                ay[i] *= -1;
                y[i] = y_min + (y_min - y[i]);
            }
            if (y[i] > y_max) {
                ay[i] *= -1;
                y[i] = y_max - (y[i] - y_max);
            }
            ay[i] += g;

        }

        if (BITSNS(0) & 0x02) {
          break;
        }

    }

    reset_screen();

}