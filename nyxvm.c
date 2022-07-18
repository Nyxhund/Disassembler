#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "unistd.h"
#include "string.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "a.out.h"
#include "stdint.h"
#include "dataTransfer.h"
#include "arithmetic.h"
#include "stdio.h"
#include "logic.h"
#include "utility.h"
#include "controlTransfer.h"

int processOPCode(uint8_t* text, int curr, int max)
{
    int read;
    uint8_t tmp;
    switch(text[curr])
    {
        case 0x00:
            if(curr + 1 < max)
                read = arithOpe(text, curr, 0);
            else
            {
                printReadBytes(1, text, curr);
                fprintf(fd, "(undefined)\n");
                read = 1;
            }
            break;
        case 0x01:
            read = arithOpe(text, curr, 0);
            break;
        case 0x02:
            read = arithOpe(text, curr, 0);
            break;
        case 0x03:
            read = arithOpe(text, curr, 0);
            break;
        
        case 0x04:
            read = arithOpeImmediateToAccu(text, curr, 0);
            break;
        case 0x05:
            read = arithOpeImmediateToAccu(text, curr, 0);
            break;

        case 0x06:
            read = dataTransferPushPopSeg(text, curr, 0);
            break;
        case 0x07:
            read = dataTransferPushPopSeg(text, curr, 1);
            break;
        
        case 0x08:
            read = logicRegMemToEither(text, curr, 1);
            break;
        case 0x09:
            read = logicRegMemToEither(text, curr, 1);
            break;
        case 0x0a:
            read = logicRegMemToEither(text, curr, 1);
            break;
        case 0x0b:
            read = logicRegMemToEither(text, curr, 1);
            break;

        case 0x0c:
            read = arithOpeImmediateToAccu(text, curr, 5);
            break;
        case 0x0d:
            read = arithOpeImmediateToAccu(text, curr, 5);
            break;

        case 0x0e:
            read = dataTransferPushPopSeg(text, curr, 0);
            break;
        case 0x0f:
            read = dataTransferPushPopSeg(text, curr, 1);
            break;
        
        case 0x10:
            read = arithOpe(text, curr, 1);
            break;
        case 0x11:
            read = arithOpe(text, curr, 1);
            break;
        case 0x12:
            read = arithOpe(text, curr, 1);
            break;
        case 0x13:
            read = arithOpe(text, curr, 1);
            break;
        
        case 0x14:
            read = arithOpeImmediateToAccu(text, curr, 1);
            break;
        case 0x15:
            read = arithOpeImmediateToAccu(text, curr, 1);
            break;

        case 0x16:
            read = dataTransferPushPopSeg(text, curr, 0);
            break;
        case 0x17:
            read = dataTransferPushPopSeg(text, curr, 1);
            break;

        case 0x18:
            read = arithOpe(text, curr, 3);
            break;
        case 0x19:
            read = arithOpe(text, curr, 3);
            break;
        case 0x1a:
            read = arithOpe(text, curr, 3);
            break;
        case 0x1b:
            read = arithOpe(text, curr, 3);
            break;
        
        case 0x1e:
            read = dataTransferPushPopSeg(text, curr, 0);
            break;
        case 0x1f:
            read = dataTransferPushPopSeg(text, curr, 1);
            break;

        case 0x20:
            read = logicRegMemToEither(text, curr, 0);
            break;
        case 0x21:
            read = logicRegMemToEither(text, curr, 0);
            break;
        case 0x22:
            read = logicRegMemToEither(text, curr, 0);
            break;
        case 0x23:
            read = logicRegMemToEither(text, curr, 0);
            break;

        case 0x24:
            read = arithOpeImmediateToAccu(text, curr, 4);
            break;
        case 0x25:
            read = arithOpeImmediateToAccu(text, curr, 4);
            break;

        
        case 0x27:
            read = arithAdjustConvert(text, curr, 1);
            break;

        case 0x28:
            read = arithOpe(text, curr, 2);
            break;
        case 0x29:
            read = arithOpe(text, curr, 2);
            break; 
        case 0x2a:
            read = arithOpe(text, curr, 2);
            break;
        case 0x2b:
            read = arithOpe(text, curr, 2);
            break;
        
        case 0x2c:
            read = arithOpeImmediateToAccu(text, curr, 2);
            break;
        case 0x2d:
            read = arithOpeImmediateToAccu(text, curr, 2);
            break;

        case 0x2f:
            read = arithAdjustConvert(text, curr, 3);
            break;
        
        case 0x30:
            read = logicRegMemToEither(text, curr, 2);
            break;
        case 0x31:
            read = logicRegMemToEither(text, curr, 2);
            break;
        case 0x32:
            read = logicRegMemToEither(text, curr, 2);
            break;
        case 0x33:
            read = logicRegMemToEither(text, curr, 2);
            break;

        case 0x34:
            read = arithOpeImmediateToAccu(text, curr, 6);
            break;
        case 0x35:
            read = arithOpeImmediateToAccu(text, curr, 6);
            break;

        case 0x37:
            read = arithAdjustConvert(text, curr, 0);
            break;

        case 0x38:
            read = arithOpe(text, curr, 4);
            break;
        case 0x39:
            read = arithOpe(text, curr, 4);
            break;
        case 0x3a:
            read = arithOpe(text, curr, 4);
            break;
        case 0x3b:
            read = arithOpe(text, curr, 4);
            break;
        case 0x3c:
            read = arithOpeImmediateToAccu(text, curr, 3);
            break;
        case 0x3d:
            read = arithOpeImmediateToAccu(text, curr, 3);
            break;

        case 0x3f:
            read = arithAdjustConvert(text, curr, 2);
            break;

        case 0x40:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x41:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x42:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x43:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x44:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x45:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x46:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x47:
            read = arithIncDec(text, curr, 0);
            break;
        case 0x48:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x49:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4a:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4b:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4c:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4d:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4e:
            read = arithIncDec(text, curr, 1);
            break;
        case 0x4f:
            read = arithIncDec(text, curr, 1);
            break;

        case 0x50:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x51:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x52:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x53:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x54:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x55:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x56:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x57:
            read = dataTransferPushPopRegister(text, curr, 0);
            break;
        case 0x58:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x59:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5a:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5b:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5c:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5d:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5e:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;
        case 0x5f:
            read = dataTransferPushPopRegister(text, curr, 1);
            break;

        case 0x70:
            read = controlConditionalJump(text, curr);
            break;
        case 0x71:
            read = controlConditionalJump(text, curr);
            break;
        case 0x72:
            read = controlConditionalJump(text, curr);
            break;
        case 0x73:
            read = controlConditionalJump(text, curr);
            break;
        case 0x74:
            read = controlConditionalJump(text, curr);
            break;
        case 0x75:
            read = controlConditionalJump(text, curr);
            break;
        case 0x76:
            read = controlConditionalJump(text, curr);
            break;
        case 0x77:
            read = controlConditionalJump(text, curr);
            break;
        case 0x78:
            read = controlConditionalJump(text, curr);
            break;
        case 0x79:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7a:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7b:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7c:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7d:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7e:
            read = controlConditionalJump(text, curr);
            break;
        case 0x7f:
            read = controlConditionalJump(text, curr);
            break;

        case 0x80:
            read = arithOpeImmediate(text, curr);
            break;
        case 0x81:
            read = arithOpeImmediate(text, curr);
            break;
        case 0x82:
            read = arithOpeImmediate(text, curr);
            break;
        case 0x83:
            read = arithOpeImmediate(text, curr);
            break;

        case 0x84:
            read = logicRegMemToEither(text, curr, 3);
            break;
        case 0x85:
            read = logicRegMemToEither(text, curr, 3);
            break;

        case 0x86:
            read = dataTransferXchg(text, curr);
            break;
        case 0x87:
            read = dataTransferXchg(text, curr);
            break;

        case 0x88:
            read = dataTransferOpe(text, curr, 0x00, 0x00);
            break;
        case 0x89:
            read = dataTransferOpe(text, curr, 0x00, 0x01);
            break;
        case 0x8a:
            read = dataTransferOpe(text, curr, 0x01, 0x00);
            break;
        case 0x8b:
            read = dataTransferOpe(text, curr, 0x01, 0x01);
            break;

        case 0x8c:
            read = dataTransferSeg(text, curr, 0x00);
            break;
        
        case 0x8d:
            read = dataTransferLeaLdsLes(text, curr, 0);
            break;

        case 0x8e:
            read = dataTransferSeg(text, curr, 0x01);
            break;
        
        case 0x8f:
            read = dataTransferPushPopOpe(text, curr, 1);
            break;
    
        case 0x90:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x91:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x92:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x93:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x94:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x95:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x96:
            read = dataTransferXchgAccu(text, curr);
            break;
        case 0x97:
            read = dataTransferXchgAccu(text, curr);
            break;
        
        case 0x98:
            read = arithAdjustConvert(text, curr, 5);
            break;
        case 0x99:
            read = arithAdjustConvert(text, curr, 6);
            break;
        
        case 0x9b:
            read = controlSimpleCommands(text, curr, 12);
            break;

        case 0xa0:
            read = dataTransferAccu(text, curr, 0x00, 0x00);
            break;
        case 0xa1:
            read = dataTransferAccu(text, curr, 0x01, 0x00);
            break;
        case 0xa2:
            read = dataTransferAccu(text, curr, 0x00, 0x01);
            break;
        case 0xa3:
            read = dataTransferAccu(text, curr, 0x01, 0x01);
            break;

        case 0xa4:
            read = logicStringManipulation(text, curr, 1);
            break;
        case 0xa5:
            read = logicStringManipulation(text, curr, 1);
            break;
        case 0xa6:
            read = logicStringManipulation(text, curr, 2);
            break;
        case 0xa7:
            read = logicStringManipulation(text, curr, 2);
            break;

        case 0xa8:
            read = arithOpeImmediateToAccu(text, curr, 7);
            break;
        case 0xa9:
            read = arithOpeImmediateToAccu(text, curr, 7);
            break;
        
        case 0xaa:
            read = logicStringManipulation(text, curr, 5);
            break;
        case 0xab:
            read = logicStringManipulation(text, curr, 5);
            break;
        case 0xac:
            read = logicStringManipulation(text, curr, 4);
            break;
        case 0xad:
            read = logicStringManipulation(text, curr, 4);
            break;
        case 0xae:
            read = logicStringManipulation(text, curr, 3);
            break;
        case 0xaf:
            read = logicStringManipulation(text, curr, 3);
            break;

        case 0xb0:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb1:
            dataTransferImmediateRegister(text, curr);
            read =  2;
            break;
        case 0xb2:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb3:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb4:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb5:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb6:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb7:
            dataTransferImmediateRegister(text, curr);
            read = 2;
            break;
        case 0xb8:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xb9:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xba:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xbb:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xbc:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xbd:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xbe:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;
        case 0xbf:
            dataTransferImmediateRegister(text, curr);
            read = 3;
            break;

        case 0xc2:
            read = controlDirect(text, curr, 3);
            break;

        case 0xc3:
            read = controlSimpleCommands(text, curr, 0);
            break;
        
        case 0xc4:
            read = dataTransferLeaLdsLes(text, curr, 2);
            break;
        case 0xc5:
            read = dataTransferLeaLdsLes(text, curr, 1);
            break;

        case 0xc6:
            read = dataTransferOpeImmediate(text, curr, 0);
            break;
        case 0xc7:
            read = dataTransferOpeImmediate(text, curr, 1);
            break;
        
        case 0xca:
            read = controlDirect(text, curr, 3);
            break;
        
        case 0xcb:
            read = controlSimpleCommands(text, curr, 0);
            break;
        case 0xcc:
            read = controlSimpleCommands(text, curr, 1);
            break;

        case 0xcd:
            read = controlSyscall(text, curr);
            break;
        
        case 0xce:
            read = controlSimpleCommands(text, curr, 2);
            break;
        case 0xcf:
            read = controlSimpleCommands(text, curr, 3);
            break;
        
        case 0xd0:
            read = logicShifts(text, curr);
            break;
        case 0xd1:
            read = logicShifts(text, curr);
            break;
        case 0xd2:
            read = logicShifts(text, curr);
            break;
        case 0xd3:
            read = logicShifts(text, curr);
            break;

        case 0xd5:
            read = arithAdjustConvert(text, curr, 4);
            break;

        case 0xd8:
            read = controlEscape(text, curr);
            break;
        case 0xd9:
            read = controlEscape(text, curr);
            break;
        case 0xda:
            read = controlEscape(text, curr);
            break;
        case 0xdb:
            read = controlEscape(text, curr);
            break;
        case 0xdc:
            read = controlEscape(text, curr);
            break;
        case 0xdd:
            read = controlEscape(text, curr);
            break;
        case 0xde:
            read = controlEscape(text, curr);
            break;
        case 0xdf:
            read = controlEscape(text, curr);
            break;

        case 0xe0:
            read = controlConditionalLoop(text, curr);
            break;
        case 0xe1:
            read = controlConditionalLoop(text, curr);
            break;
        case 0xe2:
            read = controlConditionalLoop(text, curr);
            break;
        case 0xe3:
            read = controlConditionalLoop(text, curr);
            break;

        case 0xe4:
            read = dataTransferInOut(text, curr, 1, 0);
            break;
        case 0xe5:
            read = dataTransferInOut(text, curr, 1, 0);
            break;
        case 0xe6:
            read = dataTransferInOut(text, curr, 1, 1);
            break;
        case 0xe7:
            read = dataTransferInOut(text, curr, 1, 1);
            break;

        case 0xe8:
            read = controlDirect(text, curr, 0);
            break;
        case 0xe9:
            read = controlDirect(text, curr, 1);
            break;

        case 0xeb:
            read = controlJumpShort(text, curr);
            break;

        case 0xec:
            read = dataTransferInOut(text, curr, 0, 0);
            break;
        case 0xed:
            read = dataTransferInOut(text, curr, 0, 0);
            break;
        case 0xee:
            read = dataTransferInOut(text, curr, 0, 1);
            break;
        case 0xef:
            read = dataTransferInOut(text, curr, 0, 1);
            break;
        
        case 0xf0:
            read = controlSimpleCommands(text, curr, 13);
            break;
        
        case 0xf2:
            read = logicStringManipulation(text, curr, 0);
            break;
        case 0xf3:
            read = logicStringManipulation(text, curr, 0);
            break;
        
        case 0xf4:
            read = controlSimpleCommands(text, curr, 11);
            break;
        case 0xf5:
            read = controlSimpleCommands(text, curr, 5);
            break;

        case 0xf6:
            if((text[curr+1] % 64) / 8 != 0x00)
                read = arithMultDiv(text, curr);
            else
                read = logicImmediateToRegMem(text, curr);
            break;
        
        case 0xf7:
            if((text[curr+1] % 64) / 8 != 0x00)
                read = arithMultDiv(text, curr);
            else
                read = logicImmediateToRegMem(text, curr);
            break;
        
        case 0xf8:
            read = controlSimpleCommands(text, curr, 4);
            break;
        case 0xf9:
            read = controlSimpleCommands(text, curr, 6);
            break;
        case 0xfa:
            read = controlSimpleCommands(text, curr, 9);
            break;
        case 0xfb:
            read = controlSimpleCommands(text, curr, 10);
            break;
        case 0xfc:
            read = controlSimpleCommands(text, curr, 7);
            break;
        case 0xfd:
            read = controlSimpleCommands(text, curr, 8);
            break;

        case 0xfe:
            read = arithIncJump(text, curr);
            break;

        case 0xff:
            if((text[curr+1] % 64) / 8 != 0x06)
                read = arithIncJump(text, curr);
            else
                read = dataTransferPushPopOpe(text, curr, 0);
            break;


        default:
            printReadBytes(1, text, curr);
            fprintf(fd, "(undefined)\n");
            read = 1;
            break;
    }
    return read;
}

int interpret;
struct CPU *cpu;
uint8_t *mem;
FILE* fd;

void processArgs(char** argv, uint16_t argc)
{
    const char* env = "PATH=/usr:/usr/bin/";
    uint16_t* addr = malloc(argc * sizeof(uint16_t));
    int position = 0;
    uint16_t len = strlen(env);
    uint16_t env_head;
    len++;

    for(int i = 0; i < argc; ++i)
    {
        len += strlen(argv[i]);
        len++;
    }

    if(len % 2)
    {
        setRegister16(0x04, *getRegister16(0x04) - 1);
        *(mem + *getRegister16(0x04)) = 0x00;
    }

    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = '\0';
    
    for(int i = strlen(env) - 1; i >= 0; --i)
    {
        setRegister16(0x04, *getRegister16(0x04) - 1);
        *(mem + *getRegister16(0x04)) = env[i];
    }
    env_head = *getRegister16(0x04);


    for (int i = argc - 1; i >= 0; --i)
    {
        const char *arg = argv[i];
        setRegister16(0x04, *getRegister16(0x04) - 1);
        *(mem + *getRegister16(0x04)) = '\0';

        for (int j = strlen(argv[i]) - 1; j >= 0; --j)
        {
            setRegister16(0x04, *getRegister16(0x04) - 1);
            *(mem + *getRegister16(0x04)) = arg[j];
        }
        addr[position] = *getRegister16(0x04);
        position++;
    }

    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = 0x00;
    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = 0x00;
    
    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = env_head >> 8;
    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = env_head;

    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = 0x00;
    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = 0x00;

    for (int i = 0; i < position; ++i)
    {
        
        setRegister16(0x04, *getRegister16(0x04) - 1);
        *(mem + *getRegister16(0x04)) = addr[i] >> 8;
        setRegister16(0x04, *getRegister16(0x04) - 1);
        *(mem + *getRegister16(0x04)) = addr[i];
    }

    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = argc >> 8;
    setRegister16(0x04, *getRegister16(0x04) - 1);
    *(mem + *getRegister16(0x04)) = argc;

    free(addr);
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Wrong input: please enter at least a path to a binary executable.\n");
        return 1;
    }

    int path = 2;
    fd = stderr;
    
    if(strcmp(argv[1], "-d") == 0)
        interpret = 0;
    else if (strcmp(argv[1], "-m") == 0)
        interpret = 1;
    else
    {
        path = 1;
        fd = fopen("binDumpStdERR.txt", "w"); // DUMP FILE FOR PRINTING IF NOT IN DEBUG MODE
        interpret = 1;
    }

    FILE* file = fopen(argv[path], "rb");

    if(!file)
    {
        fprintf(stderr, "Wrong input: please enter a correct path to an executable.\n");
        return 1;
    }

    size_t a = 0;
    if(a)
    {} // For no warning (I'm sorry)
		
    struct exec* header = malloc(sizeof(struct exec));
    a = fread(header, sizeof(struct exec), 1, file);

    
    uint8_t* text = malloc(header->a_text);
    a = fread(text, header->a_text, 1, file);

    mem = malloc(0x10000);
    memset(mem, 0, 0x10000);
    a = fread(mem, header->a_data, 1, file);
    
    int curr = 0;

    cpu = malloc(sizeof(struct CPU));
    memset(cpu, 0, sizeof(struct CPU));

    processArgs(argv + path, argc - path);

    if(interpret)
        fprintf(fd, " AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
    
    while(curr < header->a_text)
    {
        if (!interpret)
            fprintf(fd, "%04x: ", curr);
        else
            printRegisters(curr);
        curr += processOPCode(text, curr, header->a_text);
        curr %= 0x10000;
    }

    free(header);
    free(text);
    fclose(file);
    if(interpret)
        free(cpu);
    
}