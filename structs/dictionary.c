#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define TRUE 1
#define FALSE 0


typedef struct node {
	char *key;
	char *value;
	struct node* next; 
} node;


/*  create a new dictionary node with given key and value*/
dictNode *setNode(char *key, char *value) {
	dictNode* node;
	node = (dictNode*)calloc(1,sizeof(dictNode));
	node->key = (char*) calloc(strlen(key) + 1, sizeof(char));
	strcpy(node->key, key);
	node->value = (char*)calloc(strlen(value) + 1, sizeof(char));
	strcpy(node->value, value);
	node->next = NULL;
	return node;
}
/* checks of given key is in the dicitonary if exists return 1 else reutn 0*/
int findNode(dictNode** dict, char* key){
	dictNode *ptr = *dict;
	while(ptr != NULL){
		if(!strcmp(ptr->key,key)){
			return TRUE;
		}
		ptr = ptr->next;
	}
	return FALSE;
}
/*
	add a new node to a given dictonary based on given key and value 
	@param duplicateFlag - 1 for your dictorany to have duplicated keys
	0 for your dictorany to have duplicated keys
*/
int addNode(dictNode** dict, char* key, char* value, int duplicateFlag) {
	dictNode *node;
	
	if (!duplicateFlag && findNode(dict,key)) {
		return 0;
	}
	node = setNode(key,value);
	if (*dict == NULL){
		(*dict) = node;  
	}
	else {
		dictNode *ptr = *dict;
		while(ptr->next!=NULL){
			ptr = ptr->next;
		}
		ptr->next = node;
	}
	return 1;
}

/* print a given dicitonary*/
void printDict(dictNode* dict) {
	dictNode *ptr = dict;
	while (ptr != NULL){
		printf("key = %s \t\t value = %s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
}

/* return a value of given dictonary in case key doesnt exists return null*/
char* findValue(dictNode** dict, char *key) {
	dictNode *ptr;
	char *res = NULL;
	ptr = *dict;
	while (ptr != NULL){
		if (!strcmp(ptr->key, key))
			res = ptr->value;
		ptr = ptr->next;
	}
	return res;
}

/* change the value of a given key to a new value*/
int setValue(dictNode **dict, char *key, char *newValue) {
	dictNode *ptr;
	int res = FALSE;
	ptr = *dict;
	while (ptr != NULL){
		if (!strcmp(ptr->key, key)) {
			ptr->value = (char*)calloc(sizeof(newValue), sizeof(char));
			strcpy(ptr->value, newValue);
			res = TRUE;
		}
		ptr = ptr->next;
	}
	return res;
}

char *getValue(dictNode *dict) {
	return dict->value;
}
char *getKey(dictNode *dict) {
	return dict->key;
}
dictNode *getNext(dictNode *dict) {
	return dict->next;
}

/* free the memory used by the dicitonary */
void freeDict(dictNode** dict){
	dictNode *prev;
	if(*dict == NULL)
		return;
	prev = *dict;
	(*dict)=(*dict)->next;
	while(*dict!=NULL){
		free(prev->key);
		free(prev->value);
		free(prev); 
		prev=*dict; 
		(*dict)=(*dict)->next;
	}
}


