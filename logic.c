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

    struct pair* a = getRmAddress(rm, mod, disp, w);
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
        fprintf(stderr, ", 1");
    else
        fprintf(stderr, ", cl");

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, w);
    fprintf(stderr, "\n");

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
                        *(mem + a->disp) <<= rot;
                        cpu->C = *(mem + a->disp) % 2;
                        //*(mem + a->disp) &= 0x00;
                        break;
                    case 0x05:
                        //*(mem + a->disp) &= 0x00;
                        *(mem + a->disp) >>= rot;
                        cpu->C = *(mem + a->disp) / 64;
                        break;
                    case 0x07:
                        *(mem + a->disp) >>= rot;
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
                    setRegister8(a->id, *getRegister8(a->id) << rot);
                    cpu->C = *getRegister8(a->id) % 2;
                    //*getRegister8(a->id) &= 0x00;
                    break;
                case 0x05: // SHR
                    //*getRegister8(a->id) &= 0x00;
                    setRegister8(a->id, *getRegister8(a->id) >> rot);
                    cpu->C = *getRegister8(a->id) / 64;
                    break;
                case 0x07:
                    *getRegister8(a->id) >>= rot;
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
                    *((uint16_t*)(mem + a->disp)) <<= rot;
                    cpu->C = *((uint16_t*)(mem + a->disp)) % 2;
                    //*((uint16_t*)(mem + a->disp)) &= 0x00;
                    break;
                case 0x05: // SHR
                    //*((uint16_t*)(mem + a->disp)) &= 0x00;
                    *((uint16_t*)(mem + a->disp)) >>= rot;
                    cpu->C = *((uint16_t*)(mem + a->disp)) % 64;
                    break;
                case 0x07: // SAR
                    *((uint16_t*)(mem + a->disp)) >>= rot;
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
                        setRegister16(a->id, *getRegister16(a->id) << rot);
                        cpu->C = *getRegister16(a->id) % 2;
                        //*getRegister16(a->id) &= 0x00;
                        break;
                    case 0x05: // SHR
                        
                        //*getRegister16(a->id) &= 0x00;
                        setRegister16(a->id, *getRegister16(a->id) >> rot);
                        cpu->C = *getRegister16(a->id) / 64;
                        break;
                    case 0x07:
                        *getRegister16(a->id) >>= rot;
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
            fprintf(stderr, "%s, ", regByte[reg]);
        else
            fprintf(stderr, "%s, ", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(stderr, "\n");

    if (interpret)
    {
        if (dir == 0x00)
        {
            if (word == 0x00)
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0)
                        mem[a->disp] &= *getRegister8(reg);
                    //fprintf(stderr, "and ");
                    else if (id == 1)
                        mem[a->disp] |= *getRegister8(reg);
                    //fprintf(stderr, "or ");
                    else if (id == 2)
                        mem[a->disp] ^= *getRegister8(reg);
                    //fprintf(stderr, "xor ");
                //else

                    //fprintf(stderr, "test ");

                    setFlagsZAndS8(*(mem + a->disp));
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
                        *((uint16_t*)(mem + a->disp)) &= *getRegister16(reg);
                    //fprintf(stderr, "and ");
                    else if (id == 1)
                        *((uint16_t*)(mem + a->disp)) |= *getRegister16(reg);
                    //fprintf(stderr, "or ");
                    else if (id == 2)
                        *((uint16_t*)(mem + a->disp)) ^= *getRegister16(reg);
                    //fprintf(stderr, "xor ");
                //else

                    //fprintf(stderr, "test ");

                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
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
                        *getRegister8(reg) &= *(mem + a->disp);
                    //fprintf(stderr, "and ");
                    else if (id == 1)
                        *getRegister8(reg) |= *(mem + a->disp);
                    //fprintf(stderr, "or ");
                    else if (id == 2)
                        *getRegister8(reg) ^= *(mem + a->disp);
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
                        *getRegister16(reg) &= *((uint16_t*)(mem + a->disp));
                    //fprintf(stderr, "and ");
                    else if (id == 1)
                        *getRegister16(reg) |= *((uint16_t*)(mem + a->disp));
                    //fprintf(stderr, "or ");
                    else if (id == 2)
                        *getRegister16(reg) ^= *((uint16_t*)(mem + a->disp));
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
    fprintf(stderr, "test ");
    if (w == 0x00 && a->id == 9)
        fprintf(stderr, "byte ");

    printRm(rm, mod, disp, w, 0x00);

    if(w == 0x00)
        fprintf(stderr, ", %x", data);
    else
        fprintf(stderr, ", %04x", data);

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, w);
    fprintf(stderr, "\n");

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
    uint8_t w = text[curr] % 2;
    printReadBytes(2, text, curr);

    fprintf(stderr, "rep ");

    switch (text[curr + 1]) {
    case 0xa4:
        fprintf(stderr, "movsb\n");
        break;
    case 0xa5:
        fprintf(stderr, "movsw\n");
        break;
    case 0xa6:
        fprintf(stderr, "cmpsb\n");
        break;
    case 0xa7:
        fprintf(stderr, "cmpsw\n");
        break;
    case 0xae:
        fprintf(stderr, "scasb\n");
        break;
    case 0xaf:
        fprintf(stderr, "scasw\n");
        break;
    case 0xac:
        fprintf(stderr, "lodsb\n");
        break;
    case 0xad:
        fprintf(stderr, "lodsw\n");
        break;
    case 0xaa:
        fprintf(stderr, "stosb\n");
        break;
    case 0xab:
        fprintf(stderr, "stosw\n");
        break;
    }

    return 2;
}
