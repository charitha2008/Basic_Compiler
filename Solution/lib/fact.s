.data
	
.globl	fact
	
.text
fact:
    	movq $0, %rbx
    	movq $1 ,%rax
    	inc %rbx
Loop:  
	imulq %rbx
    	inc %rbx
    	cmp  %rdi,%rbx
	jle  Loop
End:
	ret
	



