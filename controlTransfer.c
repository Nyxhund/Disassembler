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
    int read = 3;

    if (id == 0)
        fprintf(fd, "call ");
    else if (id == 1)
        fprintf(fd, "jmp ");
    else if (id == 2)
        fprintf(fd, "jmp short ");
    else if (id == 3)
        fprintf(fd, "ret ");

    if(id != 3)
        fprintf(fd, "%04x\n", (uint16_t) (disp + curr + 0x03));
    else
        fprintf(fd, "%04x\n", disp);

    if (interpret) {
        if (id == 0)
        {
            setRegister16(0x04, *getRegister16(0x04) - 2);
            *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t) curr + 3;
            read = disp + 0x03;
        }
        else if (id == 1)
            read = disp + 0x03;
        else if (id == 2)
            read = 3;
        else if (id == 3)
        {         
            read = *((uint16_t*)(mem + *getRegister16(0x04))) - curr;
            setRegister16(0x04, *getRegister16(0x04) + 2 + disp);
        }
    }

    return read;
}

int controlJumpShort(uint8_t* text, int curr)
{
    int8_t disp = text[curr+1];
    printReadBytes(2, text, curr);
    int read = 2;

    fprintf(fd, "jmp short %04x\n", disp + curr + 2);
    if (interpret)
        read = disp + 2;
    return read;
}

int controlSimpleCommands(uint8_t* text, int curr, int id)
{
    printReadBytes(1, text, curr);
    int read = 1;

    switch (id)
    {
        case 0:
            fprintf(fd, "ret\n");
            break;
        case 1:
            fprintf(fd, "int\n");
            break;
        case 2:
            fprintf(fd, "into\n");
            break;
        case 3:
            fprintf(fd, "iret\n");
            break;
        case 4:
            fprintf(fd, "clc\n");
            break;
        case 5:
            fprintf(fd, "cmc\n");
            break;
        case 6:
            fprintf(fd, "stc\n");
            break;
        case 7:
            fprintf(fd, "cld\n");
            break;
        case 8:
            fprintf(fd, "std\n");
            break;
        case 9:
            fprintf(fd, "cli\n");
            break;
        case 10:
            fprintf(fd, "sti\n");
            break;
        case 11:
            fprintf(fd, "hlt\n");
            break;
        case 12:
            fprintf(fd, "wait\n");
            break;
        case 13:
            fprintf(fd, "lock\n");
            break;
    }

    if (interpret) {
        switch (id)
        {
        case 0:
            //fprintf(fd, "ret\n");
            read = *((uint16_t*)(mem + *getRegister16(0x04))) - curr;
            setRegister16(0x04, *getRegister16(0x04) + 2);
            break;
        case 1:
            //fprintf(fd, "int\n");
            break;
        case 2:
            //fprintf(fd, "into\n");
            break;
        case 3:
            //fprintf(fd, "iret\n");
            break;
        case 4:
            //fprintf(fd, "clc\n");
            break;
        case 5:
            //fprintf(fd, "cmc\n");
            break;
        case 6:
            //fprintf(fd, "stc\n");
            break;
        case 7:
            //fprintf(fd, "cld\n");
            break;
        case 8:
            //fprintf(fd, "std\n");
            break;
        case 9:
            //fprintf(fd, "cli\n");
            break;
        case 10:
            //fprintf(fd, "sti\n");
            break;
        case 11:
            //fprintf(fd, "hlt\n");
            break;
        case 12:
            //fprintf(fd, "wait\n");
            break;
        case 13:
            //fprintf(fd, "lock\n");
            break;
        }
    }

    return read;
}

int controlConditionalJump(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 16;
    uint16_t disp = (int8_t) text[curr+1];
    printReadBytes(2, text, curr);

    int read = 2;
    switch (id)
    {
        case 0x00:
            fprintf(fd, "jo ");
            break;
        case 0x01:
            fprintf(fd, "jno ");
            break;
        case 0x02:
            fprintf(fd, "jb ");
            break;
        case 0x03:
            fprintf(fd, "jnb ");
            break;
        case 0x04:
            fprintf(fd, "je ");
            break;
        case 0x05:
            fprintf(fd, "jne ");
            break;
        case 0x06:
            fprintf(fd, "jbe ");
            break;
        case 0x07:
            fprintf(fd, "jnbe ");
            break;
        case 0x08:
            fprintf(fd, "js ");
            break;
        case 0x09:
            fprintf(fd, "jns ");
            break;
        case 0x0a:
            fprintf(fd, "jp ");
            break;
        case 0x0b:
            fprintf(fd, "jnp ");
            break;
        case 0x0c:
            fprintf(fd, "jl ");
            break;
        case 0x0d:
            fprintf(fd, "jnl ");
            break;
        case 0x0e:
            fprintf(fd, "jle ");
            break;
        case 0x0f:
            fprintf(fd, "jnle ");
            break;
    }


    fprintf(fd, "%04x\n", (0x02 + disp + curr) % 0x10000);

    if (interpret)
    {
        switch (id)
        {
        case 0x00:
            //fprintf(fd, "jo ");
            if (cpu->O)
                read = disp + 0x02;
            break;
        case 0x01:
            //fprintf(fd, "jno ");
            if (!cpu->O)
                read = disp + 0x02;
            break;
        case 0x02:
            //fprintf(fd, "jb ");
            if (cpu->C)
                read = disp + 0x02;
            break;
        case 0x03:
            //fprintf(fd, "jnb ");
            if (!cpu->C)
                read = disp + 0x02;
            break;
        case 0x04:
            //fprintf(fd, "je ");
            if (cpu->Z)
                read = disp + 0x02;
            break;
        case 0x05:
            //fprintf(fd, "jne ");
            if (!cpu->Z)
                read = disp + 0x02;
            break;
        case 0x06:
            //fprintf(fd, "jbe ");
            if (cpu->C || cpu->Z)
                read = disp + 0x02;
            break;
        case 0x07:
            //fprintf(fd, "jnbe ");
            if (!cpu->C && !cpu->Z)
                read = disp + 0x02;
            break;
        case 0x08:
            //fprintf(fd, "js ");
            if (cpu->S)
                read = disp + 0x02;
            break;
        case 0x09:
            //fprintf(fd, "jns ");
            if (!cpu->S)
                read = disp + 0x02;
            break;
        case 0x0a:
            //fprintf(fd, "jp ");
            break;
        case 0x0b:
            //fprintf(fd, "jnp ");
            break;
        case 0x0c:
            //fprintf(fd, "jl ");
            if (cpu->S != cpu->O)
                read = disp + 0x02;
            break;
        case 0x0d:
            //fprintf(fd, "jnl ");
            if (cpu->S == cpu->O)
                read = disp + 0x02;
            break;
        case 0x0e:
            //fprintf(fd, "jle ");
            if ((cpu->S != cpu->O) || cpu->Z)
                read = disp + 0x02;
            break;
        case 0x0f:
            //fprintf(fd, "jnle ");
            if (!cpu->S && !cpu->Z)
                read = disp + 0x02;
            break;
        }
    }
    return read;
}

int controlConditionalLoop(uint8_t* text, int curr)
{
    uint8_t id = text[curr] % 4;
    uint16_t disp = (int8_t) text[curr+1];
    printReadBytes(2, text, curr);

    switch (id)
    {
        case 0x00:
            fprintf(fd, "loopnz ");
            break;
        case 0x01:
            fprintf(fd, "loopz ");
            break;
        case 0x02:
            fprintf(fd, "loop ");
            break;
        case 0x03:
            fprintf(fd, "jcxz ");
            break;
    }

    fprintf(fd, "%04x\n", (0x02 + disp + curr) % 0x10000);
    return 2;
}

int controlSyscall(uint8_t* text, int curr)
{
    uint8_t type = text[curr+1];
    printReadBytes(2, text, curr);
    fprintf(fd, "int %x\n", type);

    if(interpret)
        syscall();
    return 2;
}

int controlEscape(uint8_t* text, int curr)
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

    fprintf(fd, "esc ");
    printRm(rm, mod, disp, 0x01, 0x00);
    return read;
}