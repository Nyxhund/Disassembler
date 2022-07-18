#ifndef _DATATRANSFER_H
#define _DATATRANSFER_H


int dataTransferOpe(uint8_t* text, int curr, uint8_t dir, uint8_t word);

int dataTransferOpeImmediate(uint8_t* text, int curr, uint8_t word);

void dataTransferImmediateRegister(uint8_t* text, int curr);

int dataTransferAccu(uint8_t* text, int curr, uint8_t word, uint8_t dir);

int dataTransferSeg(uint8_t* text, int curr, uint8_t dir);

int dataTransferPushPopOpe(uint8_t* text, int curr, int pop);

int dataTransferPushPopRegister(uint8_t* text, int curr, int pop);
int dataTransferPushPopSeg(uint8_t* text, int curr, int pop);

int dataTransferXchg(uint8_t* text, int curr);

int dataTransferXchgAccu(uint8_t* text, int curr);

int dataTransferInOut(uint8_t* text, int curr, int port, int out);

int dataTransferLeaLdsLes(uint8_t* text, int curr, int id);

#endif
