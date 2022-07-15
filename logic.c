#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"
#include "unistd.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "a.out.h"
#include "stdio.h"
#include "utility.h"


int shifts(uint8_t* text, int curr)
{
    uint8_t w = text[curr] % 2;
    uint8_t v = (text[curr] % 4) / 2;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    uint8_t id = (text[curr+1] % 64) / 8;
    int read;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        printReadBytes(4, text, curr);
        disp = text[curr+3] * 256 + text[curr+2];
        read = 4;
    }
    else if (mod == 0x01)
    {
        printReadBytes(3, text, curr);
        disp = (int8_t) text[curr+2];
        read = 3;
    }
    else
    {
        printReadBytes(2, text, curr);
        read = 2;
    }

    struct pair* a = getRmAddress(rm, mod, disp, w);
    if(id == 0x00)
        fprintf(fd, "rol ");
    else if (id == 0x01)
        fprintf(fd, "ror ");
    else if (id == 0x02)
        fprintf(fd, "rcl ");
    else if (id == 0x03)
        fprintf(fd, "rcr ");
    else if (id == 0x04)
        fprintf(fd, "shl ");
    else if (id == 0x05)
        fprintf(fd, "shr ");
    else if (id == 0x07)
        fprintf(fd, "sar ");

    printRm(rm, mod, disp, w, 0x00);
    
    if(v == 0x00)
        fprintf(fd, ", 1");
    else
        fprintf(fd, ", cl");

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, w);
    fprintf(fd, "\n");

    if (interpret)
    {
        int rot;
        if (v == 0x00)
            rot = 1;
        else
            rot = *getRegister8(0x01);

        int tmp;
        if (w == 0x00)
        {
            if (a->id == 9)
            {
                switch (id)
                {
                    case 0x00: // ROL
                        *(mem + a->disp) <<= rot;
                        break;
                    case 0x01: // ROR
                        *(mem + a->disp) >>= rot;
                        break;

                    case 0x02: // RCL
                        *(mem + a->disp) <<= rot;
                        tmp = cpu->C;
                        cpu->C = *(mem + a->disp) % 2;
                        if (tmp)
                            *(mem + a->disp) |= 0x01;
                        else
                            *(mem + a->disp) &= 0x00;
                        break;

                    case 0x03: // RCR
                        tmp = cpu->C;
                        cpu->C = *(mem + a->disp) % 2;
                        if (tmp)
                            *(mem + a->disp) |= 0x01;
                        else
                            *(mem + a->disp) &= 0x00;
                        *(mem + a->disp) >>= rot;
                        break;

                    case 0x04: // SHL
                        cpu->C = ((*(int8_t*)(mem + a->disp) << (rot -1)) >> 7) & 1;
                        *(mem + a->disp) <<= rot;
                        if(rot == 1)
                        {
                            if (*(mem + a->disp) / 64 == cpu->C)
                                cpu->O = 0;
                            else
                                cpu->O = 1;
                        }
                        break;

                    case 0x05: // Does not appear
                        break;

                    case 0x07:  // SAR
                        cpu->C = (*(int8_t*)(mem + a->disp) >> (rot -1)) & 1;
                        *(int8_t*)(mem + a->disp) >>= rot;
                        break;
                }
                setFlagsZAndS8(*(mem + a->disp));
            }
            else
            {
                switch (id)
                {
                    case 0x00: // ROL
                        *getRegister8(a->id) <<= rot;
                        break;
                    case 0x01: // ROR
                        *getRegister8(a->id) >>= rot;
                        break;

                    case 0x02: // RCL
                        *getRegister8(a->id) <<= rot;
                        tmp = cpu->C;
                        cpu->C = *getRegister8(a->id) % 2;
                        if (tmp)
                            *getRegister8(a->id) |= 0x01;
                        else
                            *getRegister8(a->id) &= 0x00;
                        break;

                    case 0x03: // RCR
                        tmp = cpu->C;
                        cpu->C = *getRegister8(a->id) % 2;
                        if (tmp)
                            *getRegister8(a->id) |= 0x01;
                        else
                            *getRegister8(a->id) &= 0x00;
                        *getRegister8(a->id) >>= rot;
                        break;

                    case 0x04: // SHL
                        cpu->C = ((*(int8_t*)(getRegister8(a->id)) << (rot - 1)) >> 7) & 1;
                        setRegister8(a->id, *getRegister8(a->id) << rot);

                        if (*getRegister8(a->id) / 64 == cpu->C)
                            cpu->O = 0;
                        else
                            cpu->O = 1;
                        break;

                    case 0x05: // Does not appear
                        break;

                    case 0x07:
                        cpu->C = (*(int8_t*)(getRegister8(a->id)) >> (rot - 1)) & 1;
                        *(int8_t*)(getRegister8(a->id)) >>= rot;
                        break;
                }
                setFlagsZAndS8(*getRegister8(a->id));
            }
        }
        else
        {
            if (a->id == 9)
            {
                switch (id)
                {
                    case 0x00: // ROL
                        *((uint16_t*)(mem + a->disp)) <<= rot;
                        break;
                    case 0x01: // ROR
                        *((uint16_t*)(mem + a->disp)) >>= rot;
                        break;

                    case 0x02: // RCL
                        *((uint16_t*)(mem + a->disp)) <<= rot;
                        tmp = cpu->C;
                        cpu->C = *((uint16_t*)(mem + a->disp)) % 2;
                        if (tmp)
                            *((uint16_t*)(mem + a->disp)) |= 0x01;
                        else
                            *((uint16_t*)(mem + a->disp)) &= 0x00;
                        break;

                    case 0x03: // RCR
                        tmp = cpu->C;
                        cpu->C = *((uint16_t*)(mem + a->disp)) % 2;
                        if (tmp)
                            *((uint16_t*)(mem + a->disp)) |= 0x01;
                        else
                            *((uint16_t*)(mem + a->disp)) &= 0x00;
                        *((uint16_t*)(mem + a->disp)) >>= rot;
                        break;

                    case 0x04: // SHL
                        cpu->C = ((*((int16_t*)(mem + a->disp)) << (rot - 1)) >> 15) & 1;
                        *((uint16_t*)(mem + a->disp)) <<= rot;
                        
                        if (*((uint16_t*)(mem + a->disp)) / 32768 == cpu->C)
                            cpu->O = 0;
                        else
                            cpu->O = 1;
                        break;

                    case 0x05: // Does not appear
                        break;
                    case 0x07: // SAR
                        cpu->C = (*((int16_t*)(mem + a->disp)) >> (rot - 1)) & 1;
                        *((int16_t*)(mem + a->disp)) >>= rot;
                        break;
                }
                setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
            }
            else
            {
                switch (id)
                {
                    case 0x00: // ROL
                        *getRegister16(a->id) <<= rot;
                        break;
                    case 0x01: // ROR
                        *getRegister16(a->id) >>= rot;
                        break;

                    case 0x02: // RCL
                        *getRegister16(a->id) <<= rot;
                        tmp = cpu->C;
                        cpu->C = *getRegister16(a->id) % 2;
                        if (tmp)
                            *getRegister16(a->id) |= 0x01;
                        else
                            *getRegister16(a->id) &= 0x00;
                        break;

                    case 0x03: // RCR
                        tmp = cpu->C;
                        cpu->C = *getRegister16(a->id) % 2;
                        if (tmp)
                            *getRegister16(a->id) |= 0x01;
                        else
                            *getRegister16(a->id) &= 0x00;
                        *getRegister16(a->id) >>= rot;
                        break;

                    case 0x04: // SHL
                        cpu->C = ((*getRegister16(a->id) << (rot - 1)) >> 15) & 1;
                        setRegister16(a->id, *getRegister16(a->id) << rot);
                        if (*getRegister16(a->id) / 32768 == cpu->C)
                            cpu->O = 0;
                        else
                            cpu->O = 1;

                        break;

                    case 0x05: // Does not appear
                        break;

                    case 0x07:
                        cpu->C = (*(int16_t*)(getRegister16(a->id)) >> (rot - 1)) & 1;
                        *(int16_t*) (getRegister16(a->id)) >>= rot;
                        break;
                }
                setFlagsZAndS16(*getRegister16(a->id));
            }
        }
    }

    free(a);
    return read;
}

int logicRegMemToEither(uint8_t* text, int curr, int id)
{
    uint8_t word = text[curr] % 2;
    uint8_t dir = (text[curr] % 4) / 2;
    uint8_t reg = (text[curr+1] % 64) / 8;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    int read;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = 4;
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = 3;
    }
    else
    {
        read = 2;
    }

    struct pair* a = getRmAddress(rm, mod, disp, word);
    printReadBytes(read, text, curr);
    if (id == 0)
        fprintf(fd, "and ");
    else if (id == 1)
        fprintf(fd, "or ");
    else if (id == 2)
        fprintf(fd, "xor ");
    else
        fprintf(fd, "test ");

    if (dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);

        if (word == 0x00)
            fprintf(fd, ", %s", regByte[reg]);
        else
            fprintf(fd, ", %s", regWord[reg]);
    }
    else
    {
        if (word == 0x00)
            fprintf(fd, "%s, ", regByte[reg]);
        else
            fprintf(fd, "%s, ", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (dir == 0x00)
        {
            if (word == 0x00)
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // AND
                        mem[a->disp] &= *getRegister8(reg);
                    else if (id == 1) // OR
                        mem[a->disp] |= *getRegister8(reg);
                    else if (id == 2) // XOR
                        mem[a->disp] ^= *getRegister8(reg);
                    //else TEST Does not appear

                    setFlagsZAndS8(*(mem + a->disp));
                }
                else
                {
                    if (id == 0) // AND
                        *getRegister8(a->id) &= *getRegister8(reg);
                    else if (id == 1) // OR
                        *getRegister8(a->id) |= *getRegister8(reg);
                    else if (id == 2) // XOR
                        *getRegister8(a->id) ^= *getRegister8(reg);

                    setFlagsZAndS8(*getRegister8(a->id));
                }
            }
            else
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // AND
                        *((uint16_t*)(mem + a->disp)) &= *getRegister16(reg);
                    else if (id == 1) // OR
                        *((uint16_t*)(mem + a->disp)) |= *getRegister16(reg);
                    else if (id == 2) // XOR
                        *((uint16_t*)(mem + a->disp)) ^= *getRegister16(reg);
                    //else TEST Does not appear 

                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else
                {
                    if (id == 0) // AND
                        *getRegister16(a->id) &= *getRegister16(reg);
                    else if (id == 1) // OR
                        *getRegister16(a->id) |= *getRegister16(reg);
                    else if (id == 2) // XOR
                        *getRegister16(a->id) ^= *getRegister16(reg);

                    setFlagsZAndS16(*getRegister16(a->id));
                }
            }
        }
        else
        {
            if (word == 0x00)
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // AND
                        *getRegister8(reg) &= *(mem + a->disp);
                    else if (id == 1) // OR
                        *getRegister8(reg) |= *(mem + a->disp);
                    else if (id == 2) // XOR
                        *getRegister8(reg) ^= *(mem + a->disp);
                    // else TEST Does not appear

                    setFlagsZAndS8(*getRegister8(reg));
                }
                else
                {
                    if (id == 0) // AND
                        *getRegister8(reg) &= *getRegister8(a->id);
                    else if (id == 1) // OR
                        *getRegister8(reg) |= *getRegister8(a->id);
                    else if (id == 2) // XOR
                        *getRegister8(reg) ^= *getRegister8(a->id);

                    setFlagsZAndS8(*getRegister8(reg));
                }
            }
            else
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // AND
                        *getRegister16(reg) &= *((uint16_t*)(mem + a->disp));
                    else if (id == 1) // OR
                        *getRegister16(reg) |= *((uint16_t*)(mem + a->disp));
                    else if (id == 2) // XOR
                        *getRegister16(reg) ^= *((uint16_t*)(mem + a->disp));
                    // else TEST Does not appear

                    setFlagsZAndS16(*getRegister16(reg));
                }
                else
                {
                    if (id == 0) // AND
                        *getRegister16(reg) &= *getRegister16(a->id);
                    else if (id == 1) // OR
                        *getRegister16(reg) |= *getRegister16(a->id);
                    else if (id == 2) // XOR
                        *getRegister16(reg) ^= *getRegister16(a->id);

                    setFlagsZAndS16(*getRegister16(reg));
                }
            }
        }
    }

    cpu->C = 0;
    cpu->O = 0;
    free(a);
    return read;
}

int logicImmediateToRegMem(uint8_t* text, int curr)
{
    uint8_t w = text[curr] % 2;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    uint8_t id = (text[curr+1] % 64) / 8;
    int read;
    uint16_t data;
    int toRead = 3;
    if(w == 0x01)
        toRead += 1;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = toRead + 2;

        if(w == 0x01)
            data = text[curr+5] * 256 + text[curr+4];
        else
            data = text[curr+4];
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = toRead + 1;

        if(w == 0x01)
            data = text[curr+4] * 256 + text[curr+3];
        else
            data = text[curr+3];
    }
    else
    {
        read = toRead;

        if(w == 0x01)
            data = text[curr+3] * 256 + text[curr+2];
        else
            data = text[curr+2];
    }

    struct pair* a = getRmAddress(rm, mod, disp, w);
    printReadBytes(read, text, curr);
    fprintf(fd, "test ");
    if (w == 0x00 && a->id == 9)
        fprintf(fd, "byte ");

    printRm(rm, mod, disp, w, 0x00);

    if(w == 0x00)
        fprintf(fd, ", %x", data);
    else
        fprintf(fd, ", %04x", data);

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, w);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (w == 0x00)
        {
            if (a->id == 9 && interpret)
                setFlagsZAndS8(*(mem + a->disp) & ((uint8_t)data));
            else
                setFlagsZAndS8(*getRegister8(a->id) & ((uint8_t)data));
        }
        else
        {
            if (a->id == 9 && interpret)
                setFlagsZAndS16(*((uint16_t*)(mem + a->disp)) & data);
            else
                setFlagsZAndS16(*getRegister16(a->id) & data);
        }

        cpu->C = 0;
        cpu->O = 0;
    }

    free(a);
    
    return read;
}

int stringManipulation(uint8_t* text, int curr, int id)
{
    // Not tested, does not appear in files 1-6.c
    uint8_t w = text[curr] % 2;
    printReadBytes(2, text, curr);

    fprintf(fd, "rep ");

    switch (text[curr + 1]) {
    case 0xa4:
        fprintf(fd, "movsb\n");
        break;
    case 0xa5:
        fprintf(fd, "movsw\n");
        break;
    case 0xa6:
        fprintf(fd, "cmpsb\n");
        break;
    case 0xa7:
        fprintf(fd, "cmpsw\n");
        break;
    case 0xae:
        fprintf(fd, "scasb\n");
        break;
    case 0xaf:
        fprintf(fd, "scasw\n");
        break;
    case 0xac:
        fprintf(fd, "lodsb\n");
        break;
    case 0xad:
        fprintf(fd, "lodsw\n");
        break;
    case 0xaa:
        fprintf(fd, "stosb\n");
        break;
    case 0xab:
        fprintf(fd, "stosw\n");
        break;
    }

    return 2;
}