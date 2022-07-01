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


int shifts(uint8_t* text, int curr) // D (0->3)
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

    if(id == 0x00)
        fprintf(stderr, "rol ");
    else if (id == 0x01)
        fprintf(stderr, "ror ");
    else if (id == 0x02)
        fprintf(stderr, "rcl ");
    else if (id == 0x03)
        fprintf(stderr, "rcr ");
    else if (id == 0x04)
        fprintf(stderr, "shl ");
    else if (id == 0x05)
        fprintf(stderr, "shr ");
    else if (id == 0x07)
        fprintf(stderr, "sar ");

    printRm(rm, mod, disp, w, 0x00);
    
    if(v == 0x00)
        fprintf(stderr, ", 1\n");
    else
        fprintf(stderr, ", cl\n");

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
        fprintf(stderr, "and ");
    else if (id == 1)
        fprintf(stderr, "or ");
    else if (id == 2)
        fprintf(stderr, "xor ");
    else
        fprintf(stderr, "test ");

    if (dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);

        if (word == 0x00)
            fprintf(stderr, ", %s", regByte[reg]);
        else
            fprintf(stderr, ", %s", regWord[reg]);
    }
    else
    {
        if (word == 0x00)
            fprintf(stderr, ", %s", regByte[reg]);
        else
            fprintf(stderr, ", %s", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp);
    fprintf(stderr, "\n");

    if (dir == 0x00)
    {
        if (word == 0x00)
        {
            if (a->id == 9 && interpret)
            {
                if (id == 0)
                    mem[*getRegister16(0x04)] &= *getRegister8(reg);
                    //fprintf(stderr, "and ");
                else if (id == 1)
                    mem[*getRegister16(0x04)] |= *getRegister8(reg);
                    //fprintf(stderr, "or ");
                else if (id == 2)
                    mem[*getRegister16(0x04)] ^= *getRegister8(reg);
                    //fprintf(stderr, "xor ");
                //else

                    //fprintf(stderr, "test ");
                setFlagsZAndS8(mem[*getRegister16(0x04)]);
            }
            else
            {
                if (id == 0)
                    *getRegister8(a->id) &= *getRegister8(reg);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister8(a->id) |= *getRegister8(reg);
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *getRegister8(a->id) ^= *getRegister8(reg);

                setFlagsZAndS8(*getRegister8(a->id));
            }
        }
        else
        {
            if (a->id == 9 && interpret)
            {
                if (id == 0)
                    *((uint16_t*) (&mem[*getRegister16(0x04)])) &= *getRegister16(reg);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *((uint16_t*)(&mem[*getRegister16(0x04)])) |= *getRegister16(reg);
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *((uint16_t*)(&mem[*getRegister16(0x04)])) ^= *getRegister16(reg);
                //fprintf(stderr, "xor ");
            //else

                //fprintf(stderr, "test ");

                setFlagsZAndS16((uint16_t)(mem[*getRegister16(0x04)]));
            }
            else
            {
                if (id == 0)
                    *getRegister16(a->id) &= *getRegister16(reg);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister16(a->id) |= *getRegister16(reg);
                //fprintf(stderr, "or ");
                else if (id == 2)
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
                if (id == 0)
                    *getRegister8(reg) &= mem[*getRegister16(0x04)];
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister8(reg) |= mem[*getRegister16(0x04)];
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *getRegister8(reg) ^= mem[*getRegister16(0x04)];
                //fprintf(stderr, "xor ");
            //else

                //fprintf(stderr, "test ");
                setFlagsZAndS8(*getRegister8(reg));
            }
            else
            {
                if (id == 0)
                    *getRegister8(reg) &= *getRegister8(a->id);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister8(reg) |= *getRegister8(a->id);
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *getRegister8(reg) ^= *getRegister8(a->id);

                setFlagsZAndS8(*getRegister8(reg));
            }
        }
        else
        {
            if (a->id == 9 && interpret)
            {
                if (id == 0)
                    *getRegister16(reg) &= (uint16_t) (mem[*getRegister16(0x04)]);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister16(reg) |= (uint16_t)(mem[*getRegister16(0x04)]);
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *getRegister16(reg) ^= (uint16_t)(mem[*getRegister16(0x04)]);
                //fprintf(stderr, "xor ");
            //else

                //fprintf(stderr, "test ");
                setFlagsZAndS16(*getRegister16(reg));
            }
            else
            {
                if (id == 0)
                    *getRegister16(reg) &= *getRegister16(a->id);
                //fprintf(stderr, "and ");
                else if (id == 1)
                    *getRegister16(reg) |= *getRegister16(a->id);
                //fprintf(stderr, "or ");
                else if (id == 2)
                    *getRegister16(reg) ^= *getRegister16(a->id);

                setFlagsZAndS16(*getRegister16(reg));
            }
        }
    }

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
    fprintf(stderr, "test ");

    printRm(rm, mod, disp, w, 0x00);

    fprintf(stderr, ", %x", data);
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp);
    fprintf(stderr, "\n");

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
    free(a);
    
    return read;
}

int stringManipulation(uint8_t* text, int curr, int id)
{
    uint8_t w = text[curr] % 2;
    printReadBytes(1, text, curr);

    switch (id)
    {
        case 0:
            fprintf(stderr, "rep\n");
            break;
        case 1:
            fprintf(stderr, "movs\n");
            break;
        case 2:
            fprintf(stderr, "cmps\n");
            break;
        case 3:
            fprintf(stderr, "scas\n");
            break;
        case 4:
            fprintf(stderr, "lods\n");
            break;
        case 5:
            fprintf(stderr, "stos\n");
            break;
    }
    return 1;
}
