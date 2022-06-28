#ifndef _DATATRANSFER_H
#define _DATATRANSFER_H


int RegMemtofromReg(uint8_t* text, int curr, uint8_t dir, uint8_t word);

int immediateToRegMem(uint8_t* text, int curr, uint8_t word);

void immediateToRegister(uint8_t* text, int curr);

int memoryToFromAccu(uint8_t* text, int curr, uint8_t word, uint8_t dir);

int regMemToFromSeg(uint8_t* text, int curr, uint8_t dir);

// PUSH

int pushPopRegMem(uint8_t* text, int curr, int pop);

int pushPopReg(uint8_t* text, int curr, int pop);
int pushPopSeg(uint8_t* text, int curr, int pop);

// XCHG

int xchgRegMemWReg(uint8_t* text, int curr);

int xchgRegAccu(uint8_t* text, int curr);

// IN & OUT

int inOutFromTo(uint8_t* text, int curr, int port, int out);

// XLAT NOT DONE

// LEA, LDS, LES

int leaLdsLes(uint8_t* text, int curr, int id);

#endif
