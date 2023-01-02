#ifndef __H_SP_UTIL__
#define __H_SP_UTIL__

#include <iocslib.h>

#define SP_SCROLL_REG   ((volatile unsigned short*)0xEB0000)        // sprite scroll register (Inside X68000 p179)
#define BG_CONTROL_REG  ((volatile unsigned short*)0xEB0808)        // bg control register (Inside X68000 p190)
#define PCG_DATA_REG    ((volatile unsigned short*)0xEB8000)        // pcg data address (Inside X68000 p174)

#define SP_DISP_OFF     (BG_CONTROL_REG[0] &= 0xfdf6)
#define SP_DISP_ON      (BG_CONTROL_REG[0] |= 0x0200)

#ifndef GPIP
#define GPIP            ((volatile unsigned char*)0xE88001)       // generic I/O port for VSYNC check (Inside X68000 p81)
#define WAIT_VSYNC      while(!(GPIP[0] & 0x10))
#define WAIT_VBLANK     while(GPIP[0] & 0x10)
#endif

#endif