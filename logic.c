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
        printf("rol ");
    else if (id == 0x01)
        printf("ror ");
    else if (id == 0x02)
        printf("rcl ");
    else if (id == 0x03)
        printf("rcr ");
    else if (id == 0x04)
        printf("shl ");
    else if (id == 0x05)
        printf("shr ");
    else if (id == 0x07)
        printf("sar ");

    printRm(rm, mod, disp, w, 0x00);
    
    if(v == 0x00)
        printf(", 1\n");
    else
        printf(", cl\n");

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

    if(id == 0)
        printf("and ");
    else if (id == 1)
        printf("or ");
    else if (id == 2)
        printf("xor ");
    else
        printf("test ");

    if(dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);

        if(word == 0x00)
            printf(", %s\n", regByte[reg]);
        else
            printf(", %s\n", regWord[reg]);
    }
    else
    {
        if(word == 0x00)
            printf(", %s\n", regByte[reg]);
        else
            printf(", %s\n", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
        printf("\n");
    }

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
        printReadBytes(toRead + 2, text, curr);
        disp = text[curr+3] * 256 + text[curr+2];
        read = toRead + 2;

        if(w == 0x01)
            data = text[curr+5] * 256 + text[curr+4];
        else
            data = text[curr+4];
    }
    else if (mod == 0x01)
    {
        printReadBytes(toRead + 1, text, curr);
        disp = (int8_t) text[curr+2];
        read = toRead + 1;

        if(w == 0x01)
            data = text[curr+4] * 256 + text[curr+3];
        else
            data = text[curr+3];
    }
    else
    {
        printReadBytes(toRead, text, curr);
        read = toRead;

        if(w == 0x01)
            data = text[curr+3] * 256 + text[curr+2];
        else
            data = text[curr+2];
    }
    
    printf("test ");

    printRm(rm, mod, disp, w, 0x00);

    printf(", %x\n", data);
    
    return read;
}

int stringManipulation(uint8_t* text, int curr, int id)
{
    uint8_t w = text[curr] % 2;
    printReadBytes(1, text, curr);

    switch (id)
    {
        case 0:
            printf("rep\n");
            break;
        case 1:
            printf("movs\n");
            break;
        case 2:
            printf("cmps\n");
            break;
        case 3:
            printf("scas\n");
            break;
        case 4:
            printf("lods\n");
            break;
        case 5:
            printf("stos\n");
            break;
    }
    return 1;
}
