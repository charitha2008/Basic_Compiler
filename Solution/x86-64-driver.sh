#!/bin/bash


if [ $# -eq 0 ]; then
    echo "ERROR! Please Pass in the .calc file"
    exit 1
fi

calcFile="$1"
if [[ ! "${calcFile: -5}"  == ".calc" ]]; then
	echo "ERROR! File without .calc Extension"
	exit 1
else
	echo "Received File Name: $calcFile" 
	
	BASE_NAME=$(basename $1)
	FILE_NAME="${BASE_NAME: : -5}"
	SCRIPT_DIR=$(dirname $(readlink -f $0))
	ASSEMBLY_SOURCE=$SCRIPT_DIR/src/$FILE_NAME
fi


ASM_CODE="$(echo $(cat $1) | ./bin/calc3i.exe)"			# generate assembly code


if [ "$ASM_CODE" != "" ]; then	

	echo '' > $ASSEMBLY_SOURCE.s


	#prologue	
	echo \
	'.section .data
		format: .string "%ld\n"

	.section .text
		.globl main

	main:
		#Save base pointer and set the stack frame
		pushq %rbp
		movq %rsp, %rbp

		sub $192, %rsp		# Space for 26 variables, each of 8 bytes
	' >> $ASSEMBLY_SOURCE.s

	echo "$ASM_CODE" >> $ASSEMBLY_SOURCE.s


	#epilogue
	echo \
	'	
		#Systemcall exit(0)
		movq $60, %rax
		movq $0, %rdi
		syscall

		#Cleanup
		movq %rbp, %rsp
		popq %rbp
		ret
	' >> $ASSEMBLY_SOURCE.s


	if [ -e $ASSEMBLY_SOURCE.s ]; then					# Check if ASM file is created
	    echo "SUCESS CREATED ASM FILE $ASSEMBLY_SOURCE.s"
	fi

	gcc  -o  "$SCRIPT_DIR/bin/$FILE_NAME" "$ASSEMBLY_SOURCE.s" $SCRIPT_DIR/lib/fact.s $SCRIPT_DIR/lib/gcd.s $SCRIPT_DIR/lib/lntwo.s  -g -no-pie


	if [ -e "$SCRIPT_DIR/bin/$FILE_NAME" ]; then			# Check if executiable is created
	    echo "SUCESS BUILDING FILE $SCRIPT_DIR/bin/$FILE_NAME"
	fi

fi
