/*
    This file is part of CrabEmu.

    Copyright (C) 2005, 2006, 2007, 2008, 2009, 2012, 2014 Lawrence Sebald

    CrabEmu is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 
    as published by the Free Software Foundation.

    CrabEmu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CrabEmu; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SMSVDP_H
#define SMSVDP_H

#include <stdio.h>
#include "CrabEmu.h"

CLINKAGE

/* Pattern structure */
typedef struct smspat_s {
    /* Preconverted texture */
    uint8 texture[4][64];

    /* Dirty flag */
    uint8 dirty;
} sms_vdp_pattern_t;

typedef struct smsvdp_s {
    /* Command Word - written to the control port
       Consists of a 2-bit code and a 14-bit address */
    uint8 code;
    uint16 addr;
    uint8 addr_latch;

    /* Read buffer - used for reads from VRAM */
    uint8 read_buf;

    /* Software palette, stored both in SMS and
       native formats (CRAM) */
    uint8 *cram;
    pixel_t *pal;

    /* Status flags */
    uint8 status;

    /* VDP Registers */
    uint8 regs[16];

    /* VRAM */
    uint8 *vram;

    /* Palette latch, for GG */
    uint8 pal_latch;

    /* Line interrupt counter */
    uint8 linecnt;

    /* Internal flags */
    uint32 flags;

    /* Preconverted patterns */
    sms_vdp_pattern_t pattern[512];

    /* Background priority levels */
    uint8 bg_prio[32];

    /* Internal Framebuffer - 256x256 or 512x512 pixels 32bpp or 16bpp */
    pixel_t *framebuffer;
    pixel_t *framebuffer_base;

    /* Background alpha levels, used with the priority to determine if
       a given pixel is invisible or not */
    uint8 alpha[256 + 8];

    /* Viewable line count */
    int lines;

    /* Video Mode and Machine type */
    int vidmode;
    int machine;

    /* Temporary storage for a changed vertical scroll value */
    uint8 v_scroll;

    /* Latched HCounter value. */
    uint8 hcnt;

    /* The last rendered line */
    int line;

    /* Name table address (for mode 4) */
    uint8 *name_table;

    /* Sprite Attribute Table address (mode 4) */
    uint8 *sat;

    /* Scroll values */
    int xscroll_coarse;
    int xscroll_fine;
    int yscroll_fine;

    /* Framebuffer size, in lg(pixels) */
    int borders;
    int fb_x;
    int fb_y;
} sms_vdp_t;

#define SMS_VDP_FLAG_BYTES_WRITTEN  0x00000001
#define SMS_VDP_FLAG_VSCROLL_CHG    0x00000002
#define SMS_VDP_FLAG_LINE_INT       0x00000004

extern void sms_vdp_update_cache(int pat);

extern void sms_vdp_data_write(uint8 data);
extern void sms_vdp_ctl_write(uint8 data);

extern void *sms_vdp_framebuffer(void);
extern void sms_vdp_framesize(uint32_t *x, uint32_t *y);
extern void sms_vdp_activeframe(uint32_t *x, uint32_t *y, uint32_t *w,
                                uint32_t *h);

extern uint8 sms_vdp_vcnt_read(void);
extern uint8 sms_vdp_hcnt_read(void);
extern uint8 sms_vdp_data_read(void);
extern uint8 sms_vdp_status_read(void);

extern void sms_vdp_hcnt_latch(int cycles);

extern uint32 sms_vdp_execute(int line, int skip);
extern uint32 tms9918a_vdp_execute(int line, void (*irqfunc)(), int skip);
extern void sms_vdp_dump_vram(const char *fn);

extern int sms_vdp_init(int mode, int borders);
extern int sms_vdp_reset(void);
extern int sms_vdp_shutdown(void);

#define SMS_VDP_MACHINE_SMS1     1
#define SMS_VDP_MACHINE_SMS2     2
#define SMS_VDP_MACHINE_TMS9918A 3
extern void sms_vdp_set_vidmode(int mode, int machine);

extern int sms_vdp_write_context(FILE *fp);
extern int sms_vdp_read_context(const uint8 *buf);
extern void sms_vdp_read_context_v1(FILE *fp);

extern sms_vdp_t smsvdp;

ENDCLINK

#endif /* !SMSVDP_H */
