#include <stdio.h>
#include "data.h"
#include "./structs/dictionary.h"

int errorFlag;
char *savedCommands[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
char *group1[] = {"mov", "cmp", "add", "sub", "lea"};
char *group2[] = {"not", "clr", "inc", "dec", "jmp", "bne", "get", "prn", "jsr"};
char *group3[] = {"rts", "hlt"};
char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char *savedInstructs[] = {".data", ".struct", ".string", ".entry", ".extern"};


/* 
 * Assignment mmn 14 - the c lab project
 * assembler for assembly, that can take files and outputs .ob, .ext, .ent files
 * @author Daniel Dubinskey 
 * @author Yonatan Tzukerman 
 * @date 21/8/2022
*/

int main(int argc, char *argv[]) {
	int i;
	errorFlag = FALSE;
	
	
	if (argc == 1) {
		printf("Oops, you forgot to mention the files!\n");
		return 1;
	}
	
	for (i = 1; i < argc; i++) {
		printf("Going over file - %s\n", argv[i]);
	
		nicePrint("Pre Asm", NULL);
		preASM(argv[i]);
		nicePrint("Part 1", NULL);
		asmPart1(argv[i]);
		nicePrint("Part 2", NULL);
		asmPart2(argv[i]);
		
		printf("I'm done with file - %s\n\n", argv[i]);
	}
	if (errorFlag) 
		printf("It appears your run had some errors :(\nTherefor I didn't compile the code\n");
	else 
		printf("No errors found. code compiled successfully.\n");
	
	return 0;
	
}
