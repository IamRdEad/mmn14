#ifndef _DICT_H
#define _DICT_H

typedef struct node dictNode;

/*  create a new dictionary node with given key and value*/
dictNode *setNode(char *key, char *value);
/*
	add a new node to a given dictonary based on given key and value 
	@param duplicateFlag - 1 for your dictorany to have duplicated keys
	0 for your dictorany to have duplicated keys
*/
int addNode(dictNode** dict, char* key, char* value, int duplicateFlag);
/* print a given dicitonary*/
void printDict(dictNode* dict);
/* return a value of given dictonary in case key doesnt exists return null*/
char *findValue(dictNode **dict, char *key);
/* change the value of a given key to a new value*/
int setValue(dictNode **dict, char *key, char *newValue);
/* get value of specific node */
char *getValue(dictNode *dict);
/* get key of specific node */
char *getKey(dictNode *dict);
/* get next of specific node */
dictNode *getNext(dictNode *dict);
/* checks of given key is in the dicitonary if exists return 1 else reutn 0*/
int findNode(dictNode** dict, char* key);
/* free the memory used by the dicitonary */
void freeDict(dictNode** dict);
#endif
