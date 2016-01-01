#ifndef IO_H
#define IO_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

typedef struct _Input Input;
typedef struct _Output Output;
struct _Input{
    Output *pOutput;
};

typedef U32 (*InputEvent)(Output *pOutput, U32 uData, const Byte *pData);
struct _Output{
    InputEvent onInput;
};

static inline void Input_Connect(Input *pInput, Output *pOutput){
    pInput->pOutput = pOutput;
}

#endif //IO_H
