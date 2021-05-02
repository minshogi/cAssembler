	lw 0 2 mplier	load reg2 with 10383
	lw 0 3 mcand	load reg3 with 32766
	lw 0 4 neg1	load reg4 with -1
	lw 0 5 neg1 load reg5 with -1
	lw 0 6 tht	load reg6 with 13
start	add 5 6 6	decrement reg6
	add 3 3 3 	left shift to reg3
	add 4 4 4	left shift to reg4
	beq 0 6 1	if reg6==0 loop end
	beq 0 0 start	goto start
	add 1 3 1	reg1: 32766*2^13
	add 2 4 2	reg2: 10383-2^13
	lw 0 3 mcand	load reg3 with 32766
	lw 0 4 neg1	load reg4 with -1
	lw 0 6 ele	load reg5 with 11
st2	add 5 6 6	decrement reg6
	add 3 3 3	left shift reg3
	add 4 4 4	left shift reg4
	beq 0 6 1	if reg6==0 loop end
	beq 0 0 st2	goto st2
	add 1 3 1	reg1: 32766 * (2^13 + 2^11)
	add 2 4 2	reg2: 10383 - (2^13 + 2^11)
	lw 0 3 mcand	load reg3 with 32766
	lw 0 4 neg1	load reg4 with -1
	lw 0 6 sev	load reg6 with 7
st3	add 5 6 6	decrement reg6
	add 3 3 3 	left shift to reg3
	add 4 4 4	left shift to reg4
	beq 0 6 1	if reg6==0 loop end
	beq 0 0 st3	goto st3
	add 1 3 1	reg1: 32766 * (2^13 + 2^11 + 2^7)
	add 2 4 2	reg2: 10383 - (2^13 + 2^11 + 2^7)
	lw 0 3 mcand	load reg3 with 32766
st4	add 5 2 2	decrement reg2
	add 1 3 1	reg1 += 32766
	beq 0 2 1	if reg2==0 loop end
	beq 0 0 st4	goto st4
done	halt	end of program
mcand	.fill 32766
mplier	.fill 10383
one	.fill 1
tht	.fill 13
ele	.fill 11
sev	.fill 7
neg1	.fill -1
