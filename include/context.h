//
// Created by Dylan Caldwell on 10/13/23.
//

#ifndef FIJI_CONTEXT_H
#define FIJI_CONTEXT_H


struct context {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp, esp;
    unsigned int eflags, eip, cs, ds, es, fs, gs, ss;
};


#endif //FIJI_CONTEXT_H
