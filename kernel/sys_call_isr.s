bits 32
global sys_call_isr

;;; System call interrupt handler for Module R3.

extern sys_call         ; The C function that sys_call_isr will call

sys_call_isr:
    ; Push general-purpose and segment registers
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
    push ss
    push gs
    push fs
    push es
    push ds


    ; Push ESP onto the stack
    push esp

    ; Call sys_call C function
    call sys_call


    ; Adjust ESP based on returned value in EAX
    mov esp, eax

    ; Pop segment and general-purpose registers
    pop ds
    pop es
    pop fs
    pop gs
    pop ss
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp


    ; Return from ISR
    iret