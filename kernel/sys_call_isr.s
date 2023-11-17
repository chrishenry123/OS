bits 32
global sys_call_isr

;;; System call interrupt handler for Module R3.

extern sys_call         ; The C function that sys_call_isr will call

sys_call_isr:
    ; Push general-purpose and segment registers
    push eax
    push ecx
    push edx
    push ebx
    push ebp
    push esi
    push edi
    push ss
    push ds
    push es
    push fs
    push gs


    ; Push ESP onto the stack
    push esp

    ; Call sys_call C function
    call sys_call


    ; Adjust ESP based on returned value in EAX
    mov esp, eax

    ; Pop segment and general-purpose registers
    pop gs
    pop fs
    pop es
    pop ds
    pop ss
    pop edi
    pop esi
    pop ebp
    pop ebx
    pop edx
    pop ecx
    pop eax


    ; Return from ISR
    iret