	lw 0 1 five	load reg1 with 5 (symbolic address)
	lw 1 2 3	load reg2 with -1 (numeric address)
start	add 1 2 1	decrement reg1
	bep 0 1 2	unrecognized opcode
	beq 0 0 start	go back to the beginning of the loop
	noop
done	halt	end of program
five	.fill 5
neg1	.fill -1
stAddr	.fill start
