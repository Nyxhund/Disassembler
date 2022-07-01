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
#include "syscall.h"


int controlDirect(uint8_t* text, int curr, int id)
{
    uint16_t disp = text[curr+2] * 256 + text[curr+1];
    printReadBytes(3, text, curr);
    int read;

    if (id == 0)
        fprintf(stderr, "call ");
    else if (id == 1)
        fprintf(stderr, "jmp ");
    else if (id == 2)
        fprintf(stderr, "jmp short ");
    else if (id == 3)
        fprintf(stderr, "ret ");

    fprintf(stderr, "%04x\n", (uint16_t) (disp + curr + 0x03));

    if (id == 0)
    {
        setRegister16(0x04, *getRegister16(0x04) - 2);
        *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t)curr;
        read = disp + 0x03;
    }
    else if (id == 1)
        read = disp + 0x03;
    else if (id == 2)
        read = 3;
    else if (id == 3)
        read = 3;

    return read;
}

int jumpShort(uint8_t* text, int curr)
{
    int8_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    fprintf(stderr, "jmp short %04x\n", disp + curr + 2);
    return 2;
}

int controlSimpleCommands(uint8_t* text, int curr, int id)
{
    printReadBytes(1, text, curr);
    int read = 1;

    switch (id)
    {
        case 0:
            fprintf(stderr, "ret\n");
            read = *((uint16_t*)(mem + *getRegister16(0x04))) + 0x03 - curr;
            setRegister16(0x04, *getRegister16(0x04) + 2);
            break;
        case 1:
            fprintf(stderr, "int\n");
            break;
        case 2:
            fprintf(stderr, "into\n");
            break;
        case 3:
            fprintf(stderr, "iret\n");
            break;
        case 4:
            fprintf(stderr, "clc\n");
            break;
        case 5:
            fprintf(stderr, "cmc\n");
            break;
        case 6:
            fprintf(stderr, "stc\n");
            break;
        case 7:
            fprintf(stderr, "cld\n");
            break;
        case 8:
            fprintf(stderr, "std\n");
            break;
        case 9:
            fprintf(stderr, "cli\n");
            break;
        case 10:
            fprintf(stderr, "sti\n");
            break;
        case 11:
            fprintf(stderr, "hlt\n");
            break;
        case 12:
            fprintf(stderr, "wait\n");
            break;
        case 13:
            fprintf(stderr, "lock\n");
            break;
    }

    return read;
}

int conditionalJump(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 16;
    uint16_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    int read = 2;
    switch (id)
    {
        case 0x00:
            fprintf(stderr, "jo ");
            if (cpu->O)
                read = disp + 0x02;
            break;
        case 0x01:
            fprintf(stderr, "jno ");
            if (!cpu->O)
                read = disp + 0x02;
            break;
        case 0x02:
            fprintf(stderr, "jb ");
            if (cpu->C)
                read = disp + 0x02;
            break;
        case 0x03:
            fprintf(stderr, "jnb ");
            if (!cpu->C)
                read = disp + 0x02;
            break;
        case 0x04:
            fprintf(stderr, "je ");
            if (cpu->Z)
                read = disp + 0x02;
            break;
        case 0x05:
            fprintf(stderr, "jne ");
            if (!cpu->Z)
                read = disp + 0x02;
            break;
        case 0x06:
            fprintf(stderr, "jbe ");
            if (cpu->C || cpu->Z)
                read = disp + 0x02;
            break;
        case 0x07:
            fprintf(stderr, "jnbe ");
            if (!cpu->C && !cpu->Z)
                read = disp + 0x02;
            break;
        case 0x08:
            fprintf(stderr, "js ");
            if (cpu->S)
                read = disp + 0x02;
            break;
        case 0x09:
            fprintf(stderr, "jns ");
            if (!cpu->S)
                read = disp + 0x02;
            break;
        case 0x0a:
            fprintf(stderr, "jp ");
            break;
        case 0x0b:
            fprintf(stderr, "jnp ");
            break;
        case 0x0c:
            fprintf(stderr, "jl ");
            if (cpu->S != cpu->O)
                read = disp + 0x02;
            break;
        case 0x0d:
            fprintf(stderr, "jnl ");
            if (cpu->S == cpu->O)
                read = disp + 0x02;
            break;
        case 0x0e:
            fprintf(stderr, "jle ");
            if ((cpu->S != cpu->O) || cpu->Z)
                read = disp + 0x02;
            break;
        case 0x0f:
            fprintf(stderr, "jnbe ");
            if (!cpu->C && !cpu->Z)
                read = disp + 0x02;
            break;
    }

    fprintf(stderr, "%04x\n", 0x02 + disp + curr);
    return read;
}

int conditionalLoop(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 4;
    uint16_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    switch (id)
    {
        case 0x00:
            fprintf(stderr, "loopnz ");
            break;
        case 0x01:
            fprintf(stderr, "loopz ");
            break;
        case 0x02:
            fprintf(stderr, "loop ");
            break;
        case 0x03:
            fprintf(stderr, "jcxz ");
            break;
    }

    fprintf(stderr, "%04x\n", 0x02 + disp + curr);
    return 2;
}

int intTypeSpec(uint8_t* text, int curr)
{
    uint8_t type = text[curr+1];
    printReadBytes(2, text, curr);
    fprintf(stderr, "int %x\n", type);

    syscall();

    return 2;
}

int escape(uint8_t* text, int curr)
{
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
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

    fprintf(stderr, "esc ");
    printRm(rm, mod, disp, 0x01, 0x00);
    return read;
}