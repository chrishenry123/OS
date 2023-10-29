#ifndef FIJI_CONTEXT_H
#define FIJI_CONTEXT_H


struct context {
    unsigned int eax, ebx, ecx, edx, esi, edi, ebp;
    unsigned int eip, cs, eflags, esp, ds, es, fs, gs, ss;
};


#endif //FIJI_CONTEXT_H
