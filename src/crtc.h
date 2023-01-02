#ifndef __H_CRTC__
#define __H_CRTC__

// graphic ops memory addresses
#define GVRAM       ((volatile unsigned short*)0xC00000)     // GVRAM
#define CRTC_R00    ((volatile unsigned short*)0xE80000)     // CRTC R00-R08 (Inside X68000 p232)
#define CRTC_R12    ((volatile unsigned short*)0xE80018)     // CRTC R12 for scroll (Insite X68000 p197)
#define CRTC_R20    ((volatile unsigned short*)0xE80028)     // CRTC R20 (Inside X68000 p234)
#define VDC_R1      ((volatile unsigned short*)0xE82400)     // video controller (Inside X68000 p234)
#define VDC_R2      ((volatile unsigned short*)0xE82600)     // video controller (Inside X68000 p210)
#define PALETTE     ((volatile unsigned short*)0xE82000)     // graphic palette (Inside X68000 p218)
#define GPIP         ((volatile unsigned char*)0xE88001)     // generic I/O port (Inside X68000 p81)
#define SYSP         ((volatile unsigned char*)0xE8E007)     // system port (Inside/Out X68030 p44)
#define SCON        ((volatile unsigned short*)0xEB080A)     // sprite controller (Inside X68000 p235)

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
int crtc_initialize_screen(int mode);
int crtc_initialize_palette(int mode);

#endif