/*
    This file is part of CrabEmu.

    Copyright (C) 2005, 2007, 2008, 2009, 2012, 2014 Lawrence Sebald

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

#ifndef SMSMEM_H
#define SMSMEM_H

#include <stdio.h>
#include "CrabEmu.h"

CLINKAGE

#define SMS_MAPPER_NONE             0
#define SMS_MAPPER_SEGA             1
#define SMS_MAPPER_CODEMASTERS      2
#define SMS_MAPPER_KOREAN           3
#define SMS_MAPPER_93C46            4
#define SMS_MAPPER_CASTLE           5
#define SMS_MAPPER_TEREBI_OEKAKI    6
#define SMS_MAPPER_KOREAN_MSX       7
#define SMS_MAPPER_4PAK_ALL_ACTION  8
#define SMS_MAPPER_JANGGUN          9
#define SMS_MAPPER_TW_MSX_TYPE_A    10
#define SMS_MAPPER_TW_MSX_TYPE_B    11

#define SMS_MEMCTL_IO               (1 << 2)
#define SMS_MEMCTL_BIOS             (1 << 3)
#define SMS_MEMCTL_RAM              (1 << 4)
#define SMS_MEMCTL_CARD             (1 << 5)
#define SMS_MEMCTL_CART             (1 << 6)
#define SMS_MEMCTL_EXP              (1 << 7)

#define SMS_IOCTL_TR_A_DIRECTION    (1 << 0)
#define SMS_IOCTL_TH_A_DIRECTION    (1 << 1)
#define SMS_IOCTL_TR_B_DIRECTION    (1 << 2)
#define SMS_IOCTL_TH_B_DIRECTION    (1 << 3)
#define SMS_IOCTL_TR_A_LEVEL        (1 << 4)
#define SMS_IOCTL_TH_A_LEVEL        (1 << 5)
#define SMS_IOCTL_TR_B_LEVEL        (1 << 6)
#define SMS_IOCTL_TH_B_LEVEL        (1 << 7)

extern void sms_port_write(uint16 port, uint8 data);
extern uint8 sms_port_read(uint16 port);

extern void sms_mem_handle_memctl(uint8 data);
extern void sms_mem_handle_ioctl(uint8 data);

extern int sms_mem_load_bios(const char *fn);
extern int sms_mem_load_rom(const char *fn, int console);

extern int sms_mem_run_bios(int console);

extern int sms_mem_init(void);
extern int sms_mem_shutdown(void);
extern void sms_mem_reset(void);

extern int sms_write_cartram_to_file(const char *fn);
extern int sms_read_cartram_from_file(const char *fn);

extern int sms_ym2413_write_context(FILE *fp);
extern int sms_ym2413_read_context(const uint8 *buf);
extern int sms_mem_write_context(FILE *fp);
extern int sms_mem_read_context(const uint8 *buf);
extern int sms_regs_read_context(const uint8 *buf);
extern int sms_ggregs_read_context(const uint8 *buf);
extern int sms_mapper_read_context(const uint8 *buf);
extern int sms_mapper_pg_read_context(const uint8 *buf);
extern int sms_mapper_mem_read_context(const uint8 *buf);

extern int sms_game_read_context(const uint8 *buf);
extern int sms_game_write_context(FILE *fp);

extern void sms_mem_read_context_v1(FILE *fp);

extern void sms_get_checksums(uint32 *crc, uint32 *adler);

ENDCLINK

#endif /* !SMSMEM_H */
