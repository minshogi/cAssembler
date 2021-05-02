/* 
 * Project1 - Computer Architecture (Summer 2020)
 * @file name   assembler.c
 * @author      KWONMINSUK(minshogi@gmail.com) 2014003927
 * @date        2020/05/22
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define MAXLINELENGTH 1000

char labels[51][MAXLINELENGTH] = {0};
int PC = 0;
int max_addr = 51;
int endline = 51;
char* oplist[9] = {"add", "nor", "lw", "sw", "beq", "jalr", "halt", "noop", ".fill"};

int readAndParse (FILE* , char *, char *, char *, char *, char *);
int isNumber (char *);

int add(char *, char *, char *);
int nor(char *, char *, char *);
int lw(char *, char *, char *);
int sw(char *, char *, char *);
int beq(char *, char *, char *);
int jalr(char *, char *);
int halt();
int noop();
int fill(char *, char *);

void check_op(char *);

int labelAddr(char *);
int16_t get_os(char *);
int16_t branch(char *);

int main(int argc, char *argv[])
{
        char *inFileString, *outFileString;
        FILE *inFilePtr, *outFilePtr;
        char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
        
   
        if (argc != 3) 
        {
                printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
                exit(1);
        }

        inFileString = argv[1];
        outFileString = argv[2];
        inFilePtr = fopen(inFileString, "r");
        if (inFilePtr == NULL)
        {
                printf("error in opening %s\n", inFileString);
                exit(1);
        }

        outFilePtr = fopen(outFileString, "w+");
        if (outFilePtr == NULL)
        {
                printf("error in opening %s\n", outFileString);
                exit(1);
        }

        PC = 0;
        while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))   
        {
            if (labelAddr(label) != -1)
            {   
                    printf("error: duplicated label \n%s\n", label);
                    exit(1);
            }
            check_op(opcode);
            strcpy(labels[PC], label);
			if(!strcmp(opcode, "halt")) endline=PC;
            PC++;
        }
        max_addr = PC;                

        
		PC=0;
		rewind(inFilePtr);
        
        while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) )
        {
                if(!strcmp(opcode,"add")) fprintf(outFilePtr, "%d\n", add(arg0, arg1, arg2));
                if(!strcmp(opcode,"nor")) fprintf(outFilePtr, "%d\n",nor(arg0, arg1, arg2));
                if(!strcmp(opcode,"lw")) fprintf(outFilePtr, "%d\n",lw(arg0, arg1, arg2));
                if(!strcmp(opcode,"sw")) fprintf(outFilePtr, "%d\n",sw(arg0, arg1, arg2));
                if(!strcmp(opcode,"beq")) fprintf(outFilePtr, "%d\n",beq(arg0, arg1, arg2));
                if(!strcmp(opcode,"jalr")) fprintf(outFilePtr, "%d\n",jalr(arg0, arg1));
                if(!strcmp(opcode,"halt")) fprintf(outFilePtr, "%d\n",halt());
                if(!strcmp(opcode,"noop")) fprintf(outFilePtr, "%d\n",noop());
                if(!strcmp(opcode,".fill")) fprintf(outFilePtr, "%d\n",fill(label, arg0));
        		PC++;
		}

	fclose(inFilePtr);
	fclose(outFilePtr);
	exit(0);
}

/* Read and parse a line of the assembly-language file. 
 * Fields are returned in label, opcode, arg0, arg1, arg2 (these strings must have memory already allocated to them).
 *
 * Return values:
 *  0 if reached end of file
 *  1 if all went well
 *
 * exit(1) if line is too long.
 */

int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
        char line[MAXLINELENGTH];
        char *ptr = line;
        /* delete prior values*/

        label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

        /*read the line from the assembly-language file*/
        if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
        {
                /*reached end of filr*/
                return(0);
        }
        /*check for line too long (by looking for a \n)*/
        if (strchr(line, '\n') == NULL)
        {
                /*line too long*/
                printf("error: line too long\n");
                exit(1);
        }
        /*if there a label?*/
        ptr = line;
        if (sscanf(ptr, "%[^\t\n\r ]", label))
        {
                /*successfully read label: advance pointer over the label*/
                ptr += strlen(label);
        }

        /*
         * Parse the rest of the line. 
         * Would be nice to have real regular expressions, but scanf will suffice.
         */
        sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
        return (1);
}

int isNumber(char *string)
{
        /* return 1 if string is a number */
        int i;
        return( (sscanf(string, "%d", &i)) == 1);
}


void regErr(char *reg)
{
        int r = atoi(reg);
        if(!isNumber(reg) || r<0 || r>=8)
        {
            printf("error: register out of range\n%s\n", reg);
            exit(1);
        }
}

void offsetErr(char *off)
{
        int r = atoi(off), f = labelAddr(off);
        if(isNumber(off))
        {
        	if(r<INT16_MIN || r>INT16_MAX)
       		{
       			printf("error: offsetField don't fit in 16 bits\n%s\n", off);
       			exit(1);
       		}
		}
        else
        {
            if(f == -1)
            {
                printf("error: undefined label \n%s\n", off);
                exit(1);
            }
       }
}


int add(char *regA, char *regB, char *destReg)
{
    int ret=0, ra, rb, dest;
    regErr(regA);
    regErr(regB);
    regErr(destReg);

    ra = atoi(regA);
    rb = atoi(regB);
    dest = atoi(destReg);
    ret |= (ra<<19);
    ret |= (rb<<16);
    ret |= dest;
    return ret;
}

int nor(char *regA, char *regB, char *destReg)
{
    int ret=0, ra, rb, dest;
    regErr(regA);
    regErr(regB);
    regErr(destReg);

    ra = atoi(regA);
    rb = atoi(regB);
    dest = atoi(destReg);
    ret |= (1<<22);
    ret |= (ra<<19);
    ret |= (rb<<16);
    ret |= dest;
    return ret;
}

int lw(char *regA, char *regB, char *offset)
{
    int ret=0, ra, rb;
    uint16_t os=0;
    regErr(regA);
    regErr(regB);
    offsetErr(offset);

    ra = atoi(regA);
    rb = atoi(regB);
    os |= get_os(offset);
    ret |= (2<<22);
    ret |= (ra<<19);
    ret |= (rb<<16);
	ret |= os;
	return ret;
}
int sw(char *regA, char *regB, char *offset)
{
    int ret=0, ra, rb;
    uint16_t os=0;
    regErr(regA);
    regErr(regB);
    offsetErr(offset);
    ra = atoi(regA);
    rb = atoi(regB);
    os |= get_os(offset);
    ret |= (3<<22);
    ret |= (ra<<19);
    ret |= (rb<<16);
	ret |= os;
    return ret;
}

int beq(char *regA, char *regB, char *offset)
{
    int ret=0, ra, rb;
    uint16_t os=0;
    regErr(regA);
    regErr(regB);
    offsetErr(offset);
    ra = atoi(regA);
    rb = atoi(regB);
    os |= branch(offset);
    ret |= (4<<22);
    ret |= (ra<<19);
    ret |= (rb<<16);
	ret |= os;
	return ret;

}
int jalr(char *regA, char *regB)
{
    int ret=0, ra, rb;
    regErr(regA);
    regErr(regB);
    ra = atoi(regA);
    rb = atoi(regB);
    ret|=(5<<22);
    ret|=(ra<<19);
    ret|=(rb<<16);
    return ret;

}
int halt()
{
    int ret=0;
    ret |= (6<<22);
    return ret;
}
int noop()
{
    int ret=0;
    ret |= (7<<22);
    return ret;
}

int labelAddr(char *label)
{
    if(label[0] == '\0') return -1;
    for(int i=0; i<max_addr; i++)
    {
        if(!strcmp(labels[i], label)) return i;
    }
    return -1;
}

int fill(char *label, char *offset)
{
        int ret;
       	long long x;
        if(isNumber(offset))
		{
				x = strtoll(offset, NULL, 10);
				if(x>INT32_MAX || x<INT32_MIN)
				{
					printf("error: numeric value don't fit in 32 bits\n%s\n",offset);
					exit(1);
				}
				ret = atoi(offset);
		}
		else
    	{
                ret = labelAddr(offset);
                if(ret == -1)
                {
                    printf("error: undefined label\n%s\n", offset);
                    exit(1);
                }
        }

       	return ret;
        
}

void check_op(char *opcode)
{
    for(int i=0; i<9; i++)
            if(!strcmp(opcode, oplist[i])) return;
    printf("error: unrecognized opcode\n%s\n", opcode);
    exit(1);
}

int16_t get_os(char *offset)
{
	int16_t ret = labelAddr(offset);
    if(isNumber(offset)) return ret=atoi(offset);
    return ret;
}

int16_t branch(char *offset)
{
	int16_t ret = labelAddr(offset);
    if(isNumber(offset)) return ret=atoi(offset);
    if(ret<=endline) ret = -(PC+1-ret);
	return ret;	
}
