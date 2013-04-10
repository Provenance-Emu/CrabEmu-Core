/*
    This file is part of CrabEmu.

    Copyright (C) 2009, 2012 Lawrence Sebald

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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cheats.h"
#include "list.h"
#include "smsmem.h"

cc_dlist_t *sms_active_cheats = NULL;
int sms_cheats_enabled = 0;

extern uint8 *sms_write_map[256];
extern int sms_bios_active;

int sms_cheat_init(void) {
    if(sms_active_cheats)
        return 0;

    sms_active_cheats = cc_dlist_create();

    if(sms_active_cheats)
        return 0;

    return -1;
}

void sms_cheat_shutdown(void) {
    cc_dlist_node_t *i;

    if(!sms_active_cheats)
        return;

    CC_DLIST_FOREACH(sms_active_cheats, i) {
        free(i->data);
    }

    cc_dlist_destroy(sms_active_cheats);

    sms_cheats_enabled = 0;
    sms_active_cheats = NULL;
}

void sms_cheat_reset(void) {
    cc_dlist_node_t *i;

    if(!sms_active_cheats)
        return;

    sms_cheats_enabled = 0;

    while((i = sms_active_cheats->head)) {
        free(i->data);
        cc_dlist_remove(sms_active_cheats, i);
    }
}

void sms_cheat_frame(void) {
    cc_dlist_node_t *i;
    sms_cheat_t *c;
    uint16 addr;
    uint8 data;

    if(!sms_active_cheats)
        return;

    if(!sms_cheats_enabled)
        return;

    /* If we're executing the bios, don't try to do cheat codes. This isn't
       exactly the safest way to do this, but hopefully it won't break too
       badly... */
    if(sms_bios_active)
        return;

    CC_DLIST_FOREACH(sms_active_cheats, i) {
        c = (sms_cheat_t *)i->data;

        if(c->enabled) {
            addr = (uint16)(c->ar_code >> 8);
            data = (uint8)c->ar_code;

            sms_write_map[addr >> 8][(uint8)addr] = data;
        }
    }
}

int sms_cheat_add(sms_cheat_t *c) {
    return cc_dlist_insert_tail(sms_active_cheats, c);
}

void sms_cheat_read(const char *fn) {
    FILE *fp;
    char linebuf[256], str[64];
    size_t len;
    sms_cheat_t *c;
    unsigned int code1, code2;

    fp = fopen(fn, "r");
    if(!fp)
        return;

    while(fgets(linebuf, 256, fp)) {
        /* Ignore shell-style comments. */
        if(linebuf[0] == '#')
            continue;

        len = strlen(linebuf);

        /* Remove any line endings/whitespace at end. */
        while(isspace(linebuf[len - 1])) {
            linebuf[--len] = 0;
        }

        /* If the line is now blank, ignore it. */
        if(len == 0)
            continue;

        if(linebuf[0] != '0' || linebuf[1] != '0') {
            fprintf(stderr, "sms_cheat_read: Invalid line: %s\n", linebuf);
            continue;
        }

        /* Read the code from the line. */
        sscanf(linebuf, "%4x-%4x", &code1, &code2);
        strncpy(str, linebuf + 10, 63);
        str[63] = 0;

        c = (sms_cheat_t *)malloc(sizeof(sms_cheat_t));
        c->ar_code = (code1 << 16) | code2;
        strcpy(c->desc, str);
        c->enabled = 1;

        sms_cheat_add(c);
    }

    fclose(fp);
}

void sms_cheat_write(const char *fn) {
    FILE *fp;
    sms_cheat_t *c;
    cc_dlist_node_t *i;

    if(!sms_active_cheats || !sms_active_cheats->count)
        return;

    fp = fopen(fn, "w");
    if(!fp)
        return;

    fprintf(fp, "# Cheats file generated by CrabEmu\n");

    CC_DLIST_FOREACH(sms_active_cheats, i) {
        c = (sms_cheat_t *)i->data;

        fprintf(fp, "%04X-%04X %s", (uint16)(c->ar_code >> 16),
                (uint16)c->ar_code, c->desc);
    }

    fclose(fp);
}

void sms_cheat_enable(void) {
    sms_cheats_enabled = 1;
}

void sms_cheat_disable(void) {
    sms_cheats_enabled = 0;
}
