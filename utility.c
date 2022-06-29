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

uint16_t* getRegister16(uint8_t reg)
{
    return (uint16_t*)(cpu->registers + reg);
}

void setRegister16(uint8_t reg, uint16_t val)
{
    *getRegister16(reg) = val;
}

uint8_t* getRegister8(uint8_t reg)
{
    if (reg < 4)
        return (uint8_t*)(cpu->registers + reg);

    return (uint8_t*)(cpu->registers + reg - 4) + 1;
}

void setRegister8(uint8_t reg, uint8_t val)
{
    *getRegister8(reg) = val;
}


struct pair* getRmAddress(uint8_t rm, uint8_t mod, uint16_t disp, uint8_t word)
{
    struct pair *a = malloc(sizeof(struct pair));

    if(mod == 0x00 && rm == 0x06)
    {
        a->id = 0x09;
        a->disp = disp;
    }
    else if(mod == 0x03)
    {
        a->id = rm;
    }
    else
    {
        a->id = 9;
        switch(rm)
        {
            case 0x00:
                a->disp = *getRegister16(3) + *getRegister16(6);
                break;
            case 0x01:
                a->disp = *getRegister16(3) + *getRegister16(7);
                break;
            case 0x02:
                a->disp = *getRegister16(5) + *getRegister16(6);
                break;
            case 0x03:
                a->disp = *getRegister16(5) + *getRegister16(7);
                break;
            case 0x04:
                a->disp = *getRegister16(6);
                break;
            case 0x05:
                a->disp = *getRegister16(7);
                break;
            case 0x06:
                a->disp = *getRegister16(5);
                break;
            case 0x07:
                a->disp = *getRegister16(3);
                break;
        }

        if(mod == 0x01)
        {
            if(disp >= 0x8000)
                a->disp -= ~disp + 1;
            else
                a->disp += disp;
        }
    }

    return a;
}

void printRm(uint8_t rm, uint8_t mod, uint16_t disp, uint8_t word, uint8_t seg)
{
    if(mod == 0x00 && rm == 0x06)
    {
        printf("[%04x]", disp); // Check if negative? EA shouldn't be 0
    }
    else if(mod == 0x03)
    {
        if(seg == 0x01)
            printf("%s", regSegment[rm]);
        else if(word == 0x00)
            printf("%s", regByte[rm]);
        else if(word == 0x01)
            printf("%s", regWord[rm]);
    }
    else
    {
        switch(rm)
        {
            case 0x00:
                printf("[bx+si");
                break;
            case 0x01:
                printf("[bx+di");
                break;
            case 0x02:
                printf("[bp+si");
                break;
            case 0x03:
                printf("[bp+di");
                break;
            case 0x04:
                printf("[si");
                break;
            case 0x05:
                printf("[di");
                break;
            case 0x06:
                printf("[bp");
                break;
            case 0x07:
                printf("[bx");
                break;
        }

        if(mod == 0x00)
        {
            printf("]");
        }
        else
        {
            if(disp >= 0x8000)
            {
                disp = ~disp + 1; // 2's complement
                printf("-%x]", disp);
            }
            else
            {
                printf("+%x]", disp);
            }
        }
    }
}

// Printing the read Bytes for current instruction
void printReadBytes(int read, uint8_t* text, int curr)
{
    for(int i = 0; i < read; i++)
    {
        printf("%02x", text[i + curr]);
    }
    for(int i = read; i < 7; i++)
    {
        printf("  ");
    }
}

void printRegisters(int curr)
{
    printf("%04x %04x %04x %04x %04x %04x %04x %04x ", cpu->registers[0], cpu->registers[3], cpu->registers[2], cpu->registers[1], cpu->registers[4], cpu->registers[5], cpu->registers[6], cpu->registers[7]);
    
    if(cpu->O) //  OsZc
        printf("O");
    else
        printf("-");
    
    if(cpu->S) //  OsZc
        printf("S");
    else
        printf("-");

    if(cpu->Z) //  OsZc
        printf("Z");
    else
        printf("-");

    if(cpu->C) //  OsZc
        printf("O");
    else
        printf("-");

    printf(" %04x:", curr);
}

void setFlagsZAndS8(uint8_t val)
{
    if (val == 0x00)
        cpu->Z = 1;
    else
        cpu->Z = 0;

    if (val >= 0x80)
        cpu->S = 1;
    else
        cpu->S = 0;
}

void setFlagsZAndS16(uint16_t val)
{
    if (val == 0x0000)
        cpu->Z = 1;
    else
        cpu->Z = 0;

    if (val >= 0x8000)
        cpu->S = 1;
    else
        cpu->S = 0;
}

void printMemoryChange(uint16_t addr)
{
    printf(" ;[%04x]%04x", addr, mem[addr]);
}