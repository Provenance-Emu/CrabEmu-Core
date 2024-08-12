/*
    This file is part of CrabEmu.

    Copyright (C) 2005, 2006, 2007, 2008, 2016 Lawrence Sebald

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

#ifndef INSIDE_CRABZ80_GBEXECUTE
#error This file can only be compiled inside of CrabZ80.c. Do not try to include
#error this file in other files.
#endif

++cpu->ir.b.l;
FETCH_ARG8(inst);

switch(inst) {
    case 0x00:  /* RLC B */ //d
    case 0x01:  /* RLC C */ //d
    case 0x02:  /* RLC D */ //d
    case 0x03:  /* RLC E */ //d
    case 0x04:  /* RLC H */ //d
    case 0x05:  /* RLC L */ //d
    case 0x07:  /* RLC A */ //d
        _value = REG8(inst) = (uint8)((REG8(inst) << 1) | (REG8(inst) >> 7));
        cpu->af.b.l = ((!_value) << 7) | ((_value & 0x01) << 4);
        cycles_done += 8;
        goto out;

    case 0x06:  /* RLC (HL) */ //d
        _value = cpu->mread(cpu->hl.w);
        _value = (uint8)((_value << 1) | (_value >> 7));
        cpu->af.b.l = ((!_value) << 7) | ((_value & 0x01) << 4);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x08:  /* RRC B */ //d
    case 0x09:  /* RRC C */ //d
    case 0x0A:  /* RRC D */ //d
    case 0x0B:  /* RRC E */ //d
    case 0x0C:  /* RRC H */ //d
    case 0x0D:  /* RRC L */ //d
    case 0x0F:  /* RRC A */ //d
        _value = (uint8)((REG8(inst) >> 1) | (REG8(inst) << 7));
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x01) << 4);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x0E:  /* RRC (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)((_tmp >> 1) | (_tmp << 7));
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x01) << 4);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x10:  /* RL B */ //d
    case 0x11:  /* RL C */ //d
    case 0x12:  /* RL D */ //d
    case 0x13:  /* RL E */ //d
    case 0x14:  /* RL H */ //d
    case 0x15:  /* RL L */ //d
    case 0x17:  /* RL A */ //d
        _value = (uint8)((REG8(inst) << 1) | ((cpu->af.b.l & 0x10) >> 4));
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x80) >> 3);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x16:  /* RL (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)(_tmp << 1) | ((cpu->af.b.l & 0x10) >> 4);
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x80) >> 3);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x18:  /* RR B */ //d
    case 0x19:  /* RR C */ //d
    case 0x1A:  /* RR D */ //d
    case 0x1B:  /* RR E */ //d
    case 0x1C:  /* RR H */ //d
    case 0x1D:  /* RR L */ //d
    case 0x1F:  /* RR A */ //d
        _value = (uint8)((REG8(inst) >> 1) | ((cpu->af.b.l & 0x10) << 3));
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x01) << 4);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x1E:  /* RR (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)(_tmp >> 1) | ((cpu->af.b.l & 0x10) << 3);
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x01) << 4);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x20:  /* SLA B */ //d
    case 0x21:  /* SLA C */ //d
    case 0x22:  /* SLA D */ //d
    case 0x23:  /* SLA E */ //d
    case 0x24:  /* SLA H */ //d
    case 0x25:  /* SLA L */ //d
    case 0x27:  /* SLA A */ //d
        _value = (uint8)(REG8(inst) << 1);
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x80) >> 3);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x26:  /* SLA (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)(_tmp << 1);
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x80) >> 3);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x28:  /* SRA B */ //d
    case 0x29:  /* SRA C */ //d
    case 0x2A:  /* SRA D */ //d
    case 0x2B:  /* SRA E */ //d
    case 0x2C:  /* SRA H */ //d
    case 0x2D:  /* SRA L */ //d
    case 0x2F:  /* SRA A */ //d
        _value = (uint8)((REG8(inst) >> 1) | (REG8(inst) & 0x80));
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x01) << 4);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x2E:  /* SRA (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)((_tmp >> 1) | (_tmp & 0x80));
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x01) << 4);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    /* The SWAP instructions are unique to the LR35902. On the Z80, these are
       the undocumented SLL instructions. */
    case 0x30:  /* SWAP B */ //d
    case 0x31:  /* SWAP C */ //d
    case 0x32:  /* SWAP D */ //d
    case 0x33:  /* SWAP E */ //d
    case 0x34:  /* SWAP H */ //d
    case 0x35:  /* SWAP L */ //d
    case 0x37:  /* SWAP A */ //d
        _value = REG8(inst);
        /* Doesn't matter if we swap yet, because if it is zero after swapping,
           it was zero before too (and that's the only bit that matters). */
        cpu->af.b.l = (!_value) << 7;
        REG8(inst) = (_value >> 4) | (_value << 4);
        cycles_done += 8;
        goto out;

    case 0x36:  /* SWAP (HL) */ //d
        _value = cpu->mread(cpu->hl.w);
        /* Doesn't matter if we swap yet, because if it is zero after swapping,
           it was zero before too (and that's the only bit that matters). */
        cpu->af.b.l = (!_value) << 7;
        cpu->mwrite(cpu->hl.w, (_value >> 4) | (_value << 4));
        cycles_done += 16;
        goto out;

    case 0x38:  /* SRL B */ //d
    case 0x39:  /* SRL C */ //d
    case 0x3A:  /* SRL D */ //d
    case 0x3B:  /* SRL E */ //d
    case 0x3C:  /* SRL H */ //d
    case 0x3D:  /* SRL L */ //d
    case 0x3F:  /* SRL A */ //d
        _value = (uint8)(REG8(inst) >> 1);
        cpu->af.b.l = ((!_value) << 7) | ((REG8(inst) & 0x01) << 4);
        REG8(inst) = _value;
        cycles_done += 8;
        goto out;

    case 0x3E:  /* SRL (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w);
        _value = (uint8)(_tmp >> 1);
        cpu->af.b.l = ((!_value) << 7) | ((_tmp & 0x01) << 4);
        cpu->mwrite(cpu->hl.w, _value);
        cycles_done += 16;
        goto out;

    case 0x40:  /* BIT 0, B */ //d
    case 0x41:  /* BIT 0, C */ //d
    case 0x42:  /* BIT 0, D */ //d
    case 0x43:  /* BIT 0, E */ //d
    case 0x44:  /* BIT 0, H */ //d
    case 0x45:  /* BIT 0, L */ //d
    case 0x47:  /* BIT 0, A */ //d
    case 0x48:  /* BIT 1, B */ //d
    case 0x49:  /* BIT 1, C */ //d
    case 0x4A:  /* BIT 1, D */ //d
    case 0x4B:  /* BIT 1, E */ //d
    case 0x4C:  /* BIT 1, H */ //d
    case 0x4D:  /* BIT 1, L */ //d
    case 0x4F:  /* BIT 1, A */ //d
    case 0x50:  /* BIT 2, B */ //d
    case 0x51:  /* BIT 2, C */ //d
    case 0x52:  /* BIT 2, D */ //d
    case 0x53:  /* BIT 2, E */ //d
    case 0x54:  /* BIT 2, H */ //d
    case 0x55:  /* BIT 2, L */ //d
    case 0x57:  /* BIT 2, A */ //d
    case 0x58:  /* BIT 3, B */ //d
    case 0x59:  /* BIT 3, C */ //d
    case 0x5A:  /* BIT 3, D */ //d
    case 0x5B:  /* BIT 3, E */ //d
    case 0x5C:  /* BIT 3, H */ //d
    case 0x5D:  /* BIT 3, L */ //d
    case 0x5F:  /* BIT 3, A */ //d
    case 0x60:  /* BIT 4, B */ //d
    case 0x61:  /* BIT 4, C */ //d
    case 0x62:  /* BIT 4, D */ //d
    case 0x63:  /* BIT 4, E */ //d
    case 0x64:  /* BIT 4, H */ //d
    case 0x65:  /* BIT 4, L */ //d
    case 0x67:  /* BIT 4, A */ //d
    case 0x68:  /* BIT 5, B */ //d
    case 0x69:  /* BIT 5, C */ //d
    case 0x6A:  /* BIT 5, D */ //d
    case 0x6B:  /* BIT 5, E */ //d
    case 0x6C:  /* BIT 5, H */ //d
    case 0x6D:  /* BIT 5, L */ //d
    case 0x6F:  /* BIT 5, A */ //d
    case 0x70:  /* BIT 6, B */ //d
    case 0x71:  /* BIT 6, C */ //d
    case 0x72:  /* BIT 6, D */ //d
    case 0x73:  /* BIT 6, E */ //d
    case 0x74:  /* BIT 6, H */ //d
    case 0x75:  /* BIT 6, L */ //d
    case 0x77:  /* BIT 6, A */ //d
    case 0x78:  /* BIT 7, B */ //d
    case 0x79:  /* BIT 7, C */ //d
    case 0x7A:  /* BIT 7, D */ //d
    case 0x7B:  /* BIT 7, E */ //d
    case 0x7C:  /* BIT 7, H */ //d
    case 0x7D:  /* BIT 7, L */ //d
    case 0x7F:  /* BIT 7, A */ //d
        _tmp = REG8(inst) & (1 << ((inst >> 3) & 0x07));
        cpu->af.b.l = ((!_tmp) << 7) | 0x20 | (cpu->af.b.l & 0x10);
        cycles_done += 8;
        goto out;

    case 0x46:  /* BIT 0, (HL) */ //d
    case 0x4E:  /* BIT 1, (HL) */ //d
    case 0x56:  /* BIT 2, (HL) */ //d
    case 0x5E:  /* BIT 3, (HL) */ //d
    case 0x66:  /* BIT 4, (HL) */ //d
    case 0x6E:  /* BIT 5, (HL) */ //d
    case 0x76:  /* BIT 6, (HL) */ //d
    case 0x7E:  /* BIT 7, (HL) */ //d
        _tmp = cpu->mread(cpu->hl.w) & (1 << ((inst >> 3) & 0x07));
        cpu->af.b.l = ((!_tmp) << 7) | 0x20 | (cpu->af.b.l & 0x10);
        cycles_done += 16;
        goto out;

    case 0x80:  /* RES 0, B */ //d
    case 0x81:  /* RES 0, C */ //d
    case 0x82:  /* RES 0, D */ //d
    case 0x83:  /* RES 0, E */ //d
    case 0x84:  /* RES 0, H */ //d
    case 0x85:  /* RES 0, L */ //d
    case 0x87:  /* RES 0, A */ //d
    case 0x88:  /* RES 1, B */ //d
    case 0x89:  /* RES 1, C */ //d
    case 0x8A:  /* RES 1, D */ //d
    case 0x8B:  /* RES 1, E */ //d
    case 0x8C:  /* RES 1, H */ //d
    case 0x8D:  /* RES 1, L */ //d
    case 0x8F:  /* RES 1, A */ //d
    case 0x90:  /* RES 2, B */ //d
    case 0x91:  /* RES 2, C */ //d
    case 0x92:  /* RES 2, D */ //d
    case 0x93:  /* RES 2, E */ //d
    case 0x94:  /* RES 2, H */ //d
    case 0x95:  /* RES 2, L */ //d
    case 0x97:  /* RES 2, A */ //d
    case 0x98:  /* RES 3, B */ //d
    case 0x99:  /* RES 3, C */ //d
    case 0x9A:  /* RES 3, D */ //d
    case 0x9B:  /* RES 3, E */ //d
    case 0x9C:  /* RES 3, H */ //d
    case 0x9D:  /* RES 3, L */ //d
    case 0x9F:  /* RES 3, A */ //d
    case 0xA0:  /* RES 4, B */ //d
    case 0xA1:  /* RES 4, C */ //d
    case 0xA2:  /* RES 4, D */ //d
    case 0xA3:  /* RES 4, E */ //d
    case 0xA4:  /* RES 4, H */ //d
    case 0xA5:  /* RES 4, L */ //d
    case 0xA7:  /* RES 4, A */ //d
    case 0xA8:  /* RES 5, B */ //d
    case 0xA9:  /* RES 5, C */ //d
    case 0xAA:  /* RES 5, D */ //d
    case 0xAB:  /* RES 5, E */ //d
    case 0xAC:  /* RES 5, H */ //d
    case 0xAD:  /* RES 5, L */ //d
    case 0xAF:  /* RES 5, A */ //d
    case 0xB0:  /* RES 6, B */ //d
    case 0xB1:  /* RES 6, C */ //d
    case 0xB2:  /* RES 6, D */ //d
    case 0xB3:  /* RES 6, E */ //d
    case 0xB4:  /* RES 6, H */ //d
    case 0xB5:  /* RES 6, L */ //d
    case 0xB7:  /* RES 6, A */ //d
    case 0xB8:  /* RES 7, B */ //d
    case 0xB9:  /* RES 7, C */ //d
    case 0xBA:  /* RES 7, D */ //d
    case 0xBB:  /* RES 7, E */ //d
    case 0xBC:  /* RES 7, H */ //d
    case 0xBD:  /* RES 7, L */ //d
    case 0xBF:  /* RES 7, A */ //d
        REG8(inst) &= ~(1 << ((inst >> 3) & 0x07));
        cycles_done += 8;
        goto out;

    case 0x86:  /* RES 0, (HL) */ //d
    case 0x8E:  /* RES 1, (HL) */ //d
    case 0x96:  /* RES 2, (HL) */ //d
    case 0x9E:  /* RES 3, (HL) */ //d
    case 0xA6:  /* RES 4, (HL) */ //d
    case 0xAE:  /* RES 5, (HL) */ //d
    case 0xB6:  /* RES 6, (HL) */ //d
    case 0xBE:  /* RES 7, (HL) */ //d
        _value = cpu->mread(cpu->hl.w);
        cpu->mwrite(cpu->hl.w, _value & ~(1 << ((inst >> 3) & 0x07)));
        cycles_done += 16;
        goto out;

    case 0xC0:  /* SET 0, B */ //d
    case 0xC1:  /* SET 0, C */ //d
    case 0xC2:  /* SET 0, D */ //d
    case 0xC3:  /* SET 0, E */ //d
    case 0xC4:  /* SET 0, H */ //d
    case 0xC5:  /* SET 0, L */ //d
    case 0xC7:  /* SET 0, A */ //d
    case 0xC8:  /* SET 1, B */ //d
    case 0xC9:  /* SET 1, C */ //d
    case 0xCA:  /* SET 1, D */ //d
    case 0xCB:  /* SET 1, E */ //d
    case 0xCC:  /* SET 1, H */ //d
    case 0xCD:  /* SET 1, L */ //d
    case 0xCF:  /* SET 1, A */ //d
    case 0xD0:  /* SET 2, B */ //d
    case 0xD1:  /* SET 2, C */ //d
    case 0xD2:  /* SET 2, D */ //d
    case 0xD3:  /* SET 2, E */ //d
    case 0xD4:  /* SET 2, H */ //d
    case 0xD5:  /* SET 2, L */ //d
    case 0xD7:  /* SET 2, A */ //d
    case 0xD8:  /* SET 3, B */ //d
    case 0xD9:  /* SET 3, C */ //d
    case 0xDA:  /* SET 3, D */ //d
    case 0xDB:  /* SET 3, E */ //d
    case 0xDC:  /* SET 3, H */ //d
    case 0xDD:  /* SET 3, L */ //d
    case 0xDF:  /* SET 3, A */ //d
    case 0xE0:  /* SET 4, B */ //d
    case 0xE1:  /* SET 4, C */ //d
    case 0xE2:  /* SET 4, D */ //d
    case 0xE3:  /* SET 4, E */ //d
    case 0xE4:  /* SET 4, H */ //d
    case 0xE5:  /* SET 4, L */ //d
    case 0xE7:  /* SET 4, A */ //d
    case 0xE8:  /* SET 5, B */ //d
    case 0xE9:  /* SET 5, C */ //d
    case 0xEA:  /* SET 5, D */ //d
    case 0xEB:  /* SET 5, E */ //d
    case 0xEC:  /* SET 5, H */ //d
    case 0xED:  /* SET 5, L */ //d
    case 0xEF:  /* SET 5, A */ //d
    case 0xF0:  /* SET 6, B */ //d
    case 0xF1:  /* SET 6, C */ //d
    case 0xF2:  /* SET 6, D */ //d
    case 0xF3:  /* SET 6, E */ //d
    case 0xF4:  /* SET 6, H */ //d
    case 0xF5:  /* SET 6, L */ //d
    case 0xF7:  /* SET 6, A */ //d
    case 0xF8:  /* SET 7, B */ //d
    case 0xF9:  /* SET 7, C */ //d
    case 0xFA:  /* SET 7, D */ //d
    case 0xFB:  /* SET 7, E */ //d
    case 0xFC:  /* SET 7, H */ //d
    case 0xFD:  /* SET 7, L */ //d
    case 0xFF:  /* SET 7, A */ //d
        REG8(inst) |= (1 << ((inst >> 3) & 0x07));
        cycles_done += 8;
        goto out;

    case 0xC6:  /* SET 0, (HL) */ //d
    case 0xCE:  /* SET 1, (HL) */ //d
    case 0xD6:  /* SET 2, (HL) */ //d
    case 0xDE:  /* SET 3, (HL) */ //d
    case 0xE6:  /* SET 4, (HL) */ //d
    case 0xEE:  /* SET 5, (HL) */ //d
    case 0xF6:  /* SET 6, (HL) */ //d
    case 0xFE:  /* SET 7, (HL) */ //d
        _value = cpu->mread(cpu->hl.w);
        cpu->mwrite(cpu->hl.w, _value | (1 << ((inst >> 3) & 0x07)));
        cycles_done += 16;
        goto out;
}
