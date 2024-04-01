; Basic assembly for every chs compilation
; Call it libchs if you will

BITS 64

; Register naming
; R = r11
; G = r12
; B = r13
; Y = r14
; T = r15 (temporary reg)

section .data
chs_io_buff:
	db 0

chs_mem:
	times 256 db 0

section .text
global _start

; Auxiliar subroutines
chs_out:
	mov [chs_io_buff], r11
	mov rax, 1 ; write
	mov rdi, 1 ; stdout
	mov rsi, chs_io_buff ; buf
	mov rdx, 1 ; count
	syscall
	ret

chs_in:
	mov rax, 0 ; read
	mov rdi, 0 ; stdin
	mov rsi, chs_io_buff ; buf
	mov rdx, 1 ; count
	syscall
	mov r11, [chs_io_buff]
	ret

exit:
	mov rax, 60
	xor rdi, rdi
	syscall

_start:
chs_setup:
	xor r11, r11
	mov r12, r11
	mov r13, r11
	mov r14, r11
	mov r15, r11

; ===Actual code below===
