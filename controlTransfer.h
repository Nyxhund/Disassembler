#ifndef _CONTROLTRANSFER_H
#define _CONTROLTRANSFER_H

int controlDirect(uint8_t* text, int curr, int id);

int controlJumpShort(uint8_t* text, int curr);

int controlSimpleCommands(uint8_t* text, int curr, int id);

int controlConditionalJump(uint8_t* text, int curr);

int controlConditionalLoop(uint8_t* text, int curr);

int controlSyscall(uint8_t* text, int curr);

int controlEscape(uint8_t* text, int curr);

#endif
