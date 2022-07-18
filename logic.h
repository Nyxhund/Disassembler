#ifndef _LOGIC_H
#define _LOGIC_H

int logicShifts(uint8_t* text, int curr);

int logicRegMemToEither(uint8_t* text, int curr, int id);

int logicImmediateToRegMem(uint8_t* text, int curr);

int logicStringManipulation(uint8_t* text, int curr, int);

#endif
