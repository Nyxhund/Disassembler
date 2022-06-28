#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

int regMemAddReg(uint8_t* text, int curr, int id);

int immediateAddRegMem(uint8_t* text, int curr);

int immediateToAccu(uint8_t* text, int curr, int id);

int incRegMem(uint8_t* text, int curr);

int incReg(uint8_t* text, int curr, int id);

int aaaBaa(uint8_t* text, int curr, int id);

int negMul(uint8_t* text, int curr);

#endif
