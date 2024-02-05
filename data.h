#pragma once
#ifndef _DATA_H_
#define _DATA_H_

#include "./structs/dictionary.h"

#define MAX_LINE 81 /* Max length of a line is 80 as said in the course book*/
#define FALSE 0 /* define false as 0*/
#define TRUE 1 /* defime true as 1*/
#define SAVED_COMMANDS_LENGTH 16 /* the length of the commands array*/
#define GROUP1_LENGTH 5 /* the length of group1 arrary*/
#define GROUP2_LENGTH 9 /* the length of group2 arrary*/
#define GROUP3_LENGTH 2 /* the length of group3 arrary*/
#define SAVED_INSTRUCTS_LENGTH 5 /* the length of SAVED_INSTRUCTS arrary*/
#define RESIGTERS_LENGTH 8 /* the length of RESIGTERS arrary */
#define STANDARD_SIZE 256 /* a standart size of arrays (the amount of memory to use)*/
#define IC_DC_START 100 /* proggram start from address 100 in memory*/
#define CHAR_10_BIT 11  /* each command reprsented in 10 bits (+1 for \0)*/
#define CHAR_8_BIT 9
#define CHAR_4_BIT 5 
#define CHAR_2_BIT 3 
#define INSTRUCTION_INT_REP_SIZE 10 /*the size of intgers in instructions*/
#define COMMANDS_INT_REP_SIZE 8 /*the size of intgers in instructions*/

#define ABS_ARE "00"
#define REL_ARE "10"
#define EXT_ARE "11"
#define MIOON_0 "00"
#define MIOON_1 "01"
#define MIOON_2 "10"
#define MIOON_3 "11"
#define STAM_CHAR "#" /* stam char */


/* Arrays for saved words*/
extern char *savedCommands[];
extern char *group1[];
extern char *group2[];
extern char *group3[];
extern char *registers[];
extern char *savedInstructs[];


/* 
=============================================
				Pre Assembler 
=============================================
*/

/*
    this function recivces the name of the file open it for reading 
    for each of the line we check if it declare on new macro
    if it is we deal with it in the readMacro function 
    else we deal it in the writeMacro function 
*/
void preASM(char *file);

/* 
=============================================
				Assembler Part 1
=============================================
*/

/* main function used to open the am file for reating  */
void asmPart1(char *filePath);
/*this function checks what type of instrction is enterned and sends to the relvant function*/
void handleInstructs1(char *buffer, char *token, int *DC);
/* this function takes care of the enterd commands*/
int handleCommands1(char *buffer, char *command, int IC);
/* this funtion is used the calculate the size of L depends on the mioon type*/ 
int calculateL(char *mioon1, char *mioon2);


/* 
=============================================
				Assembler Part 2 
=============================================
*/
/* the function is used to open the am file after the 1st run */
void asmPart2(char * filePath);
/* this files takes care of the entries in the 2nd run of the assembler*/
void handleInstructs2(char *buffer, char *token, int *DC);
/* this function takes care of the enterd commands*/
int handleCommands2(char *buffer, char *token, int IC);

/* Functions that change bases */
/*
	this function converts decimal numbers 
	to their binary form. 
	also takes care for negtive number with the 
	2 complaminet mothed
*/
char* decToBin(int num, int limit);
/* used to convers binary represantion of a number back to his decimal value */
int binToDec(char* bin);
/*
	takes a binary reprsentation of a string 
	checks if the lenght is divisable by 5 if not adds zero at start 
	then takes 5 charcters every  time and converts it to it base 32 
*/
char* binToBase32(char *bin);
/* converts decimal number to base 32 */
char* decToBase32(int x);

/* 
=============================================
					utils.c 
=============================================
*/

/* check if string is a saved word */
int isSavedWord(char *str); 
/* returns mioon code of token */
char* mioon(char* token);
/* the function checks if there is a  given string in an array */
int stringInArray(char * string, char * arr[], int n);
/* the function given a input string, checks if it's a number.*/
int isNumber(char *s);
/* removes all white spaces from a string*/
void removeWSpaces(char* s);
/*
	the function takes a given strng and reformat it by:
	removing extra spaces only when needed  
	for example for command like:
			.hel llo   r1	,  , world
	the return string would be: .hel llo r1,,world
*/
char* formatCommand(char* str);
/*
	return the index of given ellemnt in the array
	if not exists return -1
*/
int indexInArray(char * s, char *arr[], int n);
/*
	converts integer in given base to a string 
	for example the int 567 would become "567"
*/
char *itoa(int val, int base);
/* any caught error will be printed and raise the flag */
void raiseError(char *msg);
/*count the amount of commas in a string*/
int countCommas(char *s);
/*
	this function is used to add zero at the beggining of given string
	to make the string at size of 10 
	it used for cases like when the opeartion command number is less 8 
	since  any nember less then 8 reprsented in binary with up to 3 bits 
	but the command use 4 so we add zeros
*/
char* addingZero(char* string, int len);

void nicePrint(char *st, dictNode *dict);
#endif