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

int printInfo(uint8_t* text, int curr, int max)
{
    int read;
    uint8_t tmp;
    switch(text[curr])
    {
        case 0x00:
            if(curr + 1 < max)
                read = regMemAddReg(text, curr, 0);
            else
            {
                printReadBytes(1, text, curr);
                printf("(undefined)\n");
                read = 1;
            }
            break;
        
        case 0x01:
            read = regMemAddReg(text, curr, 0);
            break;
        case 0x02:
            read = regMemAddReg(text, curr, 0);
            break;
        case 0x03:
            read = regMemAddReg(text, curr, 0);
            break;
        
        case 0x04:
            read = immediateToAccu(text, curr, 0);
            break;
        case 0x05:
            read = immediateToAccu(text, curr, 0);
            break;

        case 0x06:
            read = pushPopSeg(text, curr, 0);
            break;
        
        case 0x07:
            read = pushPopSeg(text, curr, 1);
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
            read = immediateToAccu(text, curr, 5);
            break;
        case 0x0d:
            read = immediateToAccu(text, curr, 5);
            break;

        case 0x0e:
            read = pushPopSeg(text, curr, 0);
            break;
        
        case 0x0f:
            read = pushPopSeg(text, curr, 1);
            break;
        
        case 0x10:
            read = regMemAddReg(text, curr, 1);
            break;
        case 0x11:
            read = regMemAddReg(text, curr, 1);
            break;
        case 0x12:
            read = regMemAddReg(text, curr, 1);
            break;
        case 0x13:
            read = regMemAddReg(text, curr, 1);
            break;
        
        case 0x14:
            read = immediateToAccu(text, curr, 1);
            break;
        case 0x15:
            read = immediateToAccu(text, curr, 1);
            break;

        case 0x16:
            read = pushPopSeg(text, curr, 0);
            break;
        
        case 0x17:
            read = pushPopSeg(text, curr, 1);
            break;
        
        case 0x1e:
            read = pushPopSeg(text, curr, 0);
            break;
        
        case 0x1f:
            read = pushPopSeg(text, curr, 1);
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
            read = immediateToAccu(text, curr, 4);
            break;
        case 0x25:
            read = immediateToAccu(text, curr, 4);
            break;

        
        case 0x27:
            read = aaaBaa(text, curr, 1);
            break;

        case 0x28:
            read = regMemAddReg(text, curr, 2);
            break;
        case 0x29:
            read = regMemAddReg(text, curr, 2);
            break; 
        case 0x2a:
            read = regMemAddReg(text, curr, 2);
            break;
        case 0x2b:
            read = regMemAddReg(text, curr, 2);
            break;
        
        case 0x2c:
            read = immediateToAccu(text, curr, 2);
            break;
        case 0x2d:
            read = immediateToAccu(text, curr, 2);
            break;

        case 0x2f:
            read = aaaBaa(text, curr, 3);
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
            read = immediateToAccu(text, curr, 6);
            break;

        case 0x35:
            read = immediateToAccu(text, curr, 6);
            break;

        case 0x37:
            read = aaaBaa(text, curr, 0);
            break;

        case 0x38:
            read = regMemAddReg(text, curr, 3);
            break;
        case 0x39:
            read = regMemAddReg(text, curr, 3);
            break;
        case 0x3a:
            read = regMemAddReg(text, curr, 3);
            break;
        case 0x3b:
            read = regMemAddReg(text, curr, 3);
            break;
        case 0x3c:
            read = immediateToAccu(text, curr, 3);
            break;
        case 0x3d:
            read = immediateToAccu(text, curr, 3);
            break;

        case 0x3f:
            read = aaaBaa(text, curr, 2);
            break;

        case 0x40:
            read = incReg(text, curr, 0);
            break;
        case 0x41:
            read = incReg(text, curr, 0);
            break;
        case 0x42:
            read = incReg(text, curr, 0);
            break;
        case 0x43:
            read = incReg(text, curr, 0);
            break;
        case 0x44:
            read = incReg(text, curr, 0);
            break;
        case 0x45:
            read = incReg(text, curr, 0);
            break;
        case 0x46:
            read = incReg(text, curr, 0);
            break;
        case 0x47:
            read = incReg(text, curr, 0);
            break;
        case 0x48:
            read = incReg(text, curr, 1);
            break;
        case 0x49:
            read = incReg(text, curr, 1);
            break;
        case 0x4a:
            read = incReg(text, curr, 1);
            break;
        case 0x4b:
            read = incReg(text, curr, 1);
            break;
        case 0x4c:
            read = incReg(text, curr, 1);
            break;
        case 0x4d:
            read = incReg(text, curr, 1);
            break;
        case 0x4e:
            read = incReg(text, curr, 1);
            break;
        case 0x4f:
            read = incReg(text, curr, 1);
            break;

        case 0x50:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x51:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x52:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x53:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x54:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x55:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x56:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x57:
            read = pushPopReg(text, curr, 0);
            break;
        case 0x58:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x59:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5a:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5b:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5c:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5d:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5e:
            read = pushPopReg(text, curr, 1);
            break;
        case 0x5f:
            read = pushPopReg(text, curr, 1);
            break;

        case 0x70:
            read = conditionalJump(text, curr);
            break;
        case 0x71:
            read = conditionalJump(text, curr);
            break;
        case 0x72:
            read = conditionalJump(text, curr);
            break;
        case 0x73:
            read = conditionalJump(text, curr);
            break;
        case 0x74:
            read = conditionalJump(text, curr);
            break;
        case 0x75:
            read = conditionalJump(text, curr);
            break;
        case 0x76:
            read = conditionalJump(text, curr);
            break;
        case 0x77:
            read = conditionalJump(text, curr);
            break;
        case 0x78:
            read = conditionalJump(text, curr);
            break;
        case 0x79:
            read = conditionalJump(text, curr);
            break;
        case 0x7a:
            read = conditionalJump(text, curr);
            break;
        case 0x7b:
            read = conditionalJump(text, curr);
            break;
        case 0x7c:
            read = conditionalJump(text, curr);
            break;
        case 0x7d:
            read = conditionalJump(text, curr);
            break;
        case 0x7e:
            read = conditionalJump(text, curr);
            break;
        case 0x7f:
            read = conditionalJump(text, curr);
            break;

        case 0x80:
            read = immediateAddRegMem(text, curr);
            break;
        case 0x81:
            read = immediateAddRegMem(text, curr);
            break;
        case 0x82:
            read = immediateAddRegMem(text, curr);
            break;
        case 0x83:
            read = immediateAddRegMem(text, curr);
            break;

        case 0x84:
            read = logicRegMemToEither(text, curr, 3);
            break;
        case 0x85:
            read = logicRegMemToEither(text, curr, 3);
            break;

        case 0x86:
            read = xchgRegMemWReg(text, curr);
            break;
        case 0x87:
            read = xchgRegMemWReg(text, curr);
            break;

        case 0x88:
            read =  RegMemtofromReg(text, curr, 0x00, 0x00);
            break;
        
        case 0x89:
            read = RegMemtofromReg(text, curr, 0x00, 0x01);
            break;
        
        case 0x8a:
            read = RegMemtofromReg(text, curr, 0x01, 0x00);
            break;
    
        case 0x8b:
            read = RegMemtofromReg(text, curr, 0x01, 0x01);
            break;
        
        case 0x8c:
            read = regMemToFromSeg(text, curr, 0x00);
            break;
        
        case 0x8d:
            read = leaLdsLes(text, curr, 0);
            break;

        case 0x8e:
            read = regMemToFromSeg(text, curr, 0x01);
            break;
        
        case 0x8f:
            read = pushPopRegMem(text, curr, 1);
            break;
    
        case 0x90:
            read = xchgRegAccu(text, curr);
            break;
        case 0x91:
            read = xchgRegAccu(text, curr);
            break;
        case 0x92:
            read = xchgRegAccu(text, curr);
            break;
        case 0x93:
            read = xchgRegAccu(text, curr);
            break;
        case 0x94:
            read = xchgRegAccu(text, curr);
            break;
        case 0x95:
            read = xchgRegAccu(text, curr);
            break;
        case 0x96:
            read = xchgRegAccu(text, curr);
            break;
        case 0x97:
            read = xchgRegAccu(text, curr);
            break;
        
        case 0x98:
            read = aaaBaa(text, curr, 5);
            break;
        case 0x99:
            read = aaaBaa(text, curr, 6);
            break;
        
        case 0x9b:
            read = controlSimpleCommands(text, curr, 12);
            break;

        case 0xa0:
            read = memoryToFromAccu(text, curr, 0x00, 0x00);
            break;
        
        case 0xa1:
            read = memoryToFromAccu(text, curr, 0x01, 0x00);
            break;

        case 0xa2:
            read = memoryToFromAccu(text, curr, 0x00, 0x01);
            break;

        case 0xa3:
            read = memoryToFromAccu(text, curr, 0x01, 0x01);
            break;

        case 0xa4:
            read = stringManipulation(text, curr, 1);
            break;
        case 0xa5:
            read = stringManipulation(text, curr, 1);
            break;
        case 0xa6:
            read = stringManipulation(text, curr, 2);
            break;
        case 0xa7:
            read = stringManipulation(text, curr, 2);
            break;

        case 0xa8:
            read = immediateToAccu(text, curr, 7);
            break;
        
        case 0xa9:
            read = immediateToAccu(text, curr, 7);
            break;
        
        case 0xaa:
            read = stringManipulation(text, curr, 5);
            break;
        case 0xab:
            read = stringManipulation(text, curr, 5);
            break;
        case 0xac:
            read = stringManipulation(text, curr, 4);
            break;
        case 0xad:
            read = stringManipulation(text, curr, 4);
            break;
        case 0xae:
            read = stringManipulation(text, curr, 3);
            break;
        case 0xaf:
            read = stringManipulation(text, curr, 3);
            break;

        case 0xb0:
            immediateToRegister(text, curr);
            read = 2;
            break;

        case 0xb1:
            immediateToRegister(text, curr);
            read =  2;
            break;

        case 0xb2:
            immediateToRegister(text, curr);
            read = 2;
            break;
        
        case 0xb3:
            immediateToRegister(text, curr);
            read = 2;
            break;
        case 0xb4:
            immediateToRegister(text, curr);
            read = 2;
            break;
        case 0xb5:
            immediateToRegister(text, curr);
            read = 2;
            break;
        case 0xb6:
            immediateToRegister(text, curr);
            read = 2;
            break;
        case 0xb7:
            immediateToRegister(text, curr);
            read = 2;
            break;
        case 0xb8:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xb9:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xba:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xbb:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xbc:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xbd:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xbe:
            immediateToRegister(text, curr);
            read = 3;
            break;
        case 0xbf:
            immediateToRegister(text, curr);
            read = 3;
            break;

        case 0xc2:
            read = controlDirect(text, curr, 3);
            break;

        case 0xc3:
            read = controlSimpleCommands(text, curr, 0);
            break;
        
        case 0xc4:
            read = leaLdsLes(text, curr, 2);
            break;
 
        case 0xc5:
            read = leaLdsLes(text, curr, 1);
            break;

        case 0xc6:
            read = immediateToRegMem(text, curr, 0);
            break;

        case 0xc7:
            read = immediateToRegMem(text, curr, 1);
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
            read = intTypeSpec(text, curr);
            break;
        
        case 0xce:
            read = controlSimpleCommands(text, curr, 2);
            break;
        case 0xcf:
            read = controlSimpleCommands(text, curr, 3);
            break;
        
        case 0xd0:
            read = shifts(text, curr);
            break;
        case 0xd1:
            read = shifts(text, curr);
            break;
        case 0xd2:
            read = shifts(text, curr);
            break;
        case 0xd3:
            read = shifts(text, curr);
            break;

        case 0xd5:
            read = aaaBaa(text, curr, 4);
            break;

        case 0xd8:
            read = escape(text, curr);
            break;
        case 0xd9:
            read = escape(text, curr);
            break;
        case 0xda:
            read = escape(text, curr);
            break;
        case 0xdb:
            read = escape(text, curr);
            break;
        case 0xdc:
            read = escape(text, curr);
            break;
        case 0xdd:
            read = escape(text, curr);
            break;
        case 0xde:
            read = escape(text, curr);
            break;
        case 0xdf:
            read = escape(text, curr);
            break;

        case 0xe0:
            read = conditionalLoop(text, curr);
            break;
        case 0xe1:
            read = conditionalLoop(text, curr);
            break;
        case 0xe2:
            read = conditionalLoop(text, curr);
            break;
        case 0xe3:
            read = conditionalLoop(text, curr);
            break;

        case 0xe4:
            read = inOutFromTo(text, curr, 1, 0);
            break;
        case 0xe5:
            read = inOutFromTo(text, curr, 1, 0);
            break;
        case 0xe6:
            read = inOutFromTo(text, curr, 1, 1);
            break;
        case 0xe7:
            read = inOutFromTo(text, curr, 1, 1);
            break;

        case 0xe8:
            read = controlDirect(text, curr, 0);
            break;
        case 0xe9:
            read = controlDirect(text, curr, 1);
            break;
        case 0xeb:
            read = jumpShort(text, curr);
            break;

        case 0xec:
            read = inOutFromTo(text, curr, 0, 0);
            break;
        case 0xed:
            read = inOutFromTo(text, curr, 0, 0);
            break;
        case 0xee:
            read = inOutFromTo(text, curr, 0, 1);
            break;
        case 0xef:
            read = inOutFromTo(text, curr, 0, 1);
            break;
        
        case 0xf0:
            read = controlSimpleCommands(text, curr, 13);
            break;
        
        case 0xf2:
            read = stringManipulation(text, curr, 0);
            break;
        case 0xf3:
            read = stringManipulation(text, curr, 0);
            break;
        
        case 0xf4:
            read = controlSimpleCommands(text, curr, 11);
            break;
        
        case 0xf5:
            read = controlSimpleCommands(text, curr, 5);
            break;

        case 0xf6:
            if((text[curr+1] % 64) / 8 != 0x00)
                read = negMul(text, curr);
            else
                read = logicImmediateToRegMem(text, curr);
            break;
        
        case 0xf7:
            if((text[curr+1] % 64) / 8 != 0x00)
                read = negMul(text, curr);
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
            read = incRegMem(text, curr);
            break;

        case 0xff:
            if((text[curr+1] % 64) / 8 != 0x06)
                read = incRegMem(text, curr);
            else
                read = pushPopRegMem(text, curr, 0);
            break;


        default:
            printReadBytes(1, text, curr);
            printf("(undefined)\n");
            read = 1;
            break;
    }
    return read;
}

int interpret;
struct CPU *cpu;
uint8_t *mem;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("Wrong input: please enter a path to a binary executable and a mode to execute.");
        return 1;
    }
    
    if(strcmp(argv[2], "-d") == 0)
        interpret = 0;
    else if (strcmp(argv[2], "-m") == 0)
        interpret = 1;
    else
    {
        printf("Wrong input: please enter a correct mode of execution.");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");

    if(!file)
        return 1;

    size_t a = 0;
    if(a)
    {} // For no warning (I'm sorry)
		
    struct exec* header = malloc(sizeof(struct exec));
    a = fread(header, sizeof(struct exec), 1, file);

    
    uint8_t* text = malloc(header->a_text);
    a = fread(text, header->a_text, 1, file);

    mem = malloc(0xffff);
    a = fread(mem, header->a_data, 1, file);
    
    int curr = 0;

    cpu = malloc(sizeof(struct CPU));
    memset(cpu, 0, sizeof(struct CPU));
    cpu->registers[4] = 0xffdc;
    mem[0xffdc] = 0x01;
    mem[0xffde] = 0xe6;
    mem[0xffdf] = 0xff;

    if(interpret)
        printf(" AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
    
    while(curr < header->a_text)
    {
        if (!interpret)
            printf("%04x: ", curr);
        else
            printRegisters(curr);
        curr += printInfo(text, curr, header->a_text);
    }

    free(header);
    free(text);
    fclose(file);
    if(interpret)
        free(cpu);
}
