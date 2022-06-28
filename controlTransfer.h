#ifndef _CONTROLTRANSFER_H
#define _CONTROLTRANSFER_H

int controlDirect(uint8_t* text, int curr, int id);

int jumpShort(uint8_t* text, int curr);

int controlSimpleCommands(uint8_t* text, int curr, int id);

int conditionalJump(uint8_t* text, int curr);

int conditionalLoop(uint8_t* text, int curr);

int intTypeSpec(uint8_t* text, int curr);

int escape(uint8_t* text, int curr);

#endif
