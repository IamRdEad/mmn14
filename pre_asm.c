#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "./structs/dictionary.h"

dictNode* macroDict = NULL;

extern int errorFlag;

void readMacro(char *buffer, FILE *fp);
void writeMacro(char *buffer, FILE **fp);

/*
 * this function recivces the name of the file open it for reading 
 * for each of the line we check if it declare on new macro
 * if it is we deal with it in the readMacro function 
 * else we deal it in the writeMacro function 
*/
void preASM(char *filePath){
    char buffer[MAX_LINE]; 
    char* fileOut = (char *)calloc(MAX_LINE,sizeof(char));
    char* fileIn = (char *)calloc(MAX_LINE,sizeof(char));
    FILE *fRead, *fWrite;
    
    strcpy(fileOut, filePath);
    strcat(fileOut, ".am");
    strcpy(fileIn, filePath);
    strcat(fileIn, ".as");

    fRead = fopen(fileIn, "r");
    fWrite = fopen(fileOut, "w");
    if(!fRead || !fWrite){
    	raiseError("opening/creating file");
        return; 
    }
    
    while (fgets(buffer, MAX_LINE, fRead)) {
        if (strstr(buffer, "macro")) 
            readMacro(buffer, fRead); /* found a macro get the name of it as a key */
        else if (strcmp(buffer, "\n"))
            writeMacro(buffer, &fWrite);   
    }
    fprintf(fWrite, "\n");
    
    /* free the space we used*/
    freeDict(&macroDict);
    fclose(fRead);
    fclose(fWrite);
    free(fileIn);
    free(fileOut);
}

/* 
 * this function get the name of the macro 
 * and copy  everything that comes after it 
 * till it encounters endmacro
 */ 
void readMacro(char *buffer, FILE *fp) {
	char* key = (char *)calloc(MAX_LINE,sizeof(char));
    char* value = (char *)calloc(MAX_LINE * STANDARD_SIZE,sizeof(char));
    char *bufferFormated;
    int count = 0;
    
    bufferFormated = formatCommand(buffer);
    strncpy(key, strstr(bufferFormated, "macro") + strlen("macro "), strlen(bufferFormated) - strlen("macro "));
    
    /* start copy the command */
    while (fgets(buffer, MAX_LINE, fp) && !(strstr(buffer, "endmacro"))) {
    	if (count % (MAX_LINE * STANDARD_SIZE) == 0)
    		value = (char *)realloc(value, (count + MAX_LINE * STANDARD_SIZE) * sizeof(char));
        strcat(value, buffer);
    }  
    
    if (!addNode(&macroDict, key , value, 0)) {
    	errorFlag = TRUE;
		printf("Error: cant add two macros with same name - %s\n", key);
	}
	if (isSavedWord(key)) {
    	errorFlag = TRUE;
		printf("Error: macro name %s, is a saved word\n", key);
	}
    
    free(key);
    free(value);
    free(bufferFormated);
}

/*
 * for each line that is not macro declaration 
 * we check if its macro we get the content of it
 * else we just leave the line as it is  
*/
void writeMacro(char *buffer, FILE **fp) {
    char *temp, *firstWord;
    
    /* we use format command to get rid of any spaces before the first word */
    temp = formatCommand(buffer);
    firstWord = strtok(temp, " ");
    
    
    if (findNode(&macroDict, firstWord)) 
        fprintf(*fp, "%s", findValue(&macroDict, firstWord));
    else 
        fprintf(*fp, "%s", buffer);
        
    free(temp);
}
