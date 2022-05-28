GLOBAL sys_getChar
GLOBAL sys_putCharWC
GLOBAL sys_getTime
GLOBAL sys_getRegistersInfo
GLOBAL sys_clear
GLOBAL sys_PrintMem
GLOBAL sys_printCharInPos
GLOBAL sys_raiseInvOpCodeExc
GLOBAL sys_getDeciseconds
GLOBAL sys_saveRegs
GLOBAL sys_ps
GLOBAL sys_getPid
GLOBAL sys_togglePsState
GLOBAL sys_killPs
GLOBAL sys_getSeconds
GLOBAL sys_mem
GLOBAL sys_createProcess
GLOBAL sys_nice
GLOBAL sys_yield
GLOBAL sys_malloc
GLOBAL sys_free

SECTION .text

%macro pushState 0
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
%endmacro

%macro syscall 1
	pushState

	mov	r10, r9 	; arg[5]
	mov	r9, r8 		; arg[4]
	mov r8, rcx  	; arg[3]
    mov rcx, rdx 	; arg[2]
	mov rdx, rsi 	; arg[1]
	mov rsi, rdi 	; arg[0]
	mov rdi, %1 	; Syscall #	
    int 80h

	popState
    ret     
%endmacro

sys_getChar:
    syscall 0
sys_putCharWC:
	syscall 1
sys_getTime:
	syscall 2
sys_clear:
	syscall 3
sys_saveRegs:
	syscall 4
sys_PrintMem:
	syscall 5
sys_printCharInPos:
	syscall 6
sys_getDeciseconds:
	syscall 7
sys_getRegistersInfo:
	syscall 8
sys_ps:
	syscall 9
sys_getPid:
	syscall 10
sys_togglePsState:
	syscall 11
sys_killPs:
	syscall 12
sys_getSeconds:
	syscall 13
sys_mem:
	syscall 14
sys_createProcess:
	syscall 15
sys_nice:
	syscall 17
sys_yield:
	syscall 18
sys_malloc:
	syscall 19	
sys_free:
	syscall 20

;Fuente: https://www.felixcloutier.com/x86/ud
sys_raiseInvOpCodeExc:
	ud2

