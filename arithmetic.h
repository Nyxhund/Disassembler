#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

int arithOpe(uint8_t* text, int curr, int id);

int arithOpeImmediate(uint8_t* text, int curr);

int arithOpeImmediateToAccu(uint8_t* text, int curr, int id);

int arithIncJump(uint8_t* text, int curr);

int arithIncDec(uint8_t* text, int curr, int id);

int arithAdjustConvert(uint8_t* text, int curr, int id);

int arithMultDiv(uint8_t* text, int curr);

#endif
