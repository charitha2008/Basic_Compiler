.data

.text	

.globl	gcd
gcd:
	
	cmpq $0, %rdi
	je invalid

	cmpq $0, %rsi
	je invalid

	cmpq %rdi, %rsi
	jl less
	jge great


less:
	movq %rsi, %rbx
	jmp loop

great:
	movq %rdi, %rbx

loop:
	movq $0, %rdx
	movq %rdi, %rax
	divq %rbx

	cmpq $0, %rdx
	jne reset

	movq %rsi, %rax
	divq %rbx
	
	cmpq $0, %rdx
	je end

reset:
	subq $1, %rbx
	jmp loop
 


invalid:
	movq $1, %rbx
	negq %rbx

end:
	movq %rbx, %rax
	ret
