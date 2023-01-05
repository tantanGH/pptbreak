#ifndef __H_CRTC__
#define __H_CRTC__

// graphic ops memory addresses
#define GVRAM       ((volatile unsigned short*)0xC00000)     // GVRAM
#define CRTC_R00    ((volatile unsigned short*)0xE80000)     // CRTC R00-R08 (Inside X68000 p232)
#define CRTC_R12    ((volatile unsigned short*)0xE80018)     // CRTC R12 for scroll (Insite X68000 p197)
#define CRTC_R20    ((volatile unsigned short*)0xE80028)     // CRTC R20 (Inside X68000 p234)
#define PALETTE     ((volatile unsigned short*)0xE82000)     // graphic palette (Inside X68000 p218)
#define VDC_R0      ((volatile unsigned short*)0xE82400)     // video controller 0 (Inside X68000 p234)
#define VDC_R1      ((volatile unsigned short*)0XE82500)     // video controller 1 (Inside X68000 p188)
#define VDC_R2      ((volatile unsigned short*)0xE82600)     // video controller 2 (Inside X68000 p210)
#define GPIP         ((volatile unsigned char*)0xE88001)     // generic I/O port (Inside X68000 p81)
#define SYSP         ((volatile unsigned char*)0xE8E007)     // system port (Inside/Out X68030 p44)
#define BG_SCROLL   ((volatile unsigned short*)0xEB0808)     // p190
#define SCON        ((volatile unsigned short*)0xEB080A)     // p173,185,235 (SP control) H-TOTAL
#define BG_DATA0    ((volatile unsigned short*)0xEBC000)     // p174,177
#define BG_DATA1    ((volatile unsigned short*)0xEBE000)     // p174,177

#define SP_SCROLL_REG   ((volatile unsigned short*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define BG_CONTROL_REG  ((volatile unsigned short*)0xEB0808)        // bg control register (Inside X68000 p190)
#define PCG_DATA_REG    ((volatile unsigned short*)0xEB8000)        // pcg data address (Inside X68000 p174)

#define SP_PALETTE_REG  ((volatile unsigned short*)0xE82200)        // test/bg/sprite palette (Inside X68000 p214)


#define FONT_ADDR_8x8    ((unsigned char*)0xF3A000)
#define FONT_ADDR_8x16   ((unsigned char*)0xF3A800)
#define FONT_ADDR_12x12  ((unsigned char*)0xF3B800)
#define FONT_ADDR_12x24  ((unsigned char*)0xF3D000)

#define FONT_BYTES_8x8   (8)
#define FONT_BYTES_8x16  (16)
#define FONT_BYTES_12x12 (24)
#define FONT_BYTES_12x24 (48)


#define SP_DISP_OFF     (BG_CONTROL_REG[0] &= 0xfdf6)
#define SP_DISP_ON      (BG_CONTROL_REG[0] |= 0x0200)

// screen mode
#define SCREEN_MODE_384x256         (0)
#define SCREEN_MODE_512x512         (1)
#define SCREEN_MODE_768x512         (2)
#define SCREEN_MODE_768x512_FULL    (3)       // only for XEiJ extended graphic memory mode 7

// wait vsync/vblank
#define WAIT_VSYNC   while(!(GPIP[0] & 0x10))
#define WAIT_VBLANK    while(GPIP[0] & 0x10)

// reset system port
#define SET_SYSP     (SYSP[0] |=  0x02)
#define RESET_SYSP   (SYSP[0] &= ~0x02)

// wait SCON write ops
#define WAIT_SCON    WAIT_VBLANK

// prototype declarations
int crtc_set_mode(int mode);

#endif