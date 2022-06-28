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
