.data


.text
	j MAIN

MAIN: 	add $s1, $s2, $s3
		add $zero, $zero, $zero
		sub $t1, $t2, $t3
		lw $t0, 32($s7)
		sw $s3, 128($0)
		j TARGET
		beq $s7, $zero, FIM
		add $t1, $t2, $t3
TARGET: 	nop
		sll $t7, $s6, 5
		jal FUNC
		


FUNC: nop
	  jr $ra

FIM:



