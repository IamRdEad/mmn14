#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "data.h"
#include "./structs/dictionary.h"


extern char *group1[]; 
extern char *group2[];
extern char *group3[];
extern char *registers[];

extern char *savedCommands[];
extern char **commands;
extern int errorFlag;

void addMioonCode(char *command, char *commandBinary, char *buffer, int *l, int IC);

/*
 * getting the first 3 letters since they should reprsent the command name 
 * checking the index of the commnad by searching it in the array 
 * change the decimal to binary and save the command binary code 
*/
int handleCommands1(char *buffer, char *command, int IC) {
	char *commandBinary = (char *)calloc(CHAR_10_BIT,sizeof(char));
	char *bin = (char *)calloc(CHAR_4_BIT,sizeof(char));
	char *temp, *binNum;
	int commandCode, l = 1; 
	

	commandCode = indexInArray(command, savedCommands, SAVED_COMMANDS_LENGTH); /* command index */
	binNum = decToBin(commandCode, 0);
	temp = addingZero(binNum, 4);
	strcat(commandBinary, temp);
	addMioonCode(command, commandBinary, buffer, &l, IC);
	
	free(commandBinary);
	free(temp);
	free(binNum);
	free(bin);
	return l;
}

/* this funtion is used the calculate the size of L depends on the mioon type*/ 
int calculateL(char *mioon1, char *mioon2) {
	int L = 0;
	if (!strcmp(mioon1, MIOON_3) || !strcmp(mioon2, MIOON_3))
		L++;
	if (!strcmp(mioon1, MIOON_2))
		L += 2;
	if (!strcmp(mioon2, MIOON_2))
		L += 2;
	if (!strcmp(mioon1, MIOON_1) || !strcmp(mioon1, MIOON_0))
		L++;
	if (!strcmp(mioon2, MIOON_1) || !strcmp(mioon2, MIOON_0))
		L++;
	return L;
}

/* 
 * based on 2 registers (max) this function returns a string in binary that represents the argument
 * the format is XXXX-YYYY-00 where XXXX is one register and YYYY is another.
 * @param r1 is the first argument
 * @param r2 is the second argument
 * @param numOfArgs is how many args we have
 * @param whichArg tells us which is the register. 0 is both, 1 if left, 2 is right.
*/
char* getRegisterCode(char *reg1, char *reg2, int numOfArgs, int whichArg) {
	char *result = (char *)calloc(CHAR_10_BIT, sizeof(char));
	int regNum;
	
	if (numOfArgs == 2 && whichArg == 0) { /* both are registers */
		regNum = indexInArray(reg1, registers, RESIGTERS_LENGTH);
		strcat(result, addingZero(decToBin(regNum, 0), 4));
		
		regNum = indexInArray(reg2, registers, RESIGTERS_LENGTH);
		strcat(result, addingZero(decToBin(regNum, 0), 4));
	} 
	else if (numOfArgs == 2 && whichArg == 1) { 
		regNum = indexInArray(reg1, registers, RESIGTERS_LENGTH);
		strcat(result, addingZero(decToBin(regNum, 0), 4));
		strcat(result, "0000");
	}
	else if ((numOfArgs == 2 && whichArg == 2) || numOfArgs == 1) {
		strcat(result, "0000");
		regNum = indexInArray(reg2, registers, RESIGTERS_LENGTH);
		strcat(result, addingZero(decToBin(regNum, 0), 4));
	}
	
	strcat(result, ABS_ARE);
	return result;
}

/* handles the arguments
 * gets an argument and figures out the code for it.
 */  
void handleArg(int *IC, char *arg, char *mioon, int numOfArgs, int whichArg) {
	char *tempBin = (char *)calloc(CHAR_10_BIT, sizeof(char));
	int limit = pow(2, COMMANDS_INT_REP_SIZE);
	char *tempStr, *temp, *binNum;
	
	/* Mioon number 4, with registers. use the register code function */
	if (!strcmp(mioon, MIOON_3)) {
		if ((numOfArgs == 2 && whichArg == 2) || numOfArgs == 1)
			strcpy(commands[(*IC)++], getRegisterCode("", arg, numOfArgs, whichArg));
		else 
			strcpy(commands[(*IC)++], getRegisterCode(arg, "", numOfArgs, whichArg));
	}
	/* Mioon number 3, when dealing with struct for now we only want the number */
	else if (!strcmp(mioon, MIOON_2)) { 
		(*IC)++;
		tempStr = strstr(arg, ".") + 1;
		binNum = decToBin(atoi(tempStr), 0);
		temp = addingZero(binNum, COMMANDS_INT_REP_SIZE);
		strcat(tempBin, temp);
		strcat(tempBin, ABS_ARE);
		strcpy(commands[(*IC)++], tempBin);
		free(temp);
	}
	
	/* Mioon number 1, we just want to get the number and write it down */
	else if (!strcmp(mioon, MIOON_0) && arg) {
		if (!isNumber(++arg)) { 
			printf("Error: %s isn't a legal number.\n", arg);
			errorFlag = TRUE;
		}
		else if (atoi(arg) >= limit || atoi(arg) < limit * -1)  
				raiseError("number too big / small");
		else {
			if (atoi(arg) < 0) 
				temp = decToBin(atoi(arg), COMMANDS_INT_REP_SIZE);
			else {
				binNum = decToBin(atoi(arg), 0);
				temp = addingZero(binNum, COMMANDS_INT_REP_SIZE);
			}
			strcpy(tempBin, temp);
			strcat(tempBin, ABS_ARE);
			strcpy(commands[(*IC)++], tempBin);
			free(tempBin);
		}
	}
	/* Mioon number 2, for now we can't do anything */
	else 
		(*IC)++;
}

/* this gets both arguments and seperates them to handle via handleArg() */
void handleArgumentCodes(int IC, char *arg1, char *arg2, char *mioon1, char *mioon2, int numOfArgs) {
	IC++;
	if (!strcmp(mioon1, MIOON_3) && !strcmp(mioon2, MIOON_3))  /* both args are registers */
		strcpy(commands[IC++], getRegisterCode(arg1, arg2, numOfArgs, 0));
	else {
		if (numOfArgs > 0) handleArg(&IC, arg1, mioon1, numOfArgs, 1);
		if (numOfArgs == 2) handleArg(&IC, arg2, mioon2, numOfArgs, 2);
	}
}
	
/* this function is used to initialize the commands array */
void initializeCommandsArr(int IC, int l) {
	int n, i;
	for (i = 0; i < l; i++) {
		n = (IC) + i;
		if (n % STANDARD_SIZE == 0) 
			commands = (char **)realloc(commands, (n + STANDARD_SIZE) * sizeof(char*));
		commands[n] = (char *)calloc(CHAR_10_BIT, sizeof(char));
	}
}

/* checking if the type of the mioon is valid based on the command */
int checkValidMioon(char *mioon1, char *mioon2, char *command) {
	int valid = TRUE;
	
	if (!strcmp(command, "mov") || !strcmp(command, "add") || !strcmp(command, "sub")) 
		if (!strcmp(mioon2, MIOON_0))
			valid = FALSE;
	if (!strcmp(command, "not") || !strcmp(command, "clr") || !strcmp(command, "inc") || !strcmp(command, "dec"))
		if (!strcmp(mioon2, MIOON_0))
			valid = FALSE;
	if (!strcmp(command, "jmp") || !strcmp(command, "bne") || !strcmp(command, "get") || !strcmp(command, "jsr"))
		if (!strcmp(mioon2, MIOON_0))
			valid = FALSE;
	if (!strcmp(command, "lea"))
		if (!strcmp(mioon2, MIOON_0) || !strcmp(mioon1, MIOON_0) || !strcmp(mioon1, MIOON_3))
			valid = FALSE;
	
	if (!valid) {
		printf("Error: unfitting operand in command %s\n", command);
		errorFlag = TRUE;
	}
	return valid;
}

/*
 * adds the mioon code for command 
 * @param command is string of the command itself
 * @param commandBinary is string with the final result. it currently should contain only the binary 	of the command
 * @param buffer is the entire line
 * @param l is pointer that'll update based on how many different binary codes we'll have
 */
void addMioonCode(char *command ,char *commandBinary, char *buffer, int *l, int IC) {
	char *temp = (char *)calloc(MAX_LINE,sizeof(char));
	char *mioon1 = (char *)calloc(CHAR_2_BIT,sizeof(char)), *mioon2 = (char *)calloc(CHAR_2_BIT,sizeof(char));
	char *token1 = (char *)calloc(MAX_LINE,sizeof(char)), *token2 = (char *)calloc(MAX_LINE,sizeof(char));
	/* we keep pointers to the start of the allocation so we can free everything after moving around*/
	char *tempP = temp, *token2P = token2, *token1P = token1, *mioon1P = mioon1, *mioon2P = mioon2;
	int numOfArgs = 0;
	
	temp = strstr(buffer, command);
	if (stringInArray(command, group1, GROUP1_LENGTH)) {
		numOfArgs = 2;
		/* first argument */
		token1 = strtok(temp + strlen(command) + 1, ",");
		mioon1 = mioon(token1);
		strcat(commandBinary, mioon1);
		
		/* second argument */
		token2 = strtok(NULL, ",");
		mioon2 = mioon(token2);
		strcat(commandBinary, mioon2);
		/* check if valid and calculate L */
		checkValidMioon(mioon1, mioon2, command);
		*l += calculateL(mioon1, mioon2);
	}
	else if (stringInArray(command, group2, GROUP2_LENGTH)) {
		/* the second group has only 1 argument. the mioon is for the second one */
		numOfArgs = 1;
		strcat(commandBinary, "00");
		/* first and only argument */
		token1 = temp + strlen(command) + 1;
		mioon1 = mioon(token1);
		strcat(commandBinary, mioon1);
		/* check if valid and calculate L */
		checkValidMioon(NULL, mioon1, command);
		*l = (!strcmp(mioon1, MIOON_2)) ? (*l) + 2 : (*l) + 1;
	}
	else {
		strcat(commandBinary, "0000");
	}
	
	/* A-R-E for commands */
	strcat(commandBinary, ABS_ARE);
	/* initialize the commands array based on the calculated l*/
	initializeCommandsArr(IC, *l);
	/* insert the code for the command */
	strcpy(commands[IC], commandBinary);
	/* insert the codes for the arguments */
	handleArgumentCodes(IC, token1, token2, mioon1, mioon2, numOfArgs);
	
	free(tempP);
	free(mioon1P);
	free(token1P);
	free(mioon2P);
	free(token2P);
}
