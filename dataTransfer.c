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
#include "dataTransfer.h"
#include "utility.h"

int RegMemtofromReg(uint8_t* text, int curr, uint8_t dir, uint8_t word)
{
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
    

    struct pair *a = getRmAddress(rm, mod, disp, word);
    if(dir == 0x00) // INTERPRETOR
    {
        if(a->id == 0x09)
        {
            if(word == 0x00)
                mem[a->disp] = *getRegister8(reg);
            else
                mem[a->disp] = *getRegister16(reg);
        }
        else
        {
            if(word == 0x00)
                setRegister8(a->id, *getRegister8(reg));
            else
                setRegister16(a->id, *getRegister16(reg));
        }

    }
    else
    {
        if(a->id == 0x09)
        {
            if(word == 0x00)
                setRegister8(reg, mem[a->disp]);
            else
                setRegister16(reg, mem[a->disp]);
        }
        else
        {
            if(word == 0x00)
                setRegister8(reg, *getRegister8(a->id));
            else
                setRegister16(reg, *getRegister16(a->id));
        }
    }

    printReadBytes(read, text, curr);
    printf("mov ");

    if(dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);
    
        if(word == 0x00)
            printf(", %s", regByte[reg]);
        else
            printf(", %s", regWord[reg]);
    }
    else
    {
        if(word == 0x00)
            printf("%s, ", regByte[reg]);
        else
            printf("%s, ", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp);
    free(a);
    printf("\n");

    return read;
}

int immediateToRegMem(uint8_t* text, int curr, uint8_t word)
{
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    int read;
    uint16_t data;
    int toRead = 3;
    if(word == 0x01)
        toRead += 1;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = toRead + 2;
        
        if(word == 0x00)
            data = text[curr+4];
        else
            data = text[curr+5] * 256 + text[curr+4];
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = toRead + 1;
        
        if(word == 0x00)
            data = text[curr+3];
        else
            data = text[curr+4] * 256 + text[curr+3];
    }
    else
    {
        read = toRead;
        
        if(word == 0x00)
            data = text[curr+2];
        else
            data = text[curr+3] * 256 + text[curr+2];
    }


    struct pair* a = getRmAddress(rm, mod, disp, word);
    if(a->id == 9) // INTERPRETER
    { 
        if(word == 0x00)
            mem[a->disp] = (uint8_t) data;
        else
            mem[a->disp] = data;
    }
    else
    {
        if(word == 0x00)
            setRegister8(a->id, (uint8_t) data);
        else
            setRegister16(a->id, data);
    }
    free(a);
    
    printReadBytes(read, text, curr);
    printf("mov ");

    printRm(rm, mod, disp, word, 0x00);
    
    printf(", %x\n", data);

    return read;
}

void immediateToRegister(uint8_t* text, int curr)
{
    uint8_t w = (text[curr] % 16) / 8;
    uint8_t reg = text[curr] % 8;


    if (w == 0x00)
        setRegister8(reg, text[curr + 1]);
    else
    {
        uint16_t data = text[curr + 2] * 256 + text[curr + 1];
        setRegister16(reg, data);
    }
    

    if(w == 0x00)
    {
        printReadBytes(2, text, curr);
        printf("mov %s, %02x\n", regByte[reg], text[curr+1]);
    }
    else
    {
        printReadBytes(3, text, curr);
        printf("mov %s, %02x%02x\n", regWord[reg], text[curr+2], text[curr+1]);
    }
}

int memoryToFromAccu(uint8_t* text, int curr, uint8_t word, uint8_t dir) // I DONT KNOW REALLY??
{
    int read;
    read = 3;
    /*
    if (dir == 0x00)
    {
        if (word == 0x00)
            //setRegister8(0x00, )
            //printf("%s, %02x -> %02x\n", regByte[0], text[curr + 1], text[curr + 2]);
        else
            //printf("%s, %02x -> %02x\n", regWord[0], text[curr + 1], text[curr + 2]);
    }
    else
    {
        if (word == 0x00)
            //printf("%02x -> %02x, %s\n", text[curr + 1], text[curr + 2], regByte[0]);
        else
            //printf("%02x -> %02x, %s\n", text[curr + 1], text[curr + 2], regWord[0]);
    }*/

    printReadBytes(3, text, curr);
    printf("mov ");
    
    if(dir == 0x00)
    {
        if(word == 0x00)
            printf("%s, %02x -> %02x\n", regByte[0], text[curr+1], text[curr+2]);
        else
            printf("%s, %02x -> %02x\n", regWord[0], text[curr+1], text[curr+2]);
    }
    else
    {
        if(word == 0x00)
            printf("%02x -> %02x, %s\n", text[curr+1], text[curr+2], regByte[0]);
        else 
            printf("%02x -> %02x, %s\n", text[curr+1], text[curr+2], regWord[0]);
    }

    return read;
}

int regMemToFromSeg(uint8_t* text, int curr, uint8_t dir) // NOT TO DO ?? We'll see
{
    uint8_t mod = text[curr+1] / 64; 
    uint8_t rm = text[curr+1] % 8;
    uint8_t reg = (text[curr+1] % 16) / 8;
    uint16_t disp = 0x0000;
    uint8_t word;
    int read;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        printReadBytes(4, text, curr);
        disp = text[curr+3] * 256 + text[curr+2];
        read = 4;
        word = 0x01;
    }
    else if (mod == 0x01)
    {
        printReadBytes(3, text, curr);
        disp = (int8_t) text[curr+2];
        read = 3;
        word = 0x01;
    }
    else
    {
        printReadBytes(2, text, curr);
        read = 2;
        word = 0x00;
    }
    
    printf("mov ");
    
    if(dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x01);
        printf(", %s\n", regSegment[reg]);
    }
    else
    {
        printf("%s, ", regSegment[reg]);
        printRm(rm, mod, disp, word, 0x01);
        printf("\n");
    }

    return read;
}

int pushPopRegMem(uint8_t* text, int curr, int pop)
{
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    uint8_t word;
    int read;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = 4;
        word = 0x01;
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = 3;
        word = 0x01;
    }
    else
    {
        read = 2;
        word = 0x00;
    }
   

    struct pair* a = getRmAddress(rm, mod, disp, word);
    if (pop == 0) // PUSH
    {
        if (word == 0x00)
        {
            setRegister16(0x04, *getRegister16(0x04) - 1);
            if (a->id == 9)
                mem[*getRegister16(0x04)] = mem[a->disp];
            else
                mem[*getRegister16(0x04)] = *getRegister8(a->id);
        }
        else
        {
            setRegister16(0x04, *getRegister16(0x04) - 2);
            if (a->id == 9)
            {
                mem[*getRegister16(0x04)] = mem[a->disp];
                mem[*getRegister16(0x04) + 1] = mem[a->disp + 1];
            }
            else
                mem[*getRegister16(0x04)] = *getRegister16(a->id);
        }
    }
    else
    {
        if (word == 0x00)
        {
            if (a->id == 9)
                mem[a->disp] = mem[*getRegister16(0x04)];
            else
                setRegister8(a->id, mem[*getRegister16(0x04)]);

            setRegister16(0x04, *getRegister16(0x04) + 1);
        }
        else
        {
            if (a->id == 9)
            {
                mem[a->disp] = mem[*getRegister16(0x04)];
                mem[a->disp + 1] = mem[*getRegister16(0x04) + 1];
            }
            else
                setRegister16(a->id, mem[*getRegister16(0x04)]); // CURSED DE FOU MDR ALED

            setRegister16(0x04, *getRegister16(0x04) + 2);
        }
    }
    free(a);

    printReadBytes(read, text, curr);
    if(pop == 0)
        printf("push ");
    else
        printf("pop ");

    printRm(rm, mod, disp, word, 0x00);
    printf("\n");
    return read;
}

int pushPopReg(uint8_t* text, int curr, int pop)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);
    if(pop == 0)
        printf("push %s\n", regWord[reg]);
    else 
        printf("pop %s\n", regWord[reg]);
    return 1;
}

int pushPopSeg(uint8_t* text, int curr, int pop)
{
    uint8_t reg = (text[curr] % 16) / 8;
    printReadBytes(1, text, curr);
    if(pop == 0)
        printf("push %s\n", regSegment[reg]);
    else      
        printf("pop %s\n", regSegment[reg]);
    return 1;
}

int xchgRegMemWReg(uint8_t* text, int curr)
{
    uint8_t reg = (text[curr+1] % 64) / 8;
    uint8_t mod = text[curr+1] / 64;
    uint8_t rm = text[curr+1] % 8;
    uint16_t disp = 0x0000;
    uint8_t word = text[curr] % 2;
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
    
    printf("xchg ");

    printRm(rm, mod, disp, word, 0x00);
    
    if(word == 0x00)
        printf(", %s\n", regByte[reg]);
    else
        printf(", %s\n", regWord[reg]);

    return read;
}

int xchgRegAccu(uint8_t* text, int curr)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);
    printf("xchg %s, %s\n", regWord[reg], regWord[0]);
    return 1;
}

// NOT SURE ABOUT THE FORMAT

int inOutFromTo(uint8_t* text, int curr, int port, int out)
{
    uint8_t word = text[curr] % 2;
    int read = 1;

    if(port == 1)
    {
        printReadBytes(2, text, curr);
        read = 2;
    }
    else
        printReadBytes(1, text, curr);

    if(out == 0)
        printf("in ");
    else
        printf("out ");

    if(port == 1)
    {
        if(word == 0x00)
            printf("%s, %x", regByte[0], text[curr+1]);
        else
            printf("%s, %x", regWord[0], text[curr+1]);
    }
    printf("\n");
    return read;
}

// XLAT NOT DONE

int leaLdsLes(uint8_t* text, int curr, int id)
{
    uint8_t mod = text[curr+1] / 64; 
    uint8_t rm = text[curr+1] % 8;
    uint8_t reg = (text[curr+1] % 16) / 8;
    uint16_t disp = 0x0000;
    uint8_t word;
    int read;

    if((mod == 0x00 && rm == 0x06) || mod == 0x02)
    {
        disp = text[curr+3] * 256 + text[curr+2];
        read = 4;
        word = 0x01;
    }
    else if (mod == 0x01)
    {
        disp = (int8_t) text[curr+2];
        read = 3;
        word = 0x01;
    }
    else
    {
        read = 2;
        word = 0x00;
    }
    
    struct pair *a = getRmAddress(rm, mod, disp, word);
    if (word == 0x00)
    {
        if (id == 0)
            setRegister16(reg, *((uint16_t*)(mem + a->disp)));
            //printf("lea ");
        //else if (id == 1)
            //printf("lds ");
        //else
            //printf("les ");
    }
    else
    {
        if (id == 0)
            setRegister16(reg, *((uint16_t*)(mem + a->disp)));
        //printf("lea ");
        //else if (id == 1)
            //printf("lds ");
        //else
            //printf("les ");s
    }

    printReadBytes(read, text, curr);
    if(id == 0)
        printf("lea ");
    else if(id == 1)
        printf("lds ");
    else
        printf("les ");

    printf("%s, ", regWord[reg]);
    printRm(rm, mod, disp, word, 0x00);
    
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp);
    free(a);
    printf("\n");

    return read;
}
