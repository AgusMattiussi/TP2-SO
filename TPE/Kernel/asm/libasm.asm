GLOBAL cpuVendor
GLOBAL getRegistersInfo
EXTERN inforeg

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getRegistersInfo:
	push rbp
	mov rbp, rsp

	mov [buffer], rax
	mov [buffer + 1*8], rbx
	mov [buffer + 2*8], rcx
	mov [buffer + 3*8], rdx
	mov [buffer + 4*8], rsi
	mov [buffer + 5*8], rdi
	mov [buffer + 6*8], rsp
	mov [buffer + 7*8], r8
	mov [buffer + 8*8], r9
	mov [buffer + 9*8], r10
	mov [buffer + 10*8], r11
	mov [buffer + 11*8], r12
	mov [buffer + 12*8], r13
	mov [buffer + 13*8], r14
	mov [buffer + 14*8], r15

	mov rax, [rsp] ; RIP.
    mov [buffer + 15*8], rax
	
	mov rdi, buffer
	call inforeg

	mov rsp, rbp
	pop rbp
	ret


section .bss
buffer: resb 128

	
