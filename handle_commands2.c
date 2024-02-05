#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"
#include "./structs/dictionary.h"


extern char *group1[]; 
extern char *group2[];
extern char *group3[];
extern char *registers[];

extern char *savedCommands[];
extern char **commands;
extern dictNode *entries, *externs, *symbols, *externSymbols;

extern int errorFlag;

void checkMioonCode(char *command, char *buffer, int *l, int IC);
void completeMioonSingleArg (char *arg, char *mioon, int IC);

/*
 * getting  the first 3 letters since they should reprsent the command name 
 * checking the index of the commnad by searching it in the array 
 * change the decimal to binary  and  save  the  command  binary code 
*/
int handleCommands2(char *buffer, char *command, int IC) {
	int l = 1; 
	
	/* if command is valid */
	if (indexInArray(command, savedCommands, SAVED_COMMANDS_LENGTH) != -1)
		checkMioonCode(command, buffer, &l, IC);
	
	return l;
}
/*
 * finishes writting the missing mioon of arguments.
 * seprate the arguments and using complete mioon single arg. 
*/
void completeMioonTwoArg (char *arg1, char *arg2, char *mioon1, char *mioon2, int IC) {
	if (arg1 && arg2) {
		if (!strcmp(mioon1, MIOON_1) || !strcmp(mioon1, MIOON_2)) 
			completeMioonSingleArg(arg1, mioon1, IC);

		IC += (strcmp(mioon1, MIOON_2)) ? 1 : 2;
		if (!strcmp(mioon2, MIOON_1) || !strcmp(mioon2, MIOON_2)) 
			completeMioonSingleArg(arg2, mioon2, IC);
	}
}
/*
 * finishes writting the missing mioon of arguments. 
*/
void completeMioonSingleArg (char *arg, char *mioon, int IC) {
	char *bin = (char *)calloc(CHAR_10_BIT, sizeof(char));
	char *tempArg = (char *)calloc(MAX_LINE, sizeof(char));
	char *value, *temp, *binNum, *tempBase32;
	
	IC++;
	strcpy(tempArg, arg);
	if (!strcmp(mioon, MIOON_2)) /* when dealing with this mioon we only want the label which comes before the .*/
		strtok(tempArg, ".");
	
	if (findNode(&externs, tempArg)) {
		/* arg  IC */
		tempBase32 = decToBase32(IC);
		addNode(&externSymbols, tempArg, tempBase32, 1);
		strcpy(commands[IC], "0000000001");
		free(tempBase32);
	}
	else {
		value = findValue(&symbols, tempArg);
		if (value) {
			binNum = decToBin(atoi(value), 0);
			temp = addingZero(binNum, COMMANDS_INT_REP_SIZE);
			strcpy(bin, temp);
			strcat(bin, REL_ARE);
			strcpy(commands[IC], bin);
		}
		else {
			printf("Error: label - %s, not defined\n", tempArg);
			errorFlag = TRUE;
		}
	}
	free(bin);
	free(tempArg);
}
/*
 * 
 */ 
void checkMioonCode(char *command, char *buffer, int *l, int IC) {
	char *mioon1 = (char *)calloc(CHAR_2_BIT,sizeof(char)), *mioon2 = (char *)calloc(CHAR_2_BIT,sizeof(char));
	char *token1 = (char *)calloc(MAX_LINE,sizeof(char)), *token2 = (char *)calloc(MAX_LINE,sizeof(char));
	char *temp = (char *)calloc(MAX_LINE,sizeof(char));
	char *tempP = temp, *token2P = token2, *token1P = token1, *mioon1P = mioon1, *mioon2P = mioon2;

	if (stringInArray(command, group1, GROUP1_LENGTH)) {
		/* first argument */
		temp = strstr(buffer, command);
		token1 = strtok(temp + strlen(command) + 1, ",");
		mioon1 = mioon(token1);
		/* second argument */
		token2 = strtok(NULL, ",");
		mioon2 = mioon(token2);
		if (!strcmp(mioon1, MIOON_1) || !strcmp(mioon1, MIOON_2) ||
		 	!strcmp(mioon2, MIOON_1) || !strcmp(mioon2, MIOON_2)) 
			completeMioonTwoArg(token1, token2, mioon1, mioon2, IC);
		*l += calculateL(mioon1, mioon2);
	}
	else if (stringInArray(command, group2, GROUP2_LENGTH)) {
		/* the second group has only 1 argument. the mioon is for the second one */
		temp = strstr(buffer, command);
		token1 = temp + strlen(command) + 1;
		mioon1 = mioon(token1);
		if (!strcmp(mioon1, MIOON_1) || !strcmp(mioon1, MIOON_2))
			completeMioonSingleArg(token1, mioon1, IC);
		*l = (!strcmp(mioon1, MIOON_2)) ? *l + 2 : *l + 1;
	}
	
	free(tempP);
	free(mioon1P);
	free(token1P);
	free(mioon2P);
	free(token2P);
}
