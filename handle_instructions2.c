#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "./structs/dictionary.h"

void entryInst2(char *buffer, int *DC);

extern int SymbolFlag, errorFlag;
extern char **instructions;
extern dictNode *entries, *externs, *symbols;

/* this files takes care of the entries in the 2nd run of the assembler*/
void handleInstructs2(char *buffer, char *token, int *DC) {
	if (!strcmp(token, ".entry")) {
		entryInst2(buffer, DC);
	}
}

/*
 * writes down all entries labels inside entries
*/
void entryInst2(char *buffer, int *DC) {
	 char *name = (char *)calloc(MAX_LINE, sizeof(char));
	 char *address, *address32, *nameP = name;
	 name = strtok(buffer, " ");
	 name = strtok(NULL, " ");
	 
	 if(name != NULL){
		if ((address = findValue(&symbols, name))) {
		 	address32 = decToBase32(atoi(address));
		 	setValue(&entries, name,address32);
		 	free(address);
	 		free(address32);
		 }
		 /* is the strcmp returns false that means the entry wasn't defined correctly */
		 else if (strcmp(name ,STAM_CHAR)){
		 	errorFlag = TRUE;
		 	printf("Error: label - %s, is set as entry but never declared\n", name);
		 }
	 }
	 free(nameP);
}