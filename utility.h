#ifndef _UTILITY_H
#define _UTILITY_H

extern int interpret;
extern uint8_t* mem;

struct CPU {
    uint16_t registers[8];
    char O;
    char S;
    char C;
    char Z;
};

extern struct CPU *cpu;

struct pair {
    uint8_t id;
    uint16_t disp;
};

static const char* regWord[] = {"ax","cx","dx","bx","sp","bp","si","di"};
static const char* regByte[] = {"al","cl","dl","bl","ah","ch","dh","bh"};
static const char* regSegment[] = {"es","cs","ss","ds"};

void printRm(uint8_t rm, uint8_t mod, uint16_t disp, uint8_t word, uint8_t seg);

void printReadBytes(int read, uint8_t* text, int curr);

void printRegisters(int curr);

struct pair* getRmAddress(uint8_t rm, uint8_t mod, uint16_t disp, uint8_t word);

uint16_t* getRegister16(uint8_t reg);
void setRegister16(uint8_t reg, uint16_t val);
uint8_t* getRegister8(uint8_t reg);
void setRegister8(uint8_t reg, uint8_t val);

void setFlagsZAndS8(uint8_t val);
void setFlagsZAndS16(uint16_t val);

void printMemoryChange(uint16_t addr);

#endif
