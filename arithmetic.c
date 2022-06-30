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


int regMemAddReg(uint8_t* text, int curr, int id)
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
        printf("add ");
    else if (id == 1)
        printf("adc ");
    else if (id == 2)
        printf("sub ");
    else
        printf("cmp");

    if (dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);

        if (word == 0x00)
            printf(", %s", regByte[reg]);
        else
            printf(", %s", regWord[reg]);
    }
    else
    {

        if (word == 0x00)
            printf("%s, ", regByte[reg]);
        else
            printf("%s, ", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }

    if (a->id == 9)
        printMemoryChange(a->disp);
    printf("\n");

    if (dir == 0x00)
    {
        if (word == 0x00)
        {
            if (a->id == 9)
            {
                if (id == 0) // ADD
                {
                    addOAC8(*(mem + a->disp), *getRegister8(reg));
                    *(mem + a->disp) += *getRegister8(reg);
                    setFlagsZAndS8(*(mem + a->disp));
                }
                /*
                else if (id == 1)
                    printf("adc ");*/
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*(mem + a->disp), *getRegister8(reg));
                    if (id == 2)
                    {
                        *(mem + a->disp) -= *getRegister8(reg);
                        setFlagsZAndS8(*(mem + a->disp));
                    }
                    else
                        setFlagsZAndS8(*(mem + a->disp) - *getRegister8(reg));
                }
            }
            else
            {
                if (id == 0) // ADD
                {
                    addOAC8(*getRegister8(a->id), *getRegister8(reg));
                    *getRegister8(a->id) += *getRegister8(reg);
                    setFlagsZAndS8(*getRegister8(a->id));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*getRegister8(a->id), *getRegister8(reg));
                    if (id == 2)
                    {
                        *getRegister8(a->id) -= *getRegister8(reg);
                        setFlagsZAndS8(*getRegister8(a->id));
                    }
                    else
                        setFlagsZAndS8(*getRegister8(a->id) - *getRegister8(reg));
                }
            }
        }
        else
        {
            if (a->id == 9)
            {
                if (id == 0) // ADD
                {
                    addOAC16(*((uint16_t*)(mem + a->disp)), *getRegister16(reg));
                    *((uint16_t*)(mem + a->disp)) += *getRegister16(reg);
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC16(*((uint16_t*)(mem + a->disp)), *getRegister16(reg));
                    if (id == 2)
                    {
                        *((uint16_t*)(mem + a->disp)) -= *getRegister16(reg);
                        setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                    }
                    else
                        setFlagsZAndS16(*((uint16_t*)(mem + a->disp)) - *getRegister16(reg));
                }
            }
            else
            {
                if (id == 0) // ADD
                {
                    addOAC16(*getRegister16(a->id), *getRegister16(reg));
                    *getRegister16(a->id) += *getRegister16(reg);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC16(*getRegister16(a->id), *getRegister16(reg));
                    if (id == 2)
                    {
                        *getRegister16(a->id) -= *getRegister16(reg);
                        setFlagsZAndS16(*getRegister16(a->id));
                    }
                    else
                        setFlagsZAndS16(*getRegister16(a->id) - *getRegister16(reg));
                }
            }
        }
    }
    else // dir == 1
    {
        if (word == 0x00)
        {
            if (a->id == 9)
            {
                if (id == 0) // ADD
                {
                    addOAC8(*getRegister8(reg), *(mem + a->disp));
                    *getRegister8(reg) += *(mem + a->disp);
                    setFlagsZAndS8(*getRegister8(reg));
                }

                /*
                else if (id == 1)
                    printf("adc ");*/
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*getRegister8(reg), *(mem + a->disp));
                    if (id == 2)
                    {
                        *getRegister8(reg) -= *(mem + a->disp);
                        setFlagsZAndS8(*getRegister8(reg));
                    }
                    else
                        setFlagsZAndS8(*getRegister8(reg) - *(mem + a->disp));
                }
            }
            else
            {
                if (id == 0) // ADD
                {
                    addOAC8(*getRegister8(reg), *getRegister8(a->id));
                    *getRegister8(reg) += *getRegister8(a->id);
                    setFlagsZAndS8(*getRegister8(reg));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*getRegister8(reg), *getRegister8(a->id));
                    if (id == 2)
                    {
                        *getRegister8(reg) -= *getRegister8(a->id);
                        setFlagsZAndS8(*getRegister8(reg));
                    }
                    else
                        setFlagsZAndS8(*getRegister8(reg) - *getRegister8(a->id));
                }
            }
        }
        else
        {
            if (a->id == 9)
            {
                if (id == 0) // ADD
                {
                    addOAC16(*getRegister16(reg), *((uint16_t*)(mem + a->disp)));
                    *getRegister16(reg) += *((uint16_t*)(mem + a->disp));
                    setFlagsZAndS16(*getRegister16(reg));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*getRegister16(reg), *((uint16_t*)(mem + a->disp)));
                    if (id == 2)
                    {
                        *getRegister16(reg) -= *((uint16_t*)(mem + a->disp));
                        setFlagsZAndS16(*getRegister16(reg));

                    }
                    else
                        setFlagsZAndS16(*getRegister16(reg) - *((uint16_t*)(mem + a->disp)));
                }
            }
            else
            {
                if (id == 0) // ADD
                {
                    addOAC16(*getRegister16(reg), *getRegister16(a->id));
                    *getRegister16(reg) += *getRegister16(a->id);
                    setFlagsZAndS16(*getRegister16(reg));
                }
                else // SUB AND CMP ---> sub id == 2
                {
                    cmpOAC16(*getRegister16(reg), *getRegister16(a->id));
                    if (id == 2)
                    {
                        *getRegister16(reg) -= *getRegister16(a->id);
                        setFlagsZAndS16(*getRegister16(reg));
                    }
                    else
                        setFlagsZAndS16(*getRegister16(reg) - *getRegister16(a->id));
                }
            }
        }
    }

    free(a);
    return read;
}

int immediateAddRegMem(uint8_t* text, int curr)
{
    uint8_t sw = text[curr] % 4;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    uint8_t id = (text[curr+1] % 64) / 8;
    int read;
    uint16_t data;
    int toRead = 3;
    if(sw == 0x01)
        toRead += 1;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = toRead + 2;

        if(sw == 0x01)
            data = text[curr+5] * 256 + text[curr+4];
        else if (sw = 0x03)
            data = (int8_t) text[curr+4];
        else
            data = text[curr+4];
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = toRead + 1;
        
        if(sw == 0x01)
            data = text[curr+4] * 256 + text[curr+3];
        else if (sw = 0x03)
            data = (int8_t) text[curr+3];
        else
            data = text[curr+3];
    }
    else
    {
        read = toRead;
        
        if(sw == 0x01)
            data = text[curr+3] * 256 + text[curr+2];
        else if (sw = 0x03)
            data = (int8_t) text[curr+2];
        else
            data = text[curr+2];
    }

    struct pair* a = getRmAddress(rm, mod, disp, sw % 2);
    printReadBytes(read, text, curr);
    if (id == 0x00)
        printf("add ");
    else if (id == 0x01)
        printf("or ");
    else if (id == 0x02)
        printf("adc ");
    else if (id == 0x04)
        printf("and ");
    else if (id == 0x05)
        printf("sub ");
    else if (id == 0x06)
        printf("xor ");
    else if (id == 0x07)
        printf("cmp ");

    printRm(rm, mod, disp, sw % 2, 0x00);

    if (sw == 0x03)
    {
        if (data >= 0x8000)
            printf(", -%04x", ~data + 1);
        else
            if(id == 0x05)
                printf(", %x", data);
            else
                printf(", %04x", data);
    }
    else
        printf(", %04x", data);

    if (a->id == 9)
        printMemoryChange(a->disp);
    printf("\n");

    if (sw % 2 == 0x00)
    {
        if (a->id == 9)
        {
            if (id == 0x00) // ADD
            {
                addOAC8(*(mem + a->disp), (uint8_t) data);
                *(mem + a->disp) += (uint8_t) data;
                setFlagsZAndS8(*(mem + a->disp));
            }
            else if (0x05 || 0x07) // SUB AND CMP ---> sub id == 5
            {
                cmpOAC8(*(mem + a->disp), (uint8_t)data);
                if (id == 0x05)
                {
                    *(mem + a->disp) -= (uint8_t)data;
                    setFlagsZAndS8(*(mem + a->disp));
                }
                else
                    setFlagsZAndS8(*(mem + a->disp) - data);
            }
        }
        else
        {
            if (id == 0x00) // ADD
            {
                addOAC8(*getRegister8(a->id), (uint8_t) data);
                *getRegister8(a->id) += (uint8_t)data;
                setFlagsZAndS8(*getRegister8(a->id));
            }
            else if (0x05 || 0x07)// SUB AND CMP ---> sub id == 2
            {
                cmpOAC8(*getRegister8(a->id), (uint8_t) data);
                if (id == 0x05)
                {
                    *getRegister8(a->id) -= (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(a->id));
                }
                else
                    setFlagsZAndS8(*getRegister8(a->id) - data);
            }
        }
    }
    else
    {
        if (a->id == 9)
        {
            if (id == 0x00) // ADD
            {
                addOAC16(*((uint16_t*)(mem + a->disp)), data);
                *((uint16_t*)(mem + a->disp)) += data;
                setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
            }
            else if (0x05 || 0x07)// SUB AND CMP ---> sub id == 2
            {
                cmpOAC16(*((uint16_t*)(mem + a->disp)), data);
                if (id == 0x05)
                {
                    *((uint16_t*)(mem + a->disp)) -= data;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)) - data);
            }
        }
        else
        {
            if (id == 0x00) // ADD
            {
                addOAC16(*getRegister16(a->id), data);
                *getRegister16(a->id) += data;
                setFlagsZAndS16(*getRegister16(a->id));
            }
            else if (0x05 || 0x07)// SUB AND CMP ---> sub id == 2
            {
                cmpOAC16(*getRegister16(a->id), data);
                if (id == 0x05)
                {
                    *getRegister16(a->id) -= data;
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else
                    setFlagsZAndS16(*getRegister16(a->id) - data);
            }
        }
    }

    free(a);
    return read;
}

int immediateToAccu(uint8_t* text, int curr, int id)
{
    uint8_t word = text[curr] % 2;
    uint16_t data;
    int read;

    if(word == 0x00)
    {
        read = 2;
        data = text[curr+1];
        printReadBytes(2, text, curr);
    }
    else
    {
        read = 3;
        data = text[curr+2] * 256 + text[curr+1];
        printReadBytes(3, text, curr);
    }

    if(id == 0)
        printf("add ");
    else if (id == 1)
        printf("adc ");
    else if (id == 2)
        printf("sub ");
    else if (id == 3)
        printf("cmp ");
    else if (id == 4)
        printf("and ");
    else if (id == 5)
        printf("or ");
    else if (id == 6)
        printf("xor ");
    else
        printf("test ");

    if(word == 0x00)
        printf("%s, %x\n", regByte[0], data);
    else
        printf("%s, %x\n", regWord[0], data);

    return read;
}

int incRegMem(uint8_t* text, int curr)
{
    uint8_t word = text[curr] % 2;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint8_t id = (text[curr+1] % 64) / 8;
    uint16_t disp = 0x0000;
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
        printf("inc ");
    else if(id == 0x01)
        printf("dec ");
    else if(id == 0x02 || 0x03)
        printf("call ");
    else if(id == 0x04 || id == 0x05)
        printf("jmp ");

    printRm(rm, mod, disp, word, 0x00);
    printf("\n");
    return read;
}

int incReg(uint8_t* text, int curr, int id)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);

    if(id == 0)
        printf("inc %s\n", regWord[reg]);
    else
        printf("dec %s\n", regWord[reg]);
    return 1;
}

int aaaBaa(uint8_t* text, int curr, int id)
{
    int read = 1;
    if(id == 4)
        read = 2;
    printReadBytes(1, text, curr);

    if(id == 0)
        printf("aaa\n");
    else if (id == 1)
        printf("baa\n");
    else if (id == 2)
        printf("aas\n");
    else if (id == 3)
        printf("das\n");
    else if (id == 4)
        printf("aad\n");
    else if (id == 5)
        printf("cbw\n");
    else
        printf("cwd\n");

    return read;
}

int negMul(uint8_t* text, int curr)
{
    uint8_t word = text[curr] % 2;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint8_t id = (text[curr+1] % 64) / 8;
    uint16_t disp = 0x0000;
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

    if(id == 0x03)
        printf("neg ");
    else if(id == 0x04)
        printf("mul ");
    else if(id == 0x05)
        printf("imul ");
    else if(id == 0x02)
        printf("not ");

    printRm(rm, mod, disp, word, 0x00);
    printf("\n");
    return read;
}
