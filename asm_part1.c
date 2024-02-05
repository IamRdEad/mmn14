#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "data.h"
#include "./structs/dictionary.h"

char **commands, **instructions;
dictNode *symbols, *externs, *entries;
/* named after the succesful rock band IC\DC*/
int finalIC, finalDC, SymbolFlag;


extern char *savedCommands[];
extern char *savedInstructs[];
extern int errorFlag;


void readLines1(FILE *fRead);
void printFirstRes();
void updateSymbols(dictNode *symbols);
int checkValidSymbol(char *symbol, char *line);
int checkCommas(char *token, char *line);
int handleLabel(char *symbol, char **token, char **line, char* buffer, char temp[]);
void ComANDIns(char *token, char* line, char* symbol, int *IC, int *DC);

/* 
	main function used to open the am file for reating 
*/
void asmPart1(char * filePath) {
    char *fileIn = (char *)calloc(STANDARD_SIZE,sizeof(char));
    FILE *fRead;
    
    commands = (char **)calloc(STANDARD_SIZE, sizeof(char*));
    instructions = (char **)calloc(STANDARD_SIZE, sizeof(char*));
    strcpy(fileIn, filePath);
    strcat(fileIn, ".am");
    fRead = fopen(fileIn, "r");
    
    if(!fRead){
    	errorFlag = TRUE;
        printf("Error: opening/creating file\n");
        return; 
    }
    
	readLines1(fRead);
	updateSymbols(symbols);
	/* printFirstRes(); */
	free(fileIn);
	
}
/*
	this function is the heart of 1st run of assembler
	reads each line and dealing with it in the appropriate way
 */
void readLines1(FILE *fRead) {
	char buffer[MAX_LINE], temp[MAX_LINE], bufferWithoutSpaces[MAX_LINE]; 
	char *token, *line, *symbol = (char *)calloc(MAX_LINE,sizeof(char));
	int DC = IC_DC_START, IC = IC_DC_START;
	/* tavlat smalim */
	symbols = externs = entries = NULL; 
	SymbolFlag = FALSE;


	while (fgets(buffer, MAX_LINE, fRead)) {
		/* copy buffer into temp. then format buffer in line.
		token now takes the first argument (symbol or command) */
		line = formatCommand(buffer);
		strcpy(temp, line);
		token = strtok(temp, " ");
		strcpy(bufferWithoutSpaces, buffer);
		removeWSpaces(bufferWithoutSpaces);
		
		/* the line is a comment */
		if (!token || token[0] == ';' || *bufferWithoutSpaces == 0) 
			continue;
			
		/* else, if the line starts with a label */
		else if (token[strlen(token) - 1] == ':') 
			if (!handleLabel(symbol, &token, &line, buffer, temp))
				continue;
		/* in case the token is command or Instruction*/
		ComANDIns(token, line, symbol, &IC, &DC);
		
		/*clear the strings*/
		SymbolFlag = FALSE;
		memset(buffer, 0, MAX_LINE);
		memset(symbol, 0, MAX_LINE);
		memset(token, 0, MAX_LINE);
		free(line);
	}
	free(symbol);
	
	finalIC = IC;
	finalDC = DC;
}

int handleLabel(char *symbol, char **token, char **line, char *buffer, char temp[] ){
	SymbolFlag = TRUE;
	strncpy(symbol, *token, strlen(*token) - 1);
	if (!checkValidSymbol(*token, *line))  {
		return 0;
	}
	free(*line);
	*line = formatCommand(buffer + strlen(*token));
	strcpy(temp, *line);
	*token = strtok(temp, " ");
	return 1;
}

void ComANDIns(char *token, char *line, char *symbol, int *IC, int *DC){
	char *addressTemp = (char *)calloc(CHAR_10_BIT, sizeof(char));
	
	if (token && stringInArray(token, savedCommands, SAVED_COMMANDS_LENGTH)) {
		if (checkCommas(token, line)) {
			raiseError("Illegal amout of opreands");
		}
		else {
			if (SymbolFlag)
				addNode(&symbols, symbol, itoa(*IC, 10), 0);
			*IC += handleCommands1(line, token, *IC);
		}
	}
	/* in case the token is instruction*/
	else if (token && stringInArray(token, savedInstructs, SAVED_INSTRUCTS_LENGTH)) {
		/*printf("going into instractions\n");*/
		if (SymbolFlag) {
			strcpy(addressTemp, itoa(*DC, 10));
			addNode(&symbols, symbol, strcat(addressTemp, STAM_CHAR), 0);
		}
		handleInstructs1(line, token, DC);
	} 
	else {
		errorFlag = TRUE;
		printf("Error: unknown command '%s'\n", line);
	}
	
	
}

/* print the results after the first run */
void printFirstRes() {
	int i;
	if (finalIC - IC_DC_START) {
		nicePrint("Commands", NULL);
		for (i = 0; i < finalIC; i++) {
			if (commands[i] != NULL) {
				printf("i = %d 		bin = %s\n", i, commands[i]);
			}
		}
	}
	
	if (finalDC - IC_DC_START) {
		nicePrint("Instructions", NULL);
		for (i = 0; i < finalDC; i++) {
			if (instructions[i] != NULL) {
				printf("i = %d 		bin = %s\n", i, instructions[i]);
			}
		}
	}
	if (symbols)
		nicePrint("Symbols",symbols);
	if (entries)
		nicePrint("Entries",entries);
	if (externs)
		nicePrint("Externals",externs);
}

/* updating a symbol list*/
void updateSymbols(dictNode *symbols) {
	char *value ,*temp = (char *)calloc(CHAR_10_BIT, sizeof(char));
	dictNode *ptr = symbols;
	int n;

	while (ptr != NULL) {
		value = getValue(ptr);
		if (strstr(value, STAM_CHAR)) {
			strncpy(temp, value, strlen(value)-1);
			n = (atoi(temp) + finalIC - IC_DC_START);
			strcpy(value, itoa(n, 10));
		}
		ptr = getNext(ptr);
	}
	free(temp);
}

/* returns if the amount of opreads in command is legal*/ 
int checkCommas(char *token, char *line){
	int res = 1, commas = countCommas(line);

	if (stringInArray(token, group1, GROUP1_LENGTH)) {
		if (commas == 1)
			res = 0;	
	}
	else {
		if (commas == 0)
			res = 0;
	}
	return res;
}

int checkValidSymbol(char *symbol, char *line) {
	int valid = TRUE;
	/* AI CODE*/
	if (findValue(&externs, symbol)) {
		printf("Error: label - %s, is already declared as extern and can't be defined\n", symbol);
		valid = FALSE;
	}
	else if (findValue(&symbols, symbol)) {
		printf("Error: label - %s, is already defined can't be re-defined\n", symbol);
		valid = FALSE;
	}
	/*lines ends with \n token ends with \0 strcmp gives 32 if no label exists*/
	else if (strcmp(line, symbol) == 32 && strlen(line)<= strlen(symbol)+1) {
		raiseError("Missing arugments after label");
		valid = FALSE;
	}
	else if (!isalpha(symbol[0])) {
		printf("Error: the string - %s, isn't a legal label\n", symbol);
		valid = FALSE;
	}
	else if (strlen(symbol) > 31) {
		printf("Error: label - %s, is too long. max of 30 letters per label\n", symbol);
		valid = FALSE;
	}
	else if (isSavedWord(symbol)) {
		printf("Error: label - %s, is a saved word\n", symbol);
		valid = FALSE;
	}
	
	if (!valid)
		errorFlag = TRUE;
	return valid;
}
