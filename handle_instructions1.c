#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "data.h"
#include "./structs/dictionary.h"


void dataInst(char *buffer, char *token, int *DC);
void stringInst(char *buffer, char *token, int *DC);
void structInst(char *buffer, char *token, int *DC);
void entryInst(char *buffer, char *token, int *DC);
void externInst(char *buffer, char *token, int *DC);

void structInstInt(char *buffer, char *number, int *DC);
void processString(char *buffer, char *token, int *DC);
void initializeInstructionsArr(int DC);

extern int SymbolFlag;
extern char **instructions;
extern dictNode *entries, *externs, *symbols;
extern int errorFlag;

/* This function checks what type of instrction is enterned and sends to the relvant function. */
void handleInstructs1(char *buffer, char *token, int *DC) {

	if (!strcmp(token, ".data"))
		dataInst(buffer, token, DC);
	else if(!strcmp(token, ".string")) 
		stringInst(buffer, token, DC);
	else if(!strcmp(token, ".struct")) 
		structInst(buffer, token, DC);
	else if(!strcmp(token, ".entry"))
		entryInst(buffer, token, DC);
	else if (!strcmp(token, ".extern")) 
		externInst(buffer, token, DC);
	else 
		raiseError("unknown instruction");
}

/* 
 * For each of the arguments in the data.
 * The function convert is to the binary represention and insert to the insturaction array.
 */ 
void dataInst(char *buffer, char *token, int *DC) {
	int limit = pow(2, INSTRUCTION_INT_REP_SIZE);
	char *temp, *binNum;
	
	/* In case no arguments buffer and token are  euqal. */
	if (strcmp(buffer, token) == 32 && strlen(buffer)<= strlen(token)+1){
		raiseError("missing arguments after data");
		return;
	}
	
	/* get the first number */
	token = strtok(NULL, ",");
	while (token) {
		/* error handling */
		if (!isNumber(token))
			raiseError("יוסטון we have a problem! out number isn't valid"); /* left here as an easter egg */
		else if (atoi(token) >= limit || atoi(token) < limit * -1)  
			raiseError("number too big / small");
			
		/*add the number to the array adding zero is needed in case number postive*/
		else {
			if (atoi(token) >= 0)  {
				binNum = decToBin(atoi(token), 0);
				temp = addingZero(binNum, INSTRUCTION_INT_REP_SIZE);
				free(binNum);
			} 
			else 
				temp = decToBin(atoi(token), INSTRUCTION_INT_REP_SIZE);

			/* Initialize the array and copy the number */
			initializeInstructionsArr(*DC); 
			strcpy(instructions[(*DC)++], temp);
		}	
		token = strtok(NULL, ",");
	} 
}

/* this function takes care of string instructions */
void stringInst(char *buffer, char *token, int *DC) {
	
	token = strstr(buffer, "\"") + 1;
	/* checks is there is a valid argument */
	if (!strstr(buffer, "\"") || token == (buffer + strlen(buffer))) {
		raiseError("missing string in .string");
		return;
	}
	
	processString(buffer, token, DC);
}

/* this function takes care of struct instructions */
void structInst(char *buffer, char *token, int *DC) {
	char *number = (char *)calloc(MAX_LINE, sizeof(char));
	
	/* in case no arguments buffer and token are  euqal*/
	if (strcmp(buffer, token) == 32 && strlen(buffer)<= strlen(token)+1){
		raiseError("missing arguments after struct");
		return;
	}
	
	/*get command after the .sturct*/
	token = strtok(buffer, ",");
	/* process the number */
	strncpy(number, token + strlen(".struct "), strlen(token) - strlen(".struct "));
	structInstInt(buffer, number, DC);
	
	token = strtok(NULL, ",");
	if (!token || !strstr(token, "\"") || !strstr(strstr(token, "\"") + 1, "\"")) {
		raiseError("missing string in .struct");
		return;
	}
	/* get the string */
	processString(buffer, ++token, DC);
	
	
	free(number);
}

/* this function takes care  if adding entries if found*/
void entryInst(char *buffer, char *token, int *DC) {
	char *name = (char *)calloc(MAX_LINE, sizeof(char));
	char *nameP = name;
	
	name = strtok(buffer, " ");
	name = strtok(NULL, " ");
	if (name)
		addNode(&entries, name, STAM_CHAR, 0);
	else 
		raiseError("missing label after .entry");
	
	if (SymbolFlag)
		raiseError(".entry should have no label before it");
		
	free(nameP);
}

/* this function takes care  if adding externs if found*/
void externInst(char *buffer, char *token, int *DC) {
	char *name = (char *)calloc(MAX_LINE, sizeof(char));
	char *nameP = name;
	
	name = strtok(buffer, " ");
	name = strtok(NULL, " ");
	if (name) 
		addNode(&externs, name, STAM_CHAR, 0);
	else 
		raiseError("missing label after .extern");

	if (SymbolFlag) 
		raiseError(".extern should have no label before it");
	if (name && findValue(&symbols, name)) {
		printf("Error: label - %s, is already defined and can't be declared as extern\n", name);
		errorFlag = TRUE;
	}
	
	free(nameP);
} 

/* handle the int part of the struct. private command to clean up the code */
void structInstInt(char *buffer, char *number, int *DC) {
	char *finalNumBin = (char *)calloc(CHAR_10_BIT, sizeof(char));
	int limit = pow(2, INSTRUCTION_INT_REP_SIZE);
	char *temp, *binNum;
	
	/* struct template is (number, string) so we first get the number. else throw error */
	if (!isNumber(number)) { 
		printf("Error: %s isn't a legal number.\n", number);
		errorFlag = TRUE;
	}
	else if (atoi(number) >= limit || atoi(number) < limit * -1)  
			raiseError("number too big / small");
	else {
		if (atoi(number) >= 0) {
			binNum = decToBin(atoi(number), 0);
			temp = addingZero(binNum, INSTRUCTION_INT_REP_SIZE);
		} 
		else 
			temp = decToBin(atoi(number), INSTRUCTION_INT_REP_SIZE);
		strcpy(finalNumBin, temp);
	}
	initializeInstructionsArr(*DC);
	strcpy(instructions[(*DC)++], finalNumBin);
}

/* insert string into instructions array */
void processString(char *buffer, char *token, int *DC) {
	char *value = (char *)calloc(MAX_LINE, sizeof(char)); 
	char *temp, *binNum;
	int i = 0, ascii;
	
	/* for each of the charcter till the end of the string copy it to instruction list*/	
	while (token[i] && token[i] != '\"') {
		ascii = token[i++];
		binNum = decToBin(ascii, 0);
		temp = addingZero(binNum, INSTRUCTION_INT_REP_SIZE);
		initializeInstructionsArr(*DC); 
		strcpy(value, temp);
		strcpy(instructions[(*DC)++], value);
		free(binNum);
		free(temp);
	}
	if (!token[i])
		raiseError("ilegal string");
	
	/*add terminating null*/
	initializeInstructionsArr(*DC);  
	strcpy(instructions[(*DC)++], "0000000000");
}

/* this function initiazlize the instruction array*/
void initializeInstructionsArr(int DC) {
	if (DC % STANDARD_SIZE == 0)
		instructions = (char **)realloc(instructions, (DC + STANDARD_SIZE) * sizeof(char*));
	instructions[DC] = (char *)calloc(CHAR_10_BIT, sizeof(char));
}

