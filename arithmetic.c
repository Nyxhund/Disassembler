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
        fprintf(stderr, "add ");
    else if (id == 1)
        fprintf(stderr, "adc ");
    else if (id == 2)
        fprintf(stderr, "sub ");
    else if (id == 3)
        fprintf(stderr, "sbb ");
    else
        fprintf(stderr, "cmp ");

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

    if (interpret) {
        if (dir == 0x00)
        {
            if (word == 0x00)
            {
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // ADD
                    {
                        addOAC8(*(mem + a->disp), *getRegister8(reg));
                        *(mem + a->disp) += *getRegister8(reg);
                        setFlagsZAndS8(*(mem + a->disp));
                    }
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
                if (a->id == 9 && interpret)
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
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // ADD
                    {
                        addOAC8(*getRegister8(reg), *(mem + a->disp));
                        *getRegister8(reg) += *(mem + a->disp);
                        setFlagsZAndS8(*getRegister8(reg));
                    }
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
                if (a->id == 9 && interpret)
                {
                    if (id == 0) // ADD
                    {
                        addOAC16(*getRegister16(reg), *((uint16_t*)(mem + a->disp)));
                        *getRegister16(reg) += *((uint16_t*)(mem + a->disp));
                        setFlagsZAndS16(*getRegister16(reg));
                    }
                    else // SUB AND CMP ---> sub id == 2
                    {
                        cmpOAC16(*getRegister16(reg), *((uint16_t*)(mem + a->disp)));
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
        else if (sw == 0x03)
            data = (int8_t) text[curr+3];
        else
            data = text[curr+3];
    }
    else
    {
        read = toRead;
        
        if(sw == 0x01)
            data = text[curr+3] * 256 + text[curr+2];
        else if (sw == 0x03)
            data = (int8_t) text[curr+2];
        else
            data = text[curr+2];
    }

    struct pair* a = getRmAddress(rm, mod, disp, sw % 2);
    printReadBytes(read, text, curr);
    if (id == 0x00)
        fprintf(stderr, "add ");
    else if (id == 0x01)
        fprintf(stderr, "or ");
    else if (id == 0x02)
        fprintf(stderr, "adc ");
    else if (id == 0x03)
        fprintf(stderr, "sbb ");
    else if (id == 0x04)
        fprintf(stderr, "and ");
    else if (id == 0x05)
        fprintf(stderr, "sub ");
    else if (id == 0x06)
        fprintf(stderr, "xor ");
    else if (id == 0x07)
        fprintf(stderr, "cmp ");

    if (sw == 0x00)
    {
        fprintf(stderr, "byte ");
    }
    printRm(rm, mod, disp, sw % 2, 0x00);


    if (sw % 2 == 0x01)
    {
        if (data >= 0x8000 && sw == 0x03)
            fprintf(stderr, ", -%x", (uint16_t) (~data) + 1);
        else
        {
            if ((id == 0x00 && sw == 0x03) || (id == 0x05 && sw == 0x03) || id == 0x03 || (id == 0x07 && sw == 0x03))
                fprintf(stderr, ", %x", data);
            else
                fprintf(stderr, ", %04x", data);
        }
    }
    else
    {
        if(id == 0x07)
            fprintf(stderr, ", %x", data);
        else
            fprintf(stderr, ", %04x", data);
    }

    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, sw % 2);
    fprintf(stderr, "\n");

    if (interpret) {
        if (sw % 2 == 0x00)
        {
            if (a->id == 9 && interpret)
            {
                if (id == 0x00) // ADD
                {
                    addOAC8(*(mem + a->disp), (uint8_t)data);
                    *(mem + a->disp) += (uint8_t)data;
                    setFlagsZAndS8(*(mem + a->disp));
                }
                else if (id == 0x05 || id == 0x07) // SUB AND CMP ---> sub id == 5
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
                else if (id == 0x04) // AND
                {
                    *(mem + a->disp) &= (uint8_t) data;
                    setFlagsZAndS8(*(mem + a->disp));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x01) // OR
                {
                    *(mem + a->disp) |= (uint8_t) data;
                    setFlagsZAndS8(*(mem + a->disp));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x06) // XOR
                {
                    *(mem + a->disp) ^= (uint8_t)data;
                    setFlagsZAndS8(*(mem + a->disp));
                    cpu->O = 0;
                    cpu->C = 0;
                }
            }
            else
            {
                if (id == 0x00) // ADD
                {
                    addOAC8(*getRegister8(a->id), (uint8_t)data);
                    *getRegister8(a->id) += (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(a->id));
                }
                else if (id == 0x05 || id == 0x07)// SUB AND CMP ---> sub id == 2
                {
                    cmpOAC8(*getRegister8(a->id), (uint8_t)data);
                    if (id == 0x05)
                    {
                        *getRegister8(a->id) -= (uint8_t)data;
                        setFlagsZAndS8(*getRegister8(a->id));
                    }
                    else
                        setFlagsZAndS8(*getRegister8(a->id) - data);
                }
                else if (id == 0x04) // AND
                {
                    *getRegister8(a->id) &= (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(a->id));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x01) // OR
                {
                    *getRegister8(a->id) |= (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(a->id));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x06) // XOR
                {
                    *getRegister8(a->id) ^= (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(a->id));
                    cpu->O = 0;
                    cpu->C = 0;
                }
            }
        }
        else
        {
            if (a->id == 9 && interpret)
            {
                if (id == 0x00) // ADD
                {
                    addOAC16(*((uint16_t*)(mem + a->disp)), data);
                    *((uint16_t*)(mem + a->disp)) += data;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else if (id == 0x05 || id == 0x07)// SUB AND CMP ---> sub id == 2
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
                else if (id == 0x04) // AND
                {
                    *((uint16_t*)(mem + a->disp)) &= data;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x01) // OR
                {
                    *((uint16_t*)(mem + a->disp)) |= data;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                    cpu->O = 0;
                    cpu->C = 0;
                }
                else if (id == 0x06) // XOR
                {
                    *((uint16_t*)(mem + a->disp)) ^= data;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                    cpu->O = 0;
                    cpu->C = 0;
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
                else if (id == 0x05 || id == 0x07)// SUB AND CMP ---> sub id == 2
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
                else if (id == 0x04) // AND
                {
                    cpu->O = 0;
                    cpu->C = 0;
                    setRegister16(a->id, *getRegister16(a->id) &= data);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else if (id == 0x01) // OR
                {
                    cpu->O = 0;
                    cpu->C = 0;
                    setRegister16(a->id, *getRegister16(a->id) |= data);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else if (id == 0x06) // XOR
                {
                    cpu->O = 0;
                    cpu->C = 0;
                    setRegister16(a->id, *getRegister16(a->id) |= data);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
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
        fprintf(stderr, "add ");
    else if (id == 1)
        fprintf(stderr, "adc ");
    else if (id == 2)
        fprintf(stderr, "sub ");
    else if (id == 3)
        fprintf(stderr, "cmp ");
    else if (id == 4)
        fprintf(stderr, "and ");
    else if (id == 5)
        fprintf(stderr, "or ");
    else if (id == 6)
        fprintf(stderr, "xor ");
    else
        fprintf(stderr, "test ");

    if(word == 0x00)
        fprintf(stderr, "%s, %x\n", regByte[0], data);
    else
        fprintf(stderr, "%s, %04x\n", regWord[0], data);


    if(interpret)
    {
        if (word == 0x00)
        {
            switch (id) {
                case 0: // ADD
                    addOAC8(*getRegister8(0x00), (uint8_t)data);
                    *getRegister8(0x00) += (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(0x00));
                    break;
            
                /*case 1:

                    break;*/
                case 2:
                    cmpOAC8(*getRegister8(0x00), (uint8_t)data);
                    *getRegister8(0x00) -= (uint8_t)data;
                    setFlagsZAndS8(*getRegister8(0x00));
                    break;
                case 3:
                    cmpOAC8(*getRegister8(0x00), (uint8_t)data);
                    setFlagsZAndS16(*getRegister8(0x00) - data);
                    break;
                /*case 4:

                    break;
                case 5:

                    break;
                case 6:

                    break;
                case 7:

                    break;*/
            }
        }
        else
        {
            switch(id){
                case 0: // ADD
                    addOAC16(*getRegister16(0x00), data);
                    *getRegister16(0x00) += data;
                    setFlagsZAndS16(*getRegister16(0x00));
                    break;

                    /*case 1:

                        break;*/
                case 2:
                    cmpOAC16(*getRegister16(0x00), data);
                    *getRegister16(0x00) -= data;
                    setFlagsZAndS16(*getRegister16(0x00));
                    break;

                case 3:
                    cmpOAC16(*getRegister16(0x00), data);
                    setFlagsZAndS16(*getRegister16(0x00) - data);
                    break;
                    /*case 4:

                        break;
                    case 5:

                        break;
                    case 6:

                        break;
                    case 7:

                        break;*/
            }
        }
    }

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

    struct pair* a = getRmAddress(rm, mod, disp, word);

    if(id == 0x00)
        fprintf(stderr, "inc ");
    else if(id == 0x01)
        fprintf(stderr, "dec ");
    else if(id == 0x02 || id == 0x03)
        fprintf(stderr, "call ");
    else if(id == 0x04 || id == 0x05)
        fprintf(stderr, "jmp ");

    printRm(rm, mod, disp, word, 0x00);
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(stderr, "\n");

    if (interpret) {
        if (word == 0x00)
        {
            if (a->id == 9)
            {
                if (id == 0x00) // INC
                {
                    addO8(*(mem + a->disp), 0x01);
                    *(mem + a->disp) += 1;
                    setFlagsZAndS8(*(mem + a->disp));
                }
                else if (id == 0x01) // DEC
                {
                    cmpO8(*(mem + a->disp), 0x01);
                    *(mem + a->disp) -= 1;
                    setFlagsZAndS8(*(mem + a->disp));
                }
                else if (id == 0x02 || id == 0x03) // CALL
                {
                    setRegister16(0x04, *getRegister16(0x04) - 2);
                    *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t) curr + read;
                    read = *(mem + a->disp) - curr;
                }
                else // JMP
                {
                    read = *(mem + a->disp) - curr;
                }
            }
            else
            {
                if (id == 0x00) // INC
                {
                    addO8(*getRegister8(a->id), 0x01);
                    setRegister8(a->id, *getRegister8(a->id) + 1);
                    setFlagsZAndS8(*getRegister8(a->id));
                }
                else if (id == 0x01) // DEC
                {
                    cmpO8(*getRegister8(a->id), 0x01);
                    setRegister8(a->id, *getRegister8(a->id) - 1);
                    setFlagsZAndS8(*getRegister8(a->id));
                }
                else if (id == 0x02 || id == 0x03) // CALL
                {
                    setRegister16(0x04, *getRegister16(0x04) - 2);
                    *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t) curr + read;
                    read = *getRegister8(a->id) - curr;
                }
                else // JMP
                {
                    read = *getRegister8(a->id) - curr;
                }
            }
        }
        else
        {
            if (a->id == 9)
            {
                if (id == 0x00) // INC
                {
                    addO16(*((uint16_t*)(mem + a->disp)), 0x01);
                    *((uint16_t*)(mem + a->disp)) += 1;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else if (id == 0x01)
                {
                    cmpO16(*((uint16_t*)(mem + a->disp)), 0x01);
                    *((uint16_t*)(mem + a->disp)) -= 1;
                    setFlagsZAndS16(*((uint16_t*)(mem + a->disp)));
                }
                else if (id == 0x02 || id == 0x03)
                {
                    setRegister16(0x04, *getRegister16(0x04) - 2);
                    *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t) curr + read;
                    read = *((uint16_t*)(mem + a->disp)) - curr;
                }
                else
                {
                    read = *((uint16_t*)(mem + a->disp)) - curr;
                }
            }
            else
            {
                if (id == 0x00) // INC
                {
                    addO16(*getRegister16(a->id), 0x01);
                    setRegister16(a->id, *getRegister16(a->id) + 1);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else if (id == 0x01) // DEC
                {
                    cmpO16(*getRegister16(a->id), 0x01);
                    setRegister16(a->id, *getRegister16(a->id) - 1);
                    setFlagsZAndS16(*getRegister16(a->id));
                }
                else if (id == 0x02 || id == 0x03) // CALL
                {
                    setRegister16(0x04, *getRegister16(0x04) - 2);
                    *((uint16_t*)(mem + *getRegister16(0x04))) = (uint16_t) curr + read;
                    read = *getRegister16(a->id) - curr;
                }
                else // JMP
                {
                    read = *getRegister16(a->id) - curr;
                }
            }
        }
    }
    free(a);
    return read;
}

int incReg(uint8_t* text, int curr, int id)
{
    uint8_t reg = text[curr] % 8;
    printReadBytes(1, text, curr);

    if(id == 0)
        fprintf(stderr, "inc %s\n", regWord[reg]);
    else
        fprintf(stderr, "dec %s\n", regWord[reg]);

    if (interpret) {
        if (id == 0)
        {
            addO16(*getRegister16(reg), 0x01);
            setRegister16(reg, *getRegister16(reg) + 1);
            setFlagsZAndS16(*getRegister16(reg));
        }
        else
        {
            cmpO16(*getRegister16(reg), 0x01);
            setRegister16(reg, *getRegister16(reg) - 1);
            setFlagsZAndS16(*getRegister16(reg));
        }
    }
    return 1;
}

int aaaBaa(uint8_t* text, int curr, int id)
{
    int read = 1;
    if(id == 4)
        read = 2;
    printReadBytes(read, text, curr);

    if(id == 0)
        fprintf(stderr, "aaa\n");
    else if (id == 1)
        fprintf(stderr, "baa\n");
    else if (id == 2)
        fprintf(stderr, "aas\n");
    else if (id == 3)
        fprintf(stderr, "das\n");
    else if (id == 4)
        fprintf(stderr, "aad\n");
    else if (id == 5)
        fprintf(stderr, "cbw\n");
    else
        fprintf(stderr, "cwd\n");

    if(interpret)
    {
        switch(id) {
            case 5:
                setRegister16(0x00, (int8_t) *getRegister8(0x00));
                break;
            
            case 6:
                if (*getRegister16(0x00) >= 0x8000)
                    setRegister16(0x02, 0xffff);
                else
                    setRegister16(0x02, 0x0000);
        }
    }

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

    struct pair* a = getRmAddress(rm, mod, disp, word);
    if (id == 0x03)
        fprintf(stderr, "neg ");
    else if (id == 0x04)
        fprintf(stderr, "mul ");
    else if (id == 0x05)
        fprintf(stderr, "imul ");
    else if (id == 0x02)
        fprintf(stderr, "not ");
    else if (id == 0x06)
        fprintf(stderr, "div ");
    else if (id == 0x07)
        fprintf(stderr, "idiv ");

    printRm(rm, mod, disp, word, 0x00);
    if (a->id == 9 && interpret)
        printMemoryChange(a->disp, word);
    fprintf(stderr, "\n");

    if(interpret)
    {
        uint8_t al;
        uint8_t ah;
        uint32_t tot;
        if (a->id == 9) {
            if (word == 0x00) {
                switch (id)
                {
                    case 0x03:
                        if (*(mem + a->disp) == 0x00)
                            cpu->C = 0;
                        else
                            cpu->C = 1;

                        *(mem + a->disp) = ~(*(mem + a->disp)) + 1;
                        setFlagsZAndS8(*(mem + a->disp));
                        break;
                    
                    case 0x06:
                        al = *getRegister16(0x00) / *(mem + a->disp);
                        ah = *getRegister16(0x00) % *(mem + a->disp);
                        setRegister8(0x00, al);
                        setRegister8(0x02, ah);
                        break;
                }
            }
            else {
                switch (id)
                {
                    case 0x03:
                        if (*((uint16_t*)(mem + a->disp)) == 0x0000)
                            cpu->C = 0;
                        else
                            cpu->C = 1;

                        *((uint16_t*)(mem + a->disp)) = ~(*((uint16_t*)(mem + a->disp))) + 1;
                        setFlagsZAndS8(*((uint16_t*)(mem + a->disp)));
                        break;
                    
                    case 0x06:
                        tot = *getRegister16(0x02) * 65536 + *getRegister16(0x00);
                        setRegister16(0x00, tot / *((uint16_t*)(mem + a->disp)));
                        setRegister16(0x02, tot % *((uint16_t*)(mem + a->disp)));
                        break;
                }
            }
        }  
        else {
            if (word == 0x00) {
                switch (id)
                {
                    case 0x03:
                        if (*getRegister8(a->id) == 0x00)
                            cpu->C = 0;
                        else
                            cpu->C = 1;

                        *getRegister8(a->id) = ~(*getRegister8(a->id)) + 1;
                        setFlagsZAndS8(*getRegister8(a->id));
                        break;
                    
                    case 0x06:
                        al = *getRegister16(0x00) / *getRegister8(a->id);
                        ah = *getRegister16(0x00) % *getRegister8(a->id);
                        setRegister8(0x00, al);
                        setRegister8(0x02, ah);
                        break;
                }
            }
            else {
                switch (id)
                {
                    case 0x03:
                        if (*getRegister16(a->id) == 0x0000)
                            cpu->C = 0;
                        else
                            cpu->C = 1;

                        *getRegister16(a->id) = ~(*getRegister16(a->id)) + 1;
                        setFlagsZAndS16(*getRegister16(a->id));
                        break;

                    case 0x06:
                        tot = *getRegister16(0x02) * 65536 + *getRegister16(0x00);
                        setRegister16(0x00, tot / *getRegister16(a->id));
                        setRegister16(0x02, tot % *getRegister16(a->id));
                        break;
                }
            }
        }
    }

    free(a);
    return read;
}