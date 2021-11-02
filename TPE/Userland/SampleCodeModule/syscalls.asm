GLOBAL sys_getChar

SECTION .text

%macro pushState 0
	;push rax Chequear si rax es necesario
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	; pop rax Chequear si rax es necesario
%endmacro

; User-level applications use as integer registers for passing 
; the sequence %rdi, %rsi, %rdx, %rcx, %r8 and %r9
; The kernel interface uses %rdi, %rsi, %rdx, %r10, %r8 and %r9
; FUENTE: https://stackoverflow.com/questions/2535989/what-are-the-calling-conventions-for-unix-linux-system-calls-and-user-space-f

; Linux Order: rax, rdi, rsi, rdx, r10, r8, r9

%macro syscall 1
    pushState

    mov rax, %1     ; Cargo en rax el numero de syscall
    int 80h         

    popState
    ret             ; es necesario?
%endmacro

sys_getChar:
    syscall 0

