#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "data.h"
#include "./structs/dictionary.h"

extern int errorFlag;

extern char *savedCommands[], *registers[], *savedInstructs[];

int stringInArray(char * s, char *arr[], int n);
char* mioon(char* token);
int isNumber(char *s);
void removeWSpaces(char* s);
char* formatCommand(char* str);
int indexInArray(char * s, char *arr[], int n);
char* itoa(int val, int base);
char* addingZero(char* string, int len);
void raiseError(char *msg);
int countCommas(char *s);


/* the function checks if there is a  given string in an array */
int stringInArray(char * s, char *arr[], int n) {
	int i, res = FALSE;
	for (i = 0; i < n; i++) {
		if (!strcmp(s, arr[i])) {
			res = TRUE;
		}
	}
	return res;
}

int isSavedWord(char *str) {
	int res = FALSE;
	if (stringInArray(str, savedCommands, SAVED_COMMANDS_LENGTH))
		res = TRUE;
	if (stringInArray(str, registers, RESIGTERS_LENGTH))
		res = TRUE;
	if (stringInArray(str, savedInstructs, SAVED_INSTRUCTS_LENGTH))
		res = TRUE;
		
	return res;
}

/*return the type if the mioon*/
char* mioon(char* token) {
	if (token) {
		if (token[0] == '#') {	/* first mioon*/
			if (!isNumber(token + 1))
				printf("Error: %s isn't a legal number\n", token);
			/* immediate mioon # */
			return MIOON_0;
		} else if (strstr(token, ".")) { 	/* third mioon */
			/* with dot ... */
			return MIOON_2;
		} else if (stringInArray(token, registers, RESIGTERS_LENGTH)) { 		/* forth mioon*/
			/* register mioon */
			return MIOON_3;
		} else { 		/* second mioon */
			/* dircet mioon*/
			return MIOON_1;
		}
	}
	else {
		raiseError("invalid argument");
		return MIOON_1;
	}
}

/* the function given a input string, checks if it's a number.*/
int isNumber(char *s) {
	char* d = s;
	int result = TRUE;
	
	if (!strcmp(d, "+") || !strcmp(d, "-"))
		result = FALSE;
	/* number has to start with a digit or + or -*/
	if (!isdigit(*d) && *d != '+' && *d != '-') 
		result = FALSE;
	d++;
	
    while (*d) {
    	/* if it's not a digit or a dot, we already know it's not a number*/
        if (!isdigit(*d)) 
        	result = FALSE;
        d++;
    }
    
    return result;
}

/* removes all white spaces from a string*/
void removeWSpaces(char* s) {
	char* d = s;
    do {
        while (*d == ' ' || *d == '\t' || *d == '\n' || *d == '\r') {
            ++d;
        }
    } while ((*s++ = *d++));
}

/*
	the function takes a given strng and reformat it by:
	removing extra spaces only when needed  
	for example for command like:
			.hel llo   r1	,  , world
	the return string would be: .hel llo r1,,world
*/
char* formatCommand(char* str) {
	char *result = (char *)calloc(MAX_LINE, sizeof(char));
	char letter[2] = "a";
	int commaFlag = FALSE, raisedError = FALSE;
	
	while (isspace(*str))
		str++;  /* command */
	while (!isspace(*str)) {
		letter[0] = *str;
		strcat(result, letter);
		str++;
	}
	strcat(result, " ");
	
	while (*str) { /* arguments */
		
		while (isspace(*str)) 
			str++; 
		while (*str && !isspace(*str) && *str != ',') {
			commaFlag = FALSE;
			if (*str == '"') {
				strcat(result, "\"");
				str++;
				while (*str && *str != '"') {
					letter[0] = *str;
					strcat(result, letter);
					str++;
				}
			}
			letter[0] = *str;
			strcat(result, letter);
			str++;
		}
		while (isspace(*str))
			str++; 
		if (*str == ',') {
			if (commaFlag) {
				raiseError("missuse of commas");
				raisedError = TRUE;
			}
			commaFlag = TRUE;
			strcat(result, ",");
			str++;
		}
	}
	if (commaFlag && ! raisedError) 
		raiseError("missuse of commas");
	strcat(result, "\0");
	return result;

}

/*
	return the index of given ellemnt in the array
	if not exists return -1
*/
int indexInArray(char * s, char *arr[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (!strcmp(s, arr[i])) {
			return i;
		}
	}
	return -1; /*couldn't find*/
}

/*
	converts integer in given base to a string 
	for example the int 567 would become "567"
*/
char* itoa(int val, int base) { 
	static char buf[32] = {0};
	int i;
	/*  if intger is 0 return "0" needed cuz otherwise it would return '\0'*/
	if(!val)
		return "0";
    for(i=30; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
    
}

/*
	this function is used to add zero at the beggining of given string
	to make the string at size of 10 
	it used for cases like when the opeartion command number is less 8 
	since  any nember less then 8 reprsented in binary with up to 3 bits 
	but the command use 4 so we add zeros
*/
char* addingZero(char* string, int len) {
	char *finalString = (char*)calloc(len, sizeof(char));
	int i; 
	for (i = 0; i < len - (strlen(string)); i++){
		strcat(finalString, "0");
	}
	strcat(finalString, string);
	return finalString;
}

/* any caught error will be printed and raise the flag */
void raiseError(char *msg) {
	printf("Error: %s\n", msg);
	errorFlag = TRUE;
}

/*
	used for counting the commas in given string 
	to see if illagel amount of opreadns in command
*/
int countCommas(char *s) {
	int i = 0, cnt = 0; 
	
	while (s[i]) {
		if (s[i] == ',')
			cnt++;
		i++;
	}
	
	return cnt;
}
/* just a function for printing dictornies in a nice  way*/ 
void nicePrint(char *st, dictNode *dict){
	printf("\n=========================\n\t%s\n=========================\n",st);
	if (dict)
		printDict(dict);
}
