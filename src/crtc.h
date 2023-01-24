#ifndef __H_CRTC__
#define __H_CRTC__

// graphic ops memory addresses
#define REG_GVRAM       ((volatile unsigned short*)0xC00000)     // GVRAM
#define REG_CRTC_R00    ((volatile unsigned short*)0xE80000)     // CRTC R00-R08 (Inside X68000 p232)
#define REG_CRTC_R12    ((volatile unsigned short*)0xE80018)     // CRTC R12 for scroll (Insite X68000 p197)
#define REG_CRTC_R20    ((volatile unsigned short*)0xE80028)     // CRTC R20 (Inside X68000 p234)
#define REG_PALETTE     ((volatile unsigned short*)0xE82000)     // graphic palette (Inside X68000 p218)
#define REG_VDC_R0      ((volatile unsigned short*)0xE82400)     // video controller 0 (Inside X68000 p234)
#define REG_VDC_R1      ((volatile unsigned short*)0XE82500)     // video controller 1 (Inside X68000 p188)
#define REG_VDC_R2      ((volatile unsigned short*)0xE82600)     // video controller 2 (Inside X68000 p210)
#define REG_GPIP         ((volatile unsigned char*)0xE88001)     // generic I/O port (Inside X68000 p81)
#define REG_SYSP         ((volatile unsigned char*)0xE8E007)     // system port (Inside/Out X68030 p44)
#define REG_SCON        ((volatile unsigned short*)0xEB080A)     // p173,185,235 (SP control)
#define REG_TACR         ((volatile unsigned char*)0xE88019)     // timer-A control register
#define REG_TADR         ((volatile unsigned char*)0xE8801F)     // timer-A data register

// screen mode
#define SCREEN_MODE_384x256         (0)
#define SCREEN_MODE_512x512         (1)
#define SCREEN_MODE_768x512         (2)
#define SCREEN_MODE_768x512_FULL    (3)       // only for XEiJ extended graphic memory mode 7

// wait vsync/vblank
#define WAIT_VSYNC   while(!(REG_GPIP[0] & 0x10))
#define WAIT_VBLANK    while(REG_GPIP[0] & 0x10)

// reset system port
#define SET_SYSP     (REG_SYSP[0] |=  0x02)
#define RESET_SYSP   (REG_SYSP[0] &= ~0x02)

// wait SCON write ops
#define WAIT_SCON    WAIT_VBLANK

// prototype declarations
int crtc_set_mode(int mode);

#endif