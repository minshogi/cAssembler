	lw 0 1 five	load reg1 with 5 
	lw 1 2 19	load reg2 with -1
	lw 0 3 one	load reg3 with 1
	nor 1 3 4	reg4 has reg1 nor reg3 : -6
start	add 1 2 1	decrement reg1
	beq 0 1 1	goto next exp if reg1 == 0
	beq 0 0 start	go back to the beginning of the loop
	noop
	sw 3 4 hundred	store reg4 into memory address 101
	lw 0 1 br1Addr	load reg1 with address of branch1: 11
	lw 0 2 doneAddr	load reg2 with address of done: 19
branch1	add 3 4 4	increment reg4
	beq 0 4 branch2	goto branch2 if reg4 == 0
	jalr 1 5	load reg5 with pc+1:14 and goto branch1
	jalr 2 3	load reg3 with pc+1:15 and goto done
branch2	jalr 6 6	load reg6 with pc+1:16 and goto next
	lw 3 7 hundred	load memory address 101 into reg7
	sw 3 7 101	store reg7 into memory address 102
	lw 3 4 101	load memory address 102 into reg4
	jalr 5 6	load reg6 with pc+1: 19 and goto addr 14
	noop
done	halt	end of program
five	.fill 5
one	.fill 1
neg1	.fill	-1
hundred	.fill 100
stAddr	.fill start
norVal	.fill -6
br1Addr	.fill branch1
br2Addr	.fill branch2
doneAddr	.fill done
