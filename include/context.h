//
// Created by Dylan Caldwell on 10/13/23.
//

#ifndef FIJI_CONTEXT_H
#define FIJI_CONTEXT_H


struct context {
    unsigned int  ds, es, fs, gs, ss, eax, ebx, ecx, edx, esi, edi, ebp, esp, eip, cs, eflags;
};


#endif //FIJI_CONTEXT_H
