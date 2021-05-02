/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000
#define MAXINSTRUCTION 1000
typedef struct stateStruct
{
		int pc;
	    int mem[NUMMEMORY];	
		int reg[NUMREGS];
	    int numMemory;
} stateType;

void printState(stateType *);

void init(stateType *);
void add(stateType *);
void nor(stateType *);
void lw(stateType *);
void sw(stateType *);
void beq(stateType *);
void jalr(stateType *);
void halt(stateType *);
void noop(stateType *);

int running(stateType *);
int get_opcode(int);

int main(int argc, char* argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	int total=0;
	FILE* filePtr;

	if (argc != 2) 
	{	
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
	}
	filePtr = fopen(argv[1], "r");
	if (filePtr == NULL) 
	{
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
	}
	
	init(&state);
	/* read in the entire machine-code file into memory */
	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) 
	{
		if (sscanf(line, "%d", state.mem + state.numMemory) != 1) 
		{
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
	}

	for (int i=0; i<MAXINSTRUCTION; i++)
	{
			total++;
			printState(&state);
			if(!running(&state))
			{
				printf("total of %d instructions executed\n", total);
				printf("final state of machine:\n");
				printState(&state);
				return 0;
			}
	}
	printf("error: too many instructions\n");
	return(0);
}

void printState(stateType* statePtr)
{
	int i;
	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");
	for (i = 0; i < statePtr->numMemory; i++) {
		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
	printf("\tregisters:\n");
	for (i = 0; i < NUMREGS; i++) {
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
	printf("end state\n");
}

int get_opcode(int mem)
{
	return ((mem>>22)&7);
}

int regA(int mem)
{
	return ((mem>>19)&7);
}

int regB(int mem)
{
	return ((mem>>16)&7);
}

int destReg(int mem)
{
	return mem&7;
}

int offset(int mem)
{
	unsigned int tmp1 = -1;
	int16_t ret = (mem & (tmp1>>16));
	return ret;
}



void init(stateType *state)
{
		state->pc=0;
	    memset(state->mem, 0, sizeof(state->mem));
	    memset(state->reg, 0, sizeof(state->reg));
	    state->numMemory=0;
}
void add(stateType *S)
{
	int mem, ra, rb, rd;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);
	rd = destReg(mem);
	S->reg[rd] = S->reg[ra] + S->reg[rb];
	S->pc++;
}

void nor(stateType *S)
{
	int mem, ra, rb, rd;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);
	rd = destReg(mem);
	S->reg[rd] = ~(S->reg[ra] | S->reg[rb]);
	S->pc++;
}
void lw(stateType *S)
{
	int mem, ra, rb, os, addr;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);
	os = offset(mem);
	addr = S->reg[ra] + os;
	S->reg[rb] = S->mem[addr];
	S->pc++;
}
void sw(stateType *S)
{
	int mem, ra, rb, os, addr;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);
	os = offset(mem);
	addr = S->reg[ra] + os;
	S->mem[addr] = S->reg[rb];
	S->pc++;
}

void beq(stateType *S)
{
	int mem, ra, rb, os;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);
	os = offset(mem);

	if(S->reg[ra] == S->reg[rb]) S->pc += os+1;
	else S->pc++;
}
void jalr(stateType *S)
{
	int mem, ra, rb;
	mem = S->mem[S->pc];
	ra = regA(mem);
	rb = regB(mem);

	S->reg[rb] = S->pc + 1;
	S->pc = S->reg[ra];
}
void halt(stateType *S)
{
	S->pc++;
	printf("machine halted\n");
}
void noop(stateType *S)
{
	S->pc++;
}


int running(stateType *S)
{
		int op = get_opcode(S->mem[S->pc]);
		if(op==0) add(S);
		else if(op==1) nor(S);
		else if(op==2) lw(S);
		else if(op==3) sw(S);
		else if(op==4) beq(S);
		else if(op==5) jalr(S);
		else if(op==6)
		{
				halt(S);
				return 0;
		}
		else if(op==7) noop(S);
		else 
		{
				printf("error: opcode %d\n", op);
				return 0;
		}
		return 1;
}
