#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "./structs/dictionary.h"

extern char **commands, **instructions;
extern dictNode *symbols, *externs, *entries;
extern int finalIC, finalDC, SymbolFlag;
extern int errorFlag;

dictNode *externSymbols;

void readLines2(FILE *fRead);
void printSecondRes(char *filePath);
void writeOb(char *filePath);
void writeEnt(char *filePath);
void writeExt(char *filePath);
void writreDict(char *fileOut, dictNode *dict);

/* the function is used to open the am file after the 1st run */
void asmPart2(char *filePath) {
	char *fileIn = (char *)calloc(STANDARD_SIZE,sizeof(char));
    FILE *fRead;
    externSymbols = NULL;
    
    strcpy(fileIn, filePath);
    strcat(fileIn, ".am");
    fRead = fopen(fileIn, "r");
    if(!fRead){
        raiseError("opening/creating file");
        return; 
    }
	readLines2(fRead);
	if (!errorFlag)
		printSecondRes(filePath);
		
	free(fileIn);
}
/*
	main function of 2nd run of assembler
	reads each line and dealing with it in the appropriate way
*/
void readLines2(FILE *fRead) {
	char *token, *line, buffer[MAX_LINE], temp[MAX_LINE], bufferWithoutSpaces[MAX_LINE]; 
	int IC = IC_DC_START, DC = IC_DC_START;

	while (fgets(buffer, MAX_LINE, fRead)) {
		line = formatCommand(buffer);
		strcpy(temp, line);
		token = strtok(temp, " ");
		strcpy(bufferWithoutSpaces, buffer);
		removeWSpaces(bufferWithoutSpaces);
		
		/* if token is a symbol   */
		if (!token || token[0] == ';' || *bufferWithoutSpaces == 0) {
			continue;
		}
		else if (token && token[strlen(token) - 1] == ':') {
			free(line);
			line = formatCommand(buffer + strlen(token));
			strcpy(temp, line);
			token = strtok(temp, " ");
		}
		/* if its a command */
		if (token && stringInArray(token, savedCommands, SAVED_COMMANDS_LENGTH)) {
			IC += handleCommands2(line, token, IC);
		}
		/* if its an instruction */
		else if (token && stringInArray(token, savedInstructs, SAVED_INSTRUCTS_LENGTH)) {
			handleInstructs2(line, token, &DC);
		}
		/* clear the buffer */
		memset(buffer, 0, MAX_LINE);
		free(line);
	}
}

/* print the results after second run */
void printSecondRes(char *filePath) {
	/*int i;
	
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
	printing the entries and externs list if not null	
	if (entries)
		nicePrint("Entries",entries);
	if (externSymbols)
		nicePrint("Externals",externSymbols);
	*/
	/* in case no error create then we create the ent and axt files*/
	if (!errorFlag) {
		writeOb(filePath);
		if (entries)
			writeEnt(filePath);
		if (externSymbols)
			writeExt(filePath);
	}
}
/* 
	this functioon creates the object file and write everything needed to it
	in base 32 according to the task
*/ 
void writeOb(char *filePath) {
    char *fileOut = (char *)calloc(MAX_LINE,sizeof(char));
    int i;
	FILE *fWrite;

	strcpy(fileOut, filePath);
    strcat(fileOut, ".ob");
    fWrite = fopen(fileOut, "w");
    if(!fWrite){
        raiseError("opening/creating file");
        return; 
    }
	
	fprintf(fWrite, "%s %s\n", decToBase32(finalIC - IC_DC_START), decToBase32(finalDC - IC_DC_START));
	for (i = 0; i < finalIC; i++) {
		if (commands[i] != NULL) {
			fprintf(fWrite, "%s	\t %s\n", decToBase32(i), binToBase32(commands[i]));
			free(commands[i]);
		}
	}
	for (i = 0; i < finalDC; i++) {
		if (instructions[i] != NULL) {
			fprintf(fWrite, "%s	\t %s\n", decToBase32(i + finalIC - IC_DC_START), binToBase32(instructions[i]));
			free(instructions[i]);
		}
	}
    fclose(fWrite);
    free(fileOut);
}

/* 
	this functioon creates the ent file and write everything needed to it
	in base 32 according to the task
*/ 
void writeEnt(char *filePath) {
	char *fileOut = (char *)calloc(MAX_LINE,sizeof(char));
	
	strcpy(fileOut, filePath);
    strcat(fileOut, ".ent");

	writreDict(fileOut, entries);
}

/* 
	this functioon creates the ext file and write everything needed to it
	in base 32 according to the task
*/ 
void writeExt(char *filePath) {
	char *fileOut = (char *)calloc(MAX_LINE,sizeof(char));
	
	strcpy(fileOut, filePath);
    strcat(fileOut, ".ext");

	writreDict(fileOut, externSymbols);
}

void writreDict(char *fileOut, dictNode *dict){
	FILE *fWrite;
	dictNode *ptr = dict;
	
    fWrite = fopen(fileOut, "w");
  	if (!fWrite) {
        raiseError("opening/creating file");
        return; 
    }
    
	while (ptr != NULL) {
		fprintf(fWrite, "%s \t %s\n", getKey(ptr), getValue(ptr));
		ptr = getNext(ptr);
	}
	freeDict(&dict);
	free(fileOut);
}
