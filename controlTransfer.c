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


int controlDirect(uint8_t* text, int curr, int id)
{
    uint16_t disp = text[curr+2] * 256 + text[curr+1];
    printReadBytes(3, text, curr);

    if (id == 0)
        printf("call ");
    else if (id == 1)
        printf("jmp ");
    else if (id == 2)
        printf("jmp short ");
    else if (id == 3)
        printf("ret ");

    printf("%04x\n", disp + curr + 0x03);
    return 3;
}

int jumpShort(uint8_t* text, int curr)
{
    int8_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    printf("jmp short %04x\n", disp + curr + 2);
    return 2;
}

int controlSimpleCommands(uint8_t* text, int curr, int id)
{
    printReadBytes(1, text, curr);

    switch (id)
    {
        case 0:
            printf("ret\n");
            break;
        case 1:
            printf("int\n");
            break;
        case 2:
            printf("into\n");
            break;
        case 3:
            printf("iret\n");
            break;
        case 4:
            printf("clc\n");
            break;
        case 5:
            printf("cmc\n");
            break;
        case 6:
            printf("stc\n");
            break;
        case 7:
            printf("cld\n");
            break;
        case 8:
            printf("std\n");
            break;
        case 9:
            printf("cli\n");
            break;
        case 10:
            printf("sti\n");
            break;
        case 11:
            printf("hlt\n");
            break;
        case 12:
            printf("wait\n");
            break;
        case 13:
            printf("lock\n");
            break;
    }

    return 1;
}

int conditionalJump(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 16;
    uint16_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    switch (id)
    {
        case 0x00:
            printf("jo ");
            break;
        case 0x01:
            printf("jno ");
            break;
        case 0x02:
            printf("jb ");
            break;
        case 0x03:
            printf("jnb ");
            break;
        case 0x04:
            printf("je ");
            break;
        case 0x05:
            printf("jne ");
            break;
        case 0x06:
            printf("jbe ");
            break;
        case 0x07:
            printf("jnbe ");
            break;
        case 0x08:
            printf("js ");
            break;
        case 0x09:
            printf("jns ");
            break;
        case 0x0a:
            printf("jp ");
            break;
        case 0x0b:
            printf("jnp ");
            break;
        case 0x0c:
            printf("jl ");
            break;
        case 0x0d:
            printf("jnl ");
            break;
        case 0x0e:
            printf("jle ");
            break;
        case 0x0f:
            printf("jnbe ");
            break;
    }

    printf("%04x\n", 0x02 + disp + curr);
    return 2;
}

int conditionalLoop(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 4;
    uint16_t disp = text[curr+1];
    printReadBytes(2, text, curr);

    switch (id)
    {
        case 0x00:
            printf("loopnz ");
            break;
        case 0x01:
            printf("loopz ");
            break;
        case 0x02:
            printf("loop ");
            break;
        case 0x03:
            printf("jcxz ");
            break;
    }

    printf("%04x\n", 0x02 + disp + curr);
    return 2;
}

int intTypeSpec(uint8_t* text, int curr)
{
    uint8_t type = text[curr+1];
    printReadBytes(2, text, curr);
    printf("int %x\n", type);
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

    printf("esc ");
    printRm(rm, mod, disp, 0x01, 0x00);
    return read;
}
