GLOBAL getRTC

section .text

getRTC:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov rax, rdi

    ;mov al, rdi
    out 70h, al
    in al, 71h
    
    sti
    ret