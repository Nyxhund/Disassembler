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

int dataTransferOpe(uint8_t* text, int curr, uint8_t dir, uint8_t word)
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
    printReadBytes(read, text, curr);
    fprintf(fd, "mov ");

    if (dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x00);

        if (word == 0x00)
            fprintf(fd, ", %s", regByte[reg]);
        else
            fprintf(fd, ", %s", regWord[reg]);
    }
    else
    {
        if (word == 0x00)
            fprintf(fd, "%s, ", regByte[reg]);
        else
            fprintf(fd, "%s, ", regWord[reg]);

        printRm(rm, mod, disp, word, 0x00);
    }

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (dir == 0x00)
        {
            if (a->id == 0x09)
            {
                if (word == 0x00)
                    mem[a->disp] = *getRegister8(reg);
                else
                    *((uint16_t*)(mem + a->disp)) = *getRegister16(reg);
            }
            else
            {
                if (word == 0x00)
                    setRegister8(a->id, *getRegister8(reg));
                else
                    setRegister16(a->id, *getRegister16(reg));
            }
        }
        else
        {
            if (a->id == 0x09)
            {
                if (word == 0x00)
                    setRegister8(reg, mem[a->disp]);
                else
                    setRegister16(reg, *((uint16_t*)(mem + a->disp)));
            }
            else
            {
                if (word == 0x00)
                    setRegister8(reg, *getRegister8(a->id));
                else
                    setRegister16(reg, *getRegister16(a->id));
            }
        }
    }

    free(a);
    return read;
}

int dataTransferOpeImmediate(uint8_t* text, int curr, uint8_t word)
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
    printReadBytes(read, text, curr);
    fprintf(fd, "mov ");

    if (word == 0x00)
    {
        fprintf(fd, "byte ");
    }

    printRm(rm, mod, disp, word, 0x00);

    if(word == 0x00)
        fprintf(fd, ", %x", data);
    else
        fprintf(fd, ", %04x", data);

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (a->id == 9)
        {
            if (word == 0x00)
                mem[a->disp] = (uint8_t)data;
            else
                *((uint16_t*)(mem + a->disp)) = data;
        }
        else
        {
            if (word == 0x00)
                setRegister8(a->id, (uint8_t)data);
            else
                setRegister16(a->id, data);
        }
    }
    free(a);
    return read;
}

void dataTransferImmediateRegister(uint8_t* text, int curr)
{
    uint8_t w = (text[curr] % 16) / 8;
    uint8_t reg = text[curr] % 8;

    if (w == 0x00)
    {
        printReadBytes(2, text, curr);
        fprintf(fd, "mov %s, %02x\n", regByte[reg], text[curr + 1]);
    }
    else
    {
        printReadBytes(3, text, curr);
        fprintf(fd, "mov %s, %02x%02x\n", regWord[reg], text[curr + 2], text[curr + 1]);
    }

    if (interpret)
    {
        if (w == 0x00)
            setRegister8(reg, text[curr + 1]);
        else
        {
            uint16_t data = text[curr + 2] * 256 + text[curr + 1];
            setRegister16(reg, data);
        }
    }
}

int dataTransferAccu(uint8_t* text, int curr, uint8_t word, uint8_t dir)
{
    // Never tested, function does not appear in 1-6.c files, so I'm not sure about the format of the print / functionning of the function
    int read;
    read = 3;
    printReadBytes(3, text, curr);
    fprintf(fd, "mov ");
    
    if(dir == 0x00)
    {
        if(word == 0x00)
            fprintf(fd, "%s, %02x -> %02x\n", regByte[0], text[curr+1], text[curr+2]);
        else
            fprintf(fd, "%s, %02x -> %02x\n", regWord[0], text[curr+1], text[curr+2]);
    }
    else
    {
        if(word == 0x00)
            fprintf(fd, "%02x -> %02x, %s\n", text[curr+1], text[curr+2], regByte[0]);
        else 
            fprintf(fd, "%02x -> %02x, %s\n", text[curr+1], text[curr+2], regWord[0]);
    }

    return read;
}

int dataTransferSeg(uint8_t* text, int curr, uint8_t dir)
{   
    // Never tested, function does not appear in 1-6.c files, so I'm not sure about the format of the print / functionning of the function
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
    
    fprintf(fd, "mov ");
    
    if(dir == 0x00)
    {
        printRm(rm, mod, disp, word, 0x01);
        fprintf(fd, ", %s\n", regSegment[reg]);
    }
    else
    {
        fprintf(fd, "%s, ", regSegment[reg]);
        printRm(rm, mod, disp, word, 0x01);
        fprintf(fd, "\n");
    }

    return read;
}

int dataTransferPushPopOpe(uint8_t* text, int curr, int pop)
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
    printReadBytes(read, text, curr);
    if (pop == 0)
        fprintf(fd, "push ");
    else
        fprintf(fd, "pop ");

    printRm(rm, mod, disp, word, 0x00);
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, 0x01);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (pop == 0) // PUSH
        {
            setRegister16(0x04, *getRegister16(0x04) - 2);
            if (a->id == 9 && interpret)
                *((uint16_t*)(mem + *getRegister16(0x04))) = *((uint16_t*)(mem + a->disp));
            else
                *((uint16_t*)(mem + *getRegister16(0x04))) = *getRegister16(a->id);
        }
        else
        {
            if (a->id == 9 && interpret)
                *((uint16_t*)(mem + a->disp)) = *((uint16_t*)(mem + *getRegister16(0x04)));
            else
                setRegister16(a->id, *((uint16_t*)(mem + *getRegister16(0x04))));
            setRegister16(0x04, *getRegister16(0x04) + 2);
        }
    }

    free(a);
    return read;
}

int dataTransferPushPopRegister(uint8_t* text, int curr, int pop)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);
    if(pop == 0)
        fprintf(fd, "push %s\n", regWord[reg]);
    else 
        fprintf(fd, "pop %s\n", regWord[reg]);

    if (interpret)
    {
        if (pop == 0)
        {
            setRegister16(0x04, *getRegister16(0x04) - 2);
            *((uint16_t*)(mem + *getRegister16(0x04))) = *getRegister16(reg);
        }
        else
        {
            setRegister16(reg, *((uint16_t*)(mem + *getRegister16(0x04))));
            setRegister16(0x04, *getRegister16(0x04) + 2);
        }
    }
    return 1;
}

int dataTransferPushPopSeg(uint8_t* text, int curr, int pop)
{
    // Never tested, function does not appear in 1-6.c files, so I'm not sure about the format of the print / functionning of the function
    uint8_t reg = (text[curr] % 16) / 8;
    printReadBytes(1, text, curr);
    if(pop == 0)
        fprintf(fd, "push %s\n", regSegment[reg]);
    else      
        fprintf(fd, "pop %s\n", regSegment[reg]);
    return 1;
}

int dataTransferXchg(uint8_t* text, int curr)
{
    // Never tested, function does not appear in 1-6.c files, so I'm not sure about the format of the print / functionning of the function
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
    
    fprintf(fd, "xchg ");

    printRm(rm, mod, disp, word, 0x00);
    
    if(word == 0x00)
        fprintf(fd, ", %s\n", regByte[reg]);
    else
        fprintf(fd, ", %s\n", regWord[reg]);

    return read;
}

int dataTransferXchgAccu(uint8_t* text, int curr)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);
    fprintf(fd, "xchg %s, %s\n", regWord[reg], regWord[0]);

    if(interpret)
    {
        uint16_t tmp = *getRegister16(0x00);
        setRegister16(0x00, *getRegister16(reg));
        setRegister16(reg, tmp);
    }
    return 1;
}

int dataTransferInOut(uint8_t* text, int curr, int port, int out)
{
    // Never tested, function does not appear in 1-6.c files, so I'm not sure about the format of the print / functionning of the function
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
        fprintf(fd, "in");
    else
        fprintf(fd, "out");
    
    if(port == 1)
    {
        if(word == 0x00)
            fprintf(fd, " %s, %x", regByte[0], text[curr+1]);
        else
            fprintf(fd, " %s, %x", regWord[0], text[curr+1]);
    }
    else
    {
        if (word == 0x00)
            fprintf(fd, " %s, dx", regByte[0]);
        else
            fprintf(fd, " %s, dx", regWord[0]);

    }
    fprintf(fd, "\n");
    return read;
}

int dataTransferLeaLdsLes(uint8_t* text, int curr, int id)
{
    uint8_t mod = text[curr+1] / 64; 
    uint8_t rm = text[curr+1] % 8;
    uint8_t reg = (text[curr+1] % 64) / 8;
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
    printReadBytes(read, text, curr);
    if (id == 0)
        fprintf(fd, "lea ");
    else if (id == 1)
        fprintf(fd, "lds ");
    else
        fprintf(fd, "les ");

    fprintf(fd, "%s, ", regWord[reg]);
    printRm(rm, mod, disp, word, 0x00);

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(fd, "\n");

    if (interpret)
    {
        if (word == 0x00)
        {
            if (id == 0) // LEA
                setRegister16(reg, a->disp);
            //else if (id == 1)
                //fprintf(fd, "lds ");
            //else
                //fprintf(fd, "les ");
        }
        else
        {
            if (id == 0) // LEA
                setRegister16(reg, a->disp);
            //else if (id == 1)
                //fprintf(fd, "lds ");
            //else
                //fprintf(fd, "les ");s
        }
    }

    free(a);
    return read;
}