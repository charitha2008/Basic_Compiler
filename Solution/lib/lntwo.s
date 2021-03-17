.data

.text	

.globl	lntwo
intwo:
	cmpq $0, %rdi
	je invalid

	cmpq $1, %rdi
	je one

	movq $1, %rax
	movq $2, %rbx
	movq $0, %rcx

loop:
	imulq %rbx
	addq $1, %rcx

	cmpq %rdi, %rax
	jl loop
	je end
	jg greater

greater:
	subq $1, %rcx
	jmp end

one:
	movq $0, %rcx
	jmp end

invalid:
	movq $1, %rcx
	negq %rcx

end:	
	movq %rcx, %rax
	ret


