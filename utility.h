#ifndef _UTILITY_H
#define _UTILITY_H


static const char* regWord[] = {"ax","cx","dx","bx","sp","bp","si","di"};
static const char* regByte[] = {"al","cl","dl","bl","ah","ch","dh","bh"};
static const char* regSegment[] = {"es","cs","ss","ds"};

void printRm(uint8_t rm, uint8_t mod, uint16_t disp, uint8_t word, uint8_t seg);

void printReadBytes(int read, uint8_t* text, int curr);

#endif
