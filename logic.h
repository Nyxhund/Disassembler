#ifndef _LOGIC_H
#define _LOGIC_H

int shifts(uint8_t* text, int curr);

int logicRegMemToEither(uint8_t* text, int curr, int id);

int logicImmediateToRegMem(uint8_t* text, int curr);

int stringManipulation(uint8_t* text, int curr, int);

#endif
